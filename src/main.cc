#include <cv.h>
#include <highgui.h>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/ml/ml.hpp>
#include "imageset.h"
#include "classifier.h"
#include <iostream>
#include <fstream>
#include <string>

using std::cout;
using std::endl;

int main ( int argc, char **argv ) {    
    if (argc < 6) {
        cout << "Wrong program call. Execute:" << endl;
        cout << "\t" << argv[0] << " <classifier> <descriptor> <extractor> <training_set_dir_path> <test_set_dir_path>" << endl;
        return 0;
    }
    
    cv::initModule_nonfree();
    
    std::string classifierType (argv[1]);
    projeto::BaseClassifier* cla;
    if (classifierType == "BAYES") {
        cla = new projeto::Classifier<CvNormalBayesClassifier>(argv[2], argv[3]);
    }
    else if (classifierType == "KNN") {
        cla = new projeto::Classifier<CvKNearest>(argv[2], argv[3]);
    }
    else if (classifierType == "DTREE") {
        cla = new projeto::Classifier<CvDTree>(argv[2], argv[3]);
    }
    else {
        cout << "ERROR : '" << classifierType << "' Classifier Type passed was not recognized." << endl;
        return 0;
    }
    
    /***/
    
    projeto::ImageSet training ( argv[4] );
    training.Load();
    projeto::ImageSet test ( argv[5] );
    test.Load();
    
    cla->RunTraining(training);
    cla->RunTests(test);
    
    delete cla;
    return 0;
}
