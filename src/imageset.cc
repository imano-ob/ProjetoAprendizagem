#include "imageset.h"
#include <dirent.h>
#include <sys/stat.h>
#include <cstdio>
#include <opencv2/core/core.hpp>

namespace projeto {

using std::string;

static std::map< std::string, float > classLabels_;
static float nextLabel_ = 1.0;
    
ImageSet::ImageSet(string basepath) : path_(basepath) {
}
ImageSet::~ImageSet() {
}

void ImageSet::Load() {
    StrVector baseDirList = listDir(path_, ONLY_DIRS);
    string className;
    StrVector::iterator it;
    //printf("Loading ImageSet from %s\n", path_.c_str());
    float label = 1.0;
    for (it=baseDirList.begin(); it != baseDirList.end(); ++it) {
        className = *it;
        classes_.push_back(className);
        if (!classLabels_.count(className)) {
            printf("REGISTER %s %.1f\n", className.c_str(), nextLabel_);
            classLabels_[className] = nextLabel_++;
        }
        label = classLabels_[className];
        
        ImgInfoVector iiv;
        StrVector filenames = listDir(path_+"/"+className, ONLY_FILES);
        
        //printf("\tListing class dir '%s' (label=%.1f):\n", className.c_str(), label);
        StrVector::iterator itFile;
        for (itFile=filenames.begin(); itFile != filenames.end(); ++itFile) {
            ImgInfo info (string(*itFile), path_+"/"+className+"/"+string(*itFile),  className, label);
            images_.push_back( info );
            iiv.push_back( info );
            //printf("\t\t%s\n", itFile->c_str());
        }
        imagePaths_[className] = iiv;
        
        label++;
    }
}

float ImageSet::GetLabelForClass(const std::string& classname) {
    if (classLabels_.count(classname)) {
        return classLabels_[classname];
    }
    return -1.0;
}

ImgInfoVector ImageSet::GetImagesForClass(const string& classname) {
    if (imagePaths_.count(classname)) {
        return imagePaths_[classname];
    }
    return ImgInfoVector();
}

/* Valeu EP1 de SO */
StrVector ImageSet::listDir(const string& path, ListDirMode mode) {
    StrVector list;
    string entryName;
    DIR *dir;
    struct dirent *ent;

    dir = opendir (path.c_str());
    if (dir != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            entryName = string(ent->d_name);
            if ( (entryName == ".") ) continue;
            if ( (entryName == "..") ) continue;

            if ( (mode == ONLY_FILES || mode == ALL) && !isDir(path+"/"+entryName))
                list.push_back(entryName);
            else if ( (mode == ONLY_DIRS || mode == ALL) && isDir(path+"/"+entryName))
                list.push_back(entryName);
        }
        closedir (dir);
    }
    
    return list;
}

bool ImageSet::isDir(const string& path) {
    struct stat file_stat_;
    lstat(path.c_str(), &file_stat_ );
    return S_ISDIR(file_stat_.st_mode) == 1;
}

}
