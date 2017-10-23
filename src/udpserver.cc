/* 
 * udpserver.cc 
 * usage: udpserver <port>
 */
#include "udpserver.hh"

#include <stdio.h>
#include <unistd.h>

UDPServer::UDPServer(int port){
	portno = port;
	/* 
   	* socket: create the parent socket 
   	*/
  	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  	if (sockfd < 0)
    		error("ERROR opening socket");

  	/* setsockopt: Handy debugging trick that lets 
   	* us rerun the server immediately after we kill it; 
   	* otherwise we have to wait about 20 secs. 
   	* Eliminates "ERROR on binding: Address already in use" error. 
   	*/
  	optval = 1;
  	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));

  	/*
   	* build the server's Internet address
   	*/
  	bzero((char *) &serveraddr, sizeof(serveraddr));
  	serveraddr.sin_family = AF_INET;
  	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  	serveraddr.sin_port = htons((unsigned short)portno);
	/* 
   	* bind: associate the parent socket with a port 
   	*/
  	if (bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
    		error("ERROR on binding");
  	remotelen = sizeof(remoteaddr);
	printf("Created server ok!\n");
}

