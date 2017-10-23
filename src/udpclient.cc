/* 
 * udpclient.c - A simple UDP client
 */
#include "udpclient.hh"
#include <stdio.h>
#include <stdlib.h>


UDPClient::UDPClient(const char* hostname, int portno){
	/* socket: create the socket */
 	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    	error("ERROR opening socket");

  	/* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }

    /* build the server's Internet address */
    bzero((char *) &remoteaddr, sizeof(remoteaddr));
    remoteaddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&remoteaddr.sin_addr.s_addr, server->h_length);
    remoteaddr.sin_port = htons(portno);
	remotelen = sizeof(remoteaddr);	
	printf("All good!\n");
}

