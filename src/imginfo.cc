#include "imginfo.h"
#include <opencv2/highgui/highgui.hpp>

namespace projeto {

cv::Mat ImgInfo::GetImage() {
    return cv::imread( filename_, CV_LOAD_IMAGE_GRAYSCALE);//CV_LOAD_IMAGE_COLOR );
}

cv::Mat ImgInfo::labelMatrix() {
    return cv::Mat(1, 1, CV_32FC1, label_);
}

}
