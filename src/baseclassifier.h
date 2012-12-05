#ifndef PROJETO_BASECLASSIFIER_H_
#define PROJETO_BASECLASSIFIER_H_

#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/features2d/features2d.hpp>

namespace projeto {

class ImageSet;

class BaseClassifier {
public:
    BaseClassifier(const std::string& detectorName, const std::string& extractorName);
    virtual ~BaseClassifier();
    
    void RunTraining(ImageSet& set);
    void RunTests(ImageSet& set);
    
protected:
    cv::Ptr<cv::FeatureDetector> detector_;
    cv::Ptr<cv::DescriptorExtractor> extractor_;
    cv::Ptr<cv::DescriptorMatcher> matcher_;
    cv::Ptr<cv::BOWTrainer> trainer_;
    cv::Ptr<cv::BOWImgDescriptorExtractor> bowide_;
    cv::Mat vocabulary_;
    
    virtual void doTrain(cv::Mat& samples, cv::Mat& labels) = 0;
    virtual float doPredict(cv::Mat& sample) = 0;
};

}


#endif
