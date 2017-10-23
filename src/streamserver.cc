#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio.hpp"

#include "udpserver.hh"

#include <stdio.h>
#include <iostream>

#include <sys/time.h>


#define SHOW_FPS 1
#define BUFLEN 65506 //MAX UDP PACKET -1 (for flag)
#define ACCEPTANCE_KEY "start camera streaming!"


using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
    int camNum=0;
	unsigned char * buf;
	unsigned char * recvbuf;
    vector<unsigned char> vecbuf;
	size_t len;
	VideoCapture cap(camNum); // open the video camera no. 0
    UDPServer server = UDPServer(9090);
	while (1) {
		cout << "Waiting for client..."<<endl;
		recvbuf = server.receive(len);
		if (strcmp((const char*)recvbuf, ACCEPTANCE_KEY) != 0)
			continue;
		cout << "Received: "<< string((const char*)recvbuf);
		cout << "Client connected!"<<endl;
		while (!cap.isOpened()) { // if not success, exit program
        	cout << "Cannot open the video cam! Trying "<< ++camNum<< endl;
    		cap = VideoCapture(camNum);
    	}

   		cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
   		cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

   		double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
   		double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

    	cout << "Frame size : " << dWidth << " x " << dHeight << endl;
    	Mat curr_frame;
    	vector<uchar> framebuf;
    	int i=0;
    	while (1) {
        	//cout << "Trying to read frame from camera!" <<endl;
			//cout << "Is video capture opened? " << cap.isOpened()<<endl;
			cap.grab();
			cout <<"Grabbing frame "<< i<< endl;
			bool bSuccess = cap.read(curr_frame); // read a new frame from video

        	//cout << " Read a frame from camera! " << bSuccess << endl;

        	if (!bSuccess) { //if not success, break loop
            	cout << "Cannot read a frame from video stream" << endl;
				cap.release();
				cap = VideoCapture(++camNum);
        		continue;
			}

			if(curr_frame.empty()){
				cout << "Grabbed empty frame! Ignore!"<<endl;
                continue;
			}
			cout <<"Encoding..."<<endl;
			cout<< " Size: " << curr_frame.size() <<endl;
			imencode(".jpg", curr_frame, framebuf);
			cout <<"Encoded!"<<endl;
			cout <<"Sending..."<<endl;
			buf = &framebuf[0];
			server.send(buf, framebuf.size());
			cout <<"Sent!"<<endl;
			//imwrite("frame.jpg", curr_frame);
        	//waitKey(1);
			i++;
    	}
	}
    return 0;
}
