#include "udp.hh"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>

/*
 * error - wrapper for perror
 */
void UDP::error(const char *msg) {
	printf("%s: %s\n", msg, hstrerror(h_errno));
	//perror(msg);
}


using namespace std;

// around 20fps when doing the copying to assemble the vector<uchar>.
// attempt to receive in a fixed size buffer instead, and avoid copying!
unsigned char* UDP::receive(size_t& len){
    unsigned hasmore = 1;
	bool first=true;
	unsigned offset = 0;
	vector<unsigned char> bufvec;
	/*
    * recvfrom: receive a UDP datagram
    */
	bzero(recvbuf, RECVBUFSIZE);
	while(hasmore == 1){
		recv_sz = recvfrom(sockfd, recvbuf + offset, RECVBUFBATCHSIZE, 0, (struct sockaddr *) &remoteaddr, &remotelen);
		if (recv_sz < 0){
			error("ERROR in recvfrom");
			return nullptr;
		}
		if (recv_sz == 0)
			printf("Received empty message! (huh?)\n");
        hasmore = (unsigned) recvbuf[offset + recv_sz-1];
		offset += recv_sz-1;
		if(first){
			/* 
        	* gethostbyaddr: determine who sent the datagram
        	*/
        	hostp = gethostbyaddr((const void *)&remoteaddr.sin_addr, sizeof(remoteaddr.sin_addr), AF_INET);
        	if (hostp == NULL){
                //printf("Could not get host info by address...\n");
               	//error("ERROR on gethostbyaddr");
        	}
        	hostaddrp = inet_ntoa(remoteaddr.sin_addr);
        	if (hostaddrp == NULL)
                error("ERROR on inet_ntoa\n");
        	if(hostp != NULL)
                printf("Received datagram from %s (%s)\n", hostp->h_name, hostaddrp);
		}
		first = false;
		//printf("Received %d bytes\n", recv_sz);
	}
	len = offset;
	return recvbuf;
}

void UDP::send(unsigned char* sendbuf, size_t len){
	int sendoffset=0;
    int currsendlen;
    unsigned char bufchunk [SENDBUFMAXSIZE];
    bool done=false;
    int send_sz;

    while (len > sendoffset){
    	// will include a flag at the end of the message to indicate whether there is a next package: 1 for yes, 0 for no
        if (len-sendoffset < SENDBUFMAXSIZE)
        	done = true;
        currsendlen = (done? len - sendoffset + 1: SENDBUFMAXSIZE);
        // leave space for the flag at the end!
        memcpy(bufchunk, sendbuf+sendoffset, currsendlen-1 );
        bufchunk[currsendlen-1] = (done? 0: 1);
       	//printf("Sending %d bytes [%d, %d]\n", currsendlen, sendoffset, sendoffset + currsendlen-2);
      	send_sz = sendto(sockfd, bufchunk, currsendlen, 0, (struct sockaddr *) &remoteaddr, remotelen);
		sendoffset+=currsendlen-1;
	}
    if (send_sz < 0)
    	error("ERROR in sendto");
 	if (sendoffset != len)
  		printf("Oops, was able to send %d bytes out of %d\n", send_sz, len);
}

