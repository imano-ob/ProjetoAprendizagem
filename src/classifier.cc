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
    trainer_ = Ptr<cv::BOWTrainer>(new cv::BOWKMeansTrainer(50)); //numClusters = 1000
    bowide_ = Ptr<cv::BOWImgDescriptorExtractor>(new cv::BOWImgDescriptorExtractor(extractor_, matcher_));
    //classifier_ = Ptr<CvRTrees>( new CvRTrees() );
    classifier_ = Ptr<CvNormalBayesClassifier>( new CvNormalBayesClassifier() );
}
Classifier::~Classifier() {
}

static void printMatType(const Mat& m, const char* msg) {
    printf("%s:: Matrix Shape=(%d rows, %d cols): Type = %d (%d depth/ %d channels)\n", msg, m.rows, m.cols, m.type(), m.depth(), m.channels());
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
        if (img.data == 0) {
            printf("FUDEU MANOLO %s\n", it->filename().c_str());
        }
        detector_->detect(img, keypoints);
        Mat descriptors;
        extractor_->compute(img, keypoints, descriptors);
        printMatType(descriptors, it->filename().c_str());
        training_descriptors.push_back(descriptors);
    }
    printMatType(training_descriptors, "TRAINING DESCRIPTORS");
    trainer_->add(training_descriptors);
    printf("Going to clusterize\n");
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
        labels.push_back( set.GetLabelForClass(it->classname()) );
    }
    printf("Going to train classifier\n");
    
    /* Step4: train our classifier using the histograms and image labels */
    printMatType(samples, "SAMPLES");

    Mat floatSamples (floatSamples.rows, floatSamples.cols, CV_32FC1);
    samples.convertTo(floatSamples, CV_32FC1);
    printMatType(floatSamples, "FLOATSAMPLES");
    printMatType(labels, "LABELS");
    
    CvMat porra = floatSamples;
    printf("FloatSamples is MAT = %d\n", CV_IS_MAT(&porra) );
    printf("FloatSamples type check = %d = %d\n", CV_MAT_TYPE(porra.type), CV_32FC1 );
    mark = time(NULL);
    //classifier_->train(floatSamples, CV_ROW_SAMPLE, labels);  //was using this for CvRTrees classifier
    classifier_->train(floatSamples, labels);
    printf("Classifier has been trained (in %ld seconds)\n", (time(NULL)-mark));
}
    

}
