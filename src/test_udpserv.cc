#include "udpserver.hh"
#include <stdio.h>
#include <unistd.h>

//#define BUFLEN 1048576
//#define BUFLEN 65506 // MAX UDP PACKET -1 (for flag)
#define BUFLEN 128000

int main(int argc, char** argv){
	char buf [BUFLEN];
	int i=0;
	size_t len;
	UDPServer server = UDPServer(9090);
	server.receive(len);
	memset(buf, 0, BUFLEN);
	for(i=0; i<BUFLEN; i++){
		buf[i] = (char) (i+1)%300;
	}
	//sprintf(buf, "elaa");
	server.send((unsigned char*)buf, BUFLEN);
	//usleep(10000000);
}
