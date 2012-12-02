#ifndef PROJETO_IMAGESET_H_
#define PROJETO_IMAGESET_H_

#include <string>
#include <vector>
#include <map>
#include "imginfo.h"

namespace cv {
class Mat;
}

namespace projeto {


typedef std::vector<std::string> StrVector;

class ImageSet {
public:
    ImageSet(std::string basepath);
    ~ImageSet();
    
    void Load();
    
    StrVector& GetClasses() { return classes_; }
    float GetLabelForClass(const std::string& classname);
    
    ImgInfoVector GetImagesForClass(const std::string& classname);
    ImgInfoVector& GetAllImages() { return images_; }
    
private:
    std::string path_;
    StrVector classes_;
    ImgInfoVector images_;
    std::map< std::string, ImgInfoVector > imagePaths_;
    
    enum ListDirMode { ONLY_FILES, ONLY_DIRS, ALL };
    StrVector listDir(const std::string& path, ListDirMode mode);
    bool isDir(const std::string& path);
};

}


#endif
