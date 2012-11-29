#include "classifier.h"
#include "imageset.h"
#include <vector>
#include <cstdio>

#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace projeto {

using cv::Mat;
using cv::Ptr;

typedef std::vector< cv::KeyPoint > KPVector;

Classifier::Classifier()  {
    detector_ = Ptr<cv::FeatureDetector>(new cv::SurfFeatureDetector(400)); //minHessian = 400
    extractor_ = Ptr<cv::DescriptorExtractor>(new cv::SurfDescriptorExtractor());
    matcher_ = Ptr<cv::DescriptorMatcher>(new cv::FlannBasedMatcher());
    trainer_ = Ptr<cv::BOWTrainer>(new cv::BOWKMeansTrainer(1000)); //numClusters = 1000
    bowide_ = Ptr<cv::BOWImgDescriptorExtractor>(new cv::BOWImgDescriptorExtractor(extractor_, matcher_));
}
Classifier::~Classifier() {
}

void Classifier::RunTraining(ImageSet& set) {
    /* Step1: use the training set to generate the vocabulary */
    Mat training_descriptors (1, extractor_->descriptorSize(), extractor_->descriptorType());
    KPVector keypoints;
    
    StrVector images = set.GetAllImages();
    StrVector::iterator it;
    for (it = images.begin(); it != images.end(); ++it) {
        Mat img = cv::imread( (*it) );
        detector_->detect(img, keypoints);
        Mat descriptors;
        extractor_->compute(img, keypoints, descriptors);
        training_descriptors.push_back(descriptors);
    }
    
    trainer_->add(training_descriptors);
    printf("Going to clusterize\n");
    vocabulary_ = trainer_->cluster();
    printf("Vocabulary has been calculated\n");
    
    /* Step2: generate image histograms using the vocabulary */
    
    /* Step3: train our classifier using the histograms and image labels */
}
    

}
