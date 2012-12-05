#include "baseclassifier.h"
#include "imageset.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <time.h>
#include <opencv2/nonfree/nonfree.hpp>

namespace projeto {

using std::cout;
using std::endl;
using cv::Mat;
using cv::Ptr;

typedef std::vector< cv::KeyPoint > KPVector;


BaseClassifier::BaseClassifier(const std::string& detectorName, const std::string& extractorName) {
    detector_ = cv::FeatureDetector::create(detectorName);
    extractor_ = Ptr<cv::DescriptorExtractor>( cv::DescriptorExtractor::create(extractorName) );
    matcher_ = Ptr<cv::DescriptorMatcher>(new cv::FlannBasedMatcher());
    trainer_ = Ptr<cv::BOWTrainer>(new cv::BOWKMeansTrainer(128)); //numClusters = 1000
    bowide_ = Ptr<cv::BOWImgDescriptorExtractor>(new cv::BOWImgDescriptorExtractor(extractor_, matcher_));
}
BaseClassifier::~BaseClassifier() {
}


void BaseClassifier::RunTraining(ImageSet& set) {
    cout << "Running Training..." << endl;
    /* Step1: use the training set to generate the vocabulary */
    Mat training_descriptors (1, extractor_->descriptorSize(), extractor_->descriptorType());
    KPVector keypoints;
    
    cout << "Getting descriptors" << endl;
    time_t mark = time(NULL);
    ImgInfoVector images = set.GetAllImages();
    ImgInfoVector::iterator it;
    for (it = images.begin(); it != images.end(); ++it) {
        Mat img = it->GetImage();
        detector_->detect(img, keypoints);
        Mat descriptors;
        extractor_->compute(img, keypoints, descriptors);
        training_descriptors.push_back(descriptors);
    }
    trainer_->add(training_descriptors);
    cout << "Descriptors acquired (in " << (time(0)-mark) << " seconds)" << endl;
    
    cout << "Going to clusterize" << endl;
    mark = time(NULL);
    vocabulary_ = trainer_->cluster();
    bowide_->setVocabulary(vocabulary_);
    cout << "Vocabulary has been calculated (in " << (time(0)-mark) << " seconds)" << endl;

    /*Step2: generate image histograms using the vocabulary
      Step3: create sample and label vectors  */
    Mat samples, labels;
    for (it = images.begin(); it != images.end(); ++it) {
        /****/
        Mat img = it->GetImage();
        detector_->detect(img, keypoints);
        Mat hist;
        bowide_->compute(img, keypoints, hist);
        it->set_histogram(hist);

        /****/
        samples.push_back( hist );
        labels.push_back( it->labelMatrix() );
    }
    cout << "Going to train classifier" << endl;
    
    /* Step4: train our classifier using the histograms and image labels */
    Mat floatSamples (samples.rows, samples.cols, CV_32FC1);
    samples.convertTo(floatSamples, CV_32FC1);
    
    mark = time(NULL);
    doTrain(floatSamples, labels);
    cout << "Classifier has been trained (in " << (time(0)-mark) << " seconds)" << endl;
}

void BaseClassifier::RunTests(ImageSet& set) {
    cout << "Running tests..." << endl;
    
    unsigned hits = 0;
    KPVector keypoints;
    
    ImgInfoVector images = set.GetAllImages();
    ImgInfoVector::iterator it;
    for (it = images.begin(); it != images.end(); ++it) {
        Mat img = it->GetImage();
        detector_->detect(img, keypoints);
        Mat hist;
        bowide_->compute(img, keypoints, hist);
        it->set_histogram(hist);
        
        float label = doPredict(hist);
        float expected = it->label();
        
        cout << "TEST " << label << " " << it->classname() << " " << expected;
        if (label == expected) {
            hits++;
            cout << " OK";
        }
        cout << endl;
    }
}
    

}
