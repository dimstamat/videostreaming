#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "udp.hh"

class UDPClient : public UDP{
    	struct hostent *server;

public:
	UDPClient(const char* hostname, int portno);
};
