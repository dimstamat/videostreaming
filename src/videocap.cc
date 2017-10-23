#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio.hpp"

#include <stdio.h>

#include <iostream>

#include <sys/time.h>


#define SHOW_FPS 1
 
using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
    VideoCapture cap(0); // open the video camera no. 0

    if (!cap.isOpened())  // if not success, exit program
    {
        cout << "Cannot open the video cam" << endl;
        return -1;
    }

   cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
   cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

   double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
   double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

    cout << "Frame size : " << dWidth << " x " << dHeight << endl;

    //namedWindow("MyShrimpTank",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

    cout <<"Created window" <<endl;
    #if SHOW_FPS
    struct timeval frameStart, frameEnd;
    #endif
    // We want our bins to have the same size (uniform) and to clear the histograms in the beginning, so:
    bool uniform = true; bool accumulate = false;

    // we compute the histogram from the 0-th and 1-st channels
    int channels[] = {0, 1};

    Mat curr_frame, prev_frame, curr_frame_hsv, prev_frame_hsv;
    Mat prev_hist, curr_hist;

    // Quantize the hue to 30 levels
    // and the saturation to 32 levels
    int hbins = 30, sbins = 32;
    int histSize[] = {hbins, sbins};
    // hue varies from 0 to 179, see cvtColor
    float hranges[] = { 0, 180 };
    // saturation varies from 0 (black-gray-white) to
    // 255 (pure spectrum color)
    float sranges[] = { 0, 256 };
    const float* histRange[] = { hranges, sranges };
	
    int i=0;


    /*
    Mat img1 = imread("/Users/dimos/Desktop/Screen shot 2014-03-11 at 6.18.59 PM.png");

    Mat img2 = imread("/Users/dimos/Desktop/Screen shot 2014-03-11 at 9.58.04 PM.png");

    Mat img1hsv, img2hsv, img1hist, img2hist;

     cvtColor(img1, img1hsv, CV_BGR2HSV);
     cvtColor(img1, img2hsv, CV_BGR2HSV);

     calcHist( &img1hsv, 1, channels, Mat(), img1hist, 2, histSize, histRange, uniform, accumulate );
     calcHist( &img2hsv, 1, channels, Mat(), img2hist, 2, histSize, histRange, uniform, accumulate );



     //cout << " Prev hist: " << prev_hist << endl;
     //cout << " Current hist: " << curr_hist <<endl;

     double hist_diff = compareHist(img1hist, img2hist, CV_COMP_CORREL);

     printf("Diff: %lf\n", hist_diff);
     */


    
    while (1)
    {
	#if SHOW_FPS
        gettimeofday(&frameStart, NULL);
	#endif
        //cout << "Trying to read frame from camera!" <<endl;
	//cout << "Is video capture opened? " << cap.isOpened()<<endl;
	cap.grab();
	//cout <<"Grabbed frame"<<endl;
	bool bSuccess = cap.read(curr_frame); // read a new frame from video

        //cout << " Read a frame from camera! " << bSuccess << endl;

         if (!bSuccess) //if not success, break loop
        {
             cout << "Cannot read a frame from video stream" << endl;
             break;
        }

        imshow("EthMiningConsumption", curr_frame); //show the frame in "MyVideo" window

        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            cout << "esc key pressed by user" << endl;
            break; 
       }

	

       /*int threshold = 400000;

       if (i > 0){
            Scalar frame_diff = sum(curr_frame - prev_frame);

            if(frame_diff[0] > threshold || frame_diff[1] > threshold ||
		 frame_diff[2] > threshold) 
			cout << "@ "<< ((frameStart.tv_sec)*1000 + (frameStart.tv_usec/1000)) << " MOVEMENT!" <<endl;
       }*/


       if (0){
         cvtColor(prev_frame, prev_frame_hsv, CV_BGR2HSV);
         cvtColor(curr_frame, curr_frame_hsv, CV_BGR2HSV);

         calcHist( &prev_frame_hsv, 1, channels, Mat(), prev_hist, 2, histSize, histRange, uniform, accumulate );
         calcHist( &curr_frame_hsv, 1, channels, Mat(), curr_hist, 2, histSize, histRange, uniform, accumulate );

         //cout << " Prev hist: " << prev_hist << endl;
         //cout << " Current hist: " << curr_hist <<endl;

         double hist_diff = compareHist(prev_hist, curr_hist, CV_COMP_CORREL);
         //if (hist_diff < 0.99 )
           // printf("Diff: %lf\n", hist_diff);
       }

       //sleep(1);

       //usleep(100000);
       #if SHOW_FPS
       gettimeofday(&frameEnd, NULL);
       long int diff = (frameEnd.tv_sec - frameStart.tv_sec)*1000 + (frameEnd.tv_usec - frameStart.tv_usec)/1000;
       //printf("%ld\n", diff);
       int fps = 1000/diff;
       printf("fps: %d\n", fps);
       #endif
       //curr_frame.copyTo(prev_frame);
       //prev_frame = curr_frame;
       i++;
    }
    return 0;

}
