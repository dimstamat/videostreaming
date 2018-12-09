#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio.hpp"

#include "udpserver.hh"
#include "streamincludes.hh"

#include <stdio.h>
#include <iostream>

#include <sys/time.h>
#include <unistd.h>
#include <thread>
#include <atomic>

#include "measure_latencies.hh"

#define BUFLEN 65506 //MAX UDP PACKET -1 (for flag)

#define FRAME_SIZE_MAX 131072 // 128K frame buffer max
#define FRAMES_BUF_LEN 500 // up to 500 frames buffering


#define DEBUG 0

using namespace cv;
using namespace std;

UDPServer server = UDPServer(9090);

std::atomic<bool> terminated;

std::vector<uchar> frames [FRAMES_BUF_LEN];


void send_frames() {
	INIT_COUNTING
	unsigned index=0;
	unsigned char * buf;
	unsigned delay_us=90000;
	while(!terminated){
		if(index == FRAMES_BUF_LEN)
			index = 0;
		if(frames[index][FRAME_SIZE_MAX-1] == 1) { // if valid frame, send it
			buf = &frames[index][0];
			//START_COUNTING
			server.send(buf, frames[index].size());
			//STOP_COUNTING_PRINT("send buffer")
			frames[index][FRAME_SIZE_MAX-1] = 0; // make frame invalid so that to be reused by camera
			index++;
		}
		else { // wait a bit for some frames to appear (buffering)
			cout<<"Wait for camera to produce frames!\n";
			sleep(1);
			delay_us += 10000;
		}
		usleep(delay_us);
	}

}

void check_termination (){
    unsigned char * recvbuf;
    size_t len;
	cout <<"Waiting for termination..."<<endl;
    recvbuf = server.receive(len);
	if (strcmp((const char*)recvbuf, TERMINATE_KEY) == 0)
		terminated = true;
	cout << "Received: "<< string((const char*)recvbuf) <<endl;
}

void send_ph(){
	FILE* fp;
	char line[128];
	char * token;
	if((fp = fopen(PH_FILE_PATH, "r")) == NULL){
		cout <<"Error opening file "<<PH_FILE_PATH<<endl;
		return;
	}
	while(fgets(line, 128, fp) != nullptr);
	token = strtok(line, " ");
	unsigned i=1;
	float ph=0;
	while(token != nullptr){
		token = strtok(NULL, " ");
		i++;
		if(i==3){
			ph = atof(token);
			cout<<ph<<endl;
			server.send((unsigned char*)token, strlen(token));
		}
	}
}

int main(int argc, char* argv[])
{
	int camNum=0;
	unsigned char * buf;
	unsigned char * recvbuf;
    vector<unsigned char> vecbuf;
	size_t len;
	unsigned frame_index=0;

	INIT_COUNTING

	// initialize frames buffer with vectors of size FRAME_SIZE_MAX and contents 0
	for(unsigned i=0; i<FRAMES_BUF_LEN; i++)
		frames[i] = std::vector<uchar>(FRAME_SIZE_MAX, 0);

	VideoCapture cap;//(camNum); // open the video camera no. 0
	while (1) {
		cap = VideoCapture(camNum);
        cout << "Waiting for client..."<<endl;
		terminated = false;
		recvbuf = server.receive(len);
		cout<< "Received something!"<<endl;
		if(strcmp((const char*) recvbuf, PH_KEY) ==0){
			send_ph();
			continue;
		}
		if (strcmp((const char*)recvbuf, ACCEPTANCE_KEY) != 0)
			continue;
		cout << "Received: "<< string((const char*)recvbuf) <<endl;
		if (!cap.isOpened()) {
			camNum = (camNum == 0? 1: 0);
			cap = VideoCapture(camNum);
		}
		std::thread check_termination_t = std::thread(check_termination);
		std::thread send_frames_t;

   		cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
   		cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

   		double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
   		double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

    	cout << "Frame size : " << dWidth << " x " << dHeight << endl;
    	Mat curr_frame;
    	vector<uchar> framebuf;
    	int i=0;
		bool send_thread_started = false;
    	while (1) {
			if(frame_index == FRAMES_BUF_LEN)
				frame_index = 0;
			//buf= (unsigned char*)malloc (7* sizeof(unsigned char));
			//bzero(buf, 7* sizeof(unsigned char));
			//buf[0]='H'; buf[1] = 'e'; buf[2] = 'l'; buf[3] = 'l'; buf[4] = 'o'; buf[5] = '!';
			//server.send(buf, 6);
			//break;
        	//cout << "Trying to read frame from camera!" <<endl;
		    if(terminated)
				break;
		    //START_COUNTING
			bool bSuccess = cap.read(curr_frame); // read a new frame from video
			//STOP_COUNTING_PRINT("grab frame")
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
		    //cout<< " Size: " << curr_frame.size() <<endl;
			//imencode(".jpg", curr_frame, framebuf);
			//START_COUNTING
		    imencode(".jpg", curr_frame, frames[frame_index]);
			//STOP_COUNTING_PRINT("jpeg encode")
			if (frames[frame_index][FRAME_SIZE_MAX-1] == 1) { // we reached the point where all frames are full and sender hasn't sent anything yet!
				cout<<"Sender hasn't sent anything yet, wait...\n";
				sleep(5);
				continue;
			}
			imencode(".jpg", curr_frame, frames[frame_index]);
			// mark frame as okay to read! (Last element will be 1)
			frames[frame_index][FRAME_SIZE_MAX-1] = 1;
			frame_index++;
			//buf = &framebuf[0];
		    #if DEBUG
			    cout <<"Sending "<<framebuf.size()<<endl;
		    #endif
		    //server.send(buf, framebuf.size());
		    i++;
			// start the frame sending thread
			if(!send_thread_started && frame_index == FRAMES_BUF_LEN/7){
				send_frames_t = std::thread(send_frames);
				send_thread_started = true;
			}
    	}
		cap.release();
        
		check_termination_t.join();
		send_frames_t.join();
		printf("Okay, continue with the next client\n");
    }
    return 0;
}
