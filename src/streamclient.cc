#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <unistd.h>
#include "udpclient.hh"
#include <stdio.h>

#include <sys/time.h>

#include "streamincludes.hh"

using namespace cv;
using namespace std;


#define SHOW_FPS 1

int main( int argc, char** argv )
{
    int i=0;
	unsigned char* buf;
	size_t len;

	#if SHOW_FPS
    struct timeval frameStart, frameEnd;
    #endif

    UDPClient client = UDPClient("XXXX", 9090);
	client.send((unsigned char*) ACCEPTANCE_KEY, strlen(ACCEPTANCE_KEY));
	namedWindow( "Fish tank", WINDOW_AUTOSIZE ); // Create a window for display.
    while(1){
		#if SHOW_FPS
        gettimeofday(&frameStart, NULL);
    	#endif
		buf = client.receive(len);
		if(buf == nullptr){
			printf("Received buffer is null...\n");
			continue;
		}
		//cout << "Received "<< len<<endl;
		Mat imgenc = Mat(640, 480, CV_8UC3, (unsigned char*)buf);
		Mat img = imdecode(imgenc, IMREAD_COLOR);
		if( img.empty() ) {                     // Check for invalid input
        	cout <<  "Could not create image" << std::endl ;
        	continue;
    	}
    	imshow("Fish tank", img );                // Show our image inside it.
		#if SHOW_FPS
       	gettimeofday(&frameEnd, NULL);
       	long int diff = (frameEnd.tv_sec - frameStart.tv_sec)*1000 + (frameEnd.tv_usec - frameStart.tv_usec)/1000;
       	int fps = 1000/diff;
       	printf("fps: %d\n", fps);
       	#endif
		waitKey(10); // Wait for a keystroke in the window
    }
    return 0;
}

