#ifndef PROJETO_CLASSIFIER_H_
#define PROJETO_CLASSIFIER_H_

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>

namespace projeto {

class ImageSet;

class Classifier {
public:
    Classifier();
    ~Classifier();
    
    void RunTraining(ImageSet& set);
    
private:
    cv::Ptr<cv::FeatureDetector> detector_;
    cv::Ptr<cv::DescriptorExtractor> extractor_;
    cv::Ptr<cv::DescriptorMatcher> matcher_;
    cv::Ptr<cv::BOWTrainer> trainer_;
    cv::Ptr<cv::BOWImgDescriptorExtractor> bowide_;
    cv::Mat vocabulary_;
};

}


#endif
