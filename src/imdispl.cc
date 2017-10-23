#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <unistd.h>
#include <stdio.h>

#include <sys/time.h>

using namespace cv;
using namespace std;


#define SHOW_FPS 1

int main( int argc, char** argv )
{
    int i=0;
    vector<unsigned char> vecbuf;
	Mat image;
	#if SHOW_FPS
    struct timeval frameStart, frameEnd;
    #endif


	namedWindow( "Display window", WINDOW_AUTOSIZE ); // Create a window for display.
    while(i<100){
		#if SHOW_FPS
        gettimeofday(&frameStart, NULL);
    	#endif

    	std::stringstream ss;
    	ss<< "frame"<<i<<".jpg";
    	String imageName( ss.str() ); // by default
    	ss.str("");
    	if( argc > 1)
    	{
        	imageName = argv[1];
    	}
		image = imread( imageName, IMREAD_COLOR ); // Read the file
		if( image.empty() ) {                     // Check for invalid input
        	cout <<  "Could not create image" << std::endl ;
        	return -1;
    	}
    	imshow( "Display window", image );                // Show our image inside it.
		#if SHOW_FPS
       	gettimeofday(&frameEnd, NULL);
       	long int diff = (frameEnd.tv_sec - frameStart.tv_sec)*1000 + (frameEnd.tv_usec - frameStart.tv_usec)/1000;
       	int fps = 1000/diff;
       	printf("fps: %d\n", fps);
       	#endif
		waitKey(1); // Wait for a keystroke in the window
		i++;
    }
    return 0;
}

