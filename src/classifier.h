#ifndef PROJETO_CLASSIFIER_H_
#define PROJETO_CLASSIFIER_H_

#include "baseclassifier.h"
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/features2d/features2d.hpp>

namespace projeto {

template <class T>
class Classifier : public BaseClassifier {
public:
    Classifier(const std::string& detectorName, const std::string& extractorName) : BaseClassifier(detectorName, extractorName) {
        classifier_ = cv::Ptr<T>( new T() );
    }
    ~Classifier() {}
    
protected:
    cv::Ptr<T> classifier_;
    
    void doTrain(cv::Mat& samples, cv::Mat& labels) {
        classifier_->train(samples, labels);
    }
    float doPredict(cv::Mat& sample) {
        return classifier_->predict(sample);
    }
};

/***Specialization for KNearest */
template <>
class Classifier<CvKNearest> : public BaseClassifier {
public:
    Classifier(const std::string& detectorName, const std::string& extractorName) : BaseClassifier(detectorName, extractorName) {
        classifier_ = cv::Ptr<CvKNearest>( new CvKNearest() );
    }
    ~Classifier() {}
    
protected:
    cv::Ptr<CvKNearest> classifier_;
    
    void doTrain(cv::Mat& samples, cv::Mat& labels) {
        classifier_->train(samples, labels);
    }
    float doPredict(cv::Mat& sample) {
        return classifier_->find_nearest(sample, 3);
    }
};

/***Specialization for DecisionTree */
template <>
class Classifier<CvDTree> : public BaseClassifier {
public:
    Classifier(const std::string& detectorName, const std::string& extractorName) : BaseClassifier(detectorName, extractorName) {
        classifier_ = cv::Ptr<CvDTree>( new CvDTree() );
    }
    ~Classifier() {}
    
protected:
    cv::Ptr<CvDTree> classifier_;
    
    void doTrain(cv::Mat& samples, cv::Mat& labels) {
        classifier_->train(samples, CV_ROW_SAMPLE, labels);
    }
    float doPredict(cv::Mat& sample) {
        return classifier_->predict(sample)->value;
    }
};

}


#endif
