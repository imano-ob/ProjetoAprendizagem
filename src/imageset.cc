#include "imageset.h"
#include <dirent.h>
#include <sys/stat.h>
#include <cstdio>

namespace projeto {

using std::string;

ImageSet::ImageSet(string basepath) : path_(basepath) {
}
ImageSet::~ImageSet() {
}

void ImageSet::Load() {
    StrVector baseDirList = listDir(path_, ONLY_DIRS);
    string className;
    StrVector::iterator it;
    printf("Loading ImageSet from %s\n", path_.c_str());
    for (it=baseDirList.begin(); it != baseDirList.end(); ++it) {
        className = *it;
        classes_.push_back(className);
        imagePaths_[className] = listDir(path_+"/"+className, ONLY_FILES);
        
        printf("\tListing class dir '%s':\n", className.c_str());
        StrVector::iterator itFile;
        for (itFile=imagePaths_[className].begin(); itFile != imagePaths_[className].end(); ++itFile)
            printf("\t\t%s\n", itFile->c_str());
    }
}

StrVector ImageSet::GetImagesForClass(const string& classname) {
    if (imagePaths_.count(classname)) {
        return imagePaths_[classname];
    }
    return StrVector();
}

StrVector ImageSet::listDir(const string& path, ListDirMode mode) {
    StrVector list;
    string entryName;
    DIR *dir;
    struct dirent *ent;

    //chdir(path.c_str());

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
