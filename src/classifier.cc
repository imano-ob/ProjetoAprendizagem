#include "classifier.h"
#include "imageset.h"
#include <vector>
#include <cstdio>
#include <time.h>
#include <opencv2/nonfree/nonfree.hpp>

namespace projeto {

using cv::Mat;
using cv::Ptr;

typedef std::vector< cv::KeyPoint > KPVector;

Classifier::Classifier()  {
    detector_ = Ptr<cv::FeatureDetector>(new cv::SurfFeatureDetector(400)); //minHessian = 400
    extractor_ = Ptr<cv::DescriptorExtractor>(new cv::SurfDescriptorExtractor());
    matcher_ = Ptr<cv::DescriptorMatcher>(new cv::FlannBasedMatcher());
    trainer_ = Ptr<cv::BOWTrainer>(new cv::BOWKMeansTrainer(100)); //numClusters = 1000
    bowide_ = Ptr<cv::BOWImgDescriptorExtractor>(new cv::BOWImgDescriptorExtractor(extractor_, matcher_));
    //classifier_ = Ptr<CvRTrees>( new CvRTrees() );
    classifier_ = Ptr<CvNormalBayesClassifier>( new CvNormalBayesClassifier() );
}
Classifier::~Classifier() {
}

void Classifier::RunTraining(ImageSet& set) {
    printf("Running Training...\n");
    /* Step1: use the training set to generate the vocabulary */
    Mat training_descriptors (1, extractor_->descriptorSize(), extractor_->descriptorType());
    KPVector keypoints;
    
    printf("Getting descriptors\n");
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
    printf("Going to clusterize\n");
    time_t mark = time(NULL);
    vocabulary_ = trainer_->cluster();
    printf("Vocabulary has been calculated (in %ld seconds)\n", (time(NULL)-mark));
    
    /* Step2: generate image histograms using the vocabulary */
    Mat hist;
    for (it = images.begin(); it != images.end(); ++it) {
        Mat img = it->GetImage();
        detector_->detect(img, keypoints);
        bowide_->compute(img, keypoints, hist);
        it->set_histogram(hist);
    }
    printf("Generated histograms\n");
    
    /* Step3: train our classifier using the histograms and image labels */
    Mat samples, labels;
    for (it = images.begin(); it != images.end(); ++it) {
        samples.push_back( it->histogram() );
        labels.push_back( set.GetLabelForClass(it->classname()) );
    }
    printf("Going to train classifier\n");
    Mat floatSamples;
    samples.convertTo(floatSamples, CV_32F);
    mark = time(NULL);
    //classifier_->train(floatSamples, CV_ROW_SAMPLE, labels);  //was using this for CvRTrees classifier
    classifier_->train(floatSamples, labels);
    printf("Classifier has been trained (in %ld seconds)\n", (time(NULL)-mark));
}
    

}
