#ifndef PROJETO_IMAGESET_H_
#define PROJETO_IMAGESET_H_

#include <string>
#include <vector>
#include <map>

namespace projeto {

typedef std::vector<std::string> StrVector;

class ImageSet {
public:
    ImageSet(std::string basepath);
    ~ImageSet();
    
    void Load();
    
    StrVector GetImagesForClass(const std::string& classname);
    StrVector& GetClasses() { return classes_; }
    StrVector& GetAllImages() { return images_; }
    
private:
    std::string path_;
    StrVector classes_;
    StrVector images_;
    std::map< std::string, StrVector > imagePaths_;
    
    enum ListDirMode { ONLY_FILES, ONLY_DIRS, ALL };
    StrVector listDir(const std::string& path, ListDirMode mode);
    bool isDir(const std::string& path);
};

}


#endif
