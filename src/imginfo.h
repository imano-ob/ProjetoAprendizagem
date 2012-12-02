#ifndef PROJETO_IMGINFO_H_
#define PROJETO_IMGINFO_H_

#include <vector>
#include <string>
#include <opencv2/core/core.hpp>

namespace projeto {

class ImgInfo {
public:
    ImgInfo(std::string name, std::string filename, std::string classname, float label) : name_(name), filename_(filename), classname_(classname), label_(label) {}
    ~ImgInfo() { }
    
    cv::Mat GetImage();
    const std::string& name() const { return name_; }
    const std::string& filename() const { return filename_; }
    const std::string& classname() const { return classname_; }
    cv::Mat& histogram() { return histogram_; }
    void set_histogram(const cv::Mat& hist) { histogram_ = hist; }
    
    float label() const { return label_; }
    cv::Mat labelMatrix();
    
private:
    std::string name_;
    std::string filename_;
    std::string classname_;
    float label_;
    cv::Mat histogram_;
    
};


typedef std::vector<ImgInfo> ImgInfoVector;

}


#endif
