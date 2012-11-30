#ifndef PROJETO_IMGINFO_H_
#define PROJETO_IMGINFO_H_

#include <vector>
#include <string>
#include <opencv2/core/core.hpp>

namespace projeto {

class ImgInfo {
public:
    ImgInfo(std::string filename, std::string classname) : filename_(filename), classname_(classname) {}
    ~ImgInfo() { }
    
    cv::Mat GetImage();
    const std::string& filename() const { return filename_; }
    const std::string& classname() const { return classname_; }
    cv::Mat& histogram() { return histogram_; }
    void set_histogram(const cv::Mat& hist) { histogram_ = hist; }
    
private:
    std::string filename_;
    std::string classname_;
    cv::Mat histogram_;
    
};


typedef std::vector<ImgInfo> ImgInfoVector;

}


#endif
