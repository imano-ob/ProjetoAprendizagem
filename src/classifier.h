#ifndef PROJETO_CLASSIFIER_H_
#define PROJETO_CLASSIFIER_H_

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"

namespace projeto {

class Classifier {
public:
    Classifier();
    ~Classifier();
    
    void RunTraining(const std::string& path);
    
private:
    cv::Ptr<cv::FeatureDescriptor> descriptor_;
    cv::Ptr<cv::DescriptorExtractor> extractor_;
    cv::Mat vocabulary_;
};

}


#endif
