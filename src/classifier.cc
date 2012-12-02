#include "classifier.h"
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

Classifier::Classifier()  {
    detector_ = Ptr<cv::FeatureDetector>(new cv::SurfFeatureDetector(400)); //minHessian = 400
    extractor_ = Ptr<cv::DescriptorExtractor>(new cv::SurfDescriptorExtractor());
    matcher_ = Ptr<cv::DescriptorMatcher>(new cv::FlannBasedMatcher());
    trainer_ = Ptr<cv::BOWTrainer>(new cv::BOWKMeansTrainer(200)); //numClusters = 1000
    bowide_ = Ptr<cv::BOWImgDescriptorExtractor>(new cv::BOWImgDescriptorExtractor(extractor_, matcher_));
    //classifier_ = Ptr<CvRTrees>( new CvRTrees() );
    classifier_ = Ptr<CvNormalBayesClassifier>( new CvNormalBayesClassifier() );
}
Classifier::~Classifier() {
}

static void printMatType(const Mat& m, const char* msg) {
    cout << msg << ":: Matrix Shape=(" << m.rows << " rows, " << m.cols << " cols): Type = ";
    cout << m.type() << " (" << m.depth() << " depth/ " << m.channels() << " channels)" << endl;
}

void Classifier::RunTraining(ImageSet& set) {
    cout << "Running Training..." << endl;
    /* Step1: use the training set to generate the vocabulary */
    Mat training_descriptors (1, extractor_->descriptorSize(), extractor_->descriptorType());
    KPVector keypoints;
    
    cout << "Getting descriptors" << endl;
    ImgInfoVector images = set.GetAllImages();
    ImgInfoVector::iterator it;
    for (it = images.begin(); it != images.end(); ++it) {
        Mat img = it->GetImage();
        detector_->detect(img, keypoints);
        Mat descriptors;
        extractor_->compute(img, keypoints, descriptors);
        printMatType(descriptors, it->filename().c_str());
        training_descriptors.push_back(descriptors);
    }
    printMatType(training_descriptors, "TRAINING DESCRIPTORS");
    trainer_->add(training_descriptors);
    cout << "Going to clusterize" << endl;
    time_t mark = time(NULL);
    vocabulary_ = trainer_->cluster();
    bowide_->setVocabulary(vocabulary_);
    printf("Vocabulary has been calculated (in %ld seconds)\n", (time(NULL)-mark));
    printMatType(vocabulary_, "VOCABULARY");
    
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
        printMatType(hist, it->filename().c_str());

        /****/
        samples.push_back( hist );
        printMatType(samples, "BUILDING SAMPLES");
        labels.push_back( it->labelMatrix() );
    }
    cout << "Going to train classifier" << endl;
    
    /* Step4: train our classifier using the histograms and image labels */
    printMatType(samples, "SAMPLES");

    Mat floatSamples (samples.rows, samples.cols, CV_32FC1);
    samples.convertTo(floatSamples, CV_32FC1);
    printMatType(floatSamples, "FLOATSAMPLES");
    printMatType(labels, "LABELS");
    
    CvMat porra = floatSamples;
    printf("FloatSamples is MAT = %d\n", CV_IS_MAT(&porra) );
    printf("FloatSamples type check = %d = %d\n", CV_MAT_TYPE(porra.type), CV_32FC1 );
    mark = time(NULL);
    //classifier_->train(floatSamples, CV_ROW_SAMPLE, labels);  //was using this for CvRTrees classifier
    classifier_->train(floatSamples, labels); //was using this for CvNormalBayesClassifier classifier
    cout << "Classifier has been trained (in " << (time(0)-mark) << " seconds)" << endl;
}

void Classifier::RunTests(ImageSet& set) {
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
        
        float label = classifier_->predict(hist);
        float expected = it->label();
        
        if (label == expected) {
            hits++;
            cout << "{HUZZAH}[" << it->classname() << "] " << it->name() << ": predicted as " << label << " (expecting " << expected << ")" << endl;
        }
        else {
            cout << "{------}[" << it->classname() << "] " << it->name() << ": predicted as " << label << " (expecting " << expected << ")" << endl;
        }
    }
    
    cout << "Total number of correct results: ";
    cout << hits << "/" << images.size() << endl;
}
    

}
