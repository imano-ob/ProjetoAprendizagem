#include <cv.h>
#include <highgui.h>
#include "imageset.h"
#include "classifier.h"
#include <iostream>
#include <fstream>

using std::cout;
using std::endl;

int main ( int argc, char **argv ) {
    /*cvNamedWindow( "My Window", 1 );
    IplImage *img = cvCreateImage( cvSize( 640, 480 ), IPL_DEPTH_8U, 1 );
    CvFont font;
    double hScale = 1.0;
    double vScale = 1.0;
    int lineWidth = 1;
    cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, hScale, vScale, 0, lineWidth );
    cvPutText( img, "Hello World!", cvPoint( 200, 400 ), &font, cvScalar( 255, 255, 0 ) );
    cvShowImage( "My Window", img );
    cvWaitKey();*/
    
    if (argc < 3) {
        cout << "Wrong program call. Execute:" << endl;
        cout << "\t" << argv[0] << " <training_set_dir_path> <test_set_dir_path>" << endl;
        return 0;
    }
    
    projeto::ImageSet training ( argv[1] );
    training.Load();
    projeto::ImageSet test ( argv[2] );
    test.Load();
    
    projeto::Classifier cla;
    cla.RunTraining(training);
    cla.RunTests(test);
    
    return 0;
}
