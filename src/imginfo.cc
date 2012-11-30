#include "imginfo.h"
#include <opencv2/highgui/highgui.hpp>

namespace projeto {

cv::Mat ImgInfo::GetImage() {
    return cv::imread( filename_ );
}

}
