#include "udpclient.hh"
#include <stdio.h>
#include <sys/time.h>
#include <iostream>

using namespace std;

//#define BUFLEN 1048576
#define BUFLEN 65506 // MAX UDP PACKET -1 (for flag)

int main(int argc, char** argv){
	char buf [BUFLEN];
	int i=0;
	size_t len;
	UDPClient client = UDPClient("192.168.1.150", 9090);
	memset(buf, 0, BUFLEN);
	for(i=0; i<BUFLEN; i++){
		buf[i] = (char) (i+1)%300;
	}
	client.send((unsigned char*)buf, BUFLEN);
	client.receive(len);
	cout << "Received total: " << len<<endl;
}
