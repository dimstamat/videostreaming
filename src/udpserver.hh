#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "udp.hh"


class UDPServer : public UDP{
  struct sockaddr_in serveraddr; /* server's addr */
  int optval; /* flag value for setsockopt */

public:  
  UDPServer(int port);
};
