#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>


#define RECVBUFBATCHSIZE 65507
#define RECVBUFSIZE 1048576 //1MB buffer
#define SENDBUFMAXSIZE 65507
//#define SENDBUFMAXSIZE 100

using namespace std;

class UDP {
protected:
  int sockfd; /* socket */
  int portno; /* port to listen on */
  struct hostent *hostp; /* client host info */
  char *hostaddrp; /* dotted decimal host addr string */
  unsigned int remotelen; /* byte size of remote's address */
  struct sockaddr_in remoteaddr; /* remote addr */
  unsigned char recvbuf[RECVBUFSIZE]; /* receive buf */
  int recv_sz; /* receive byte size */
  int current_fl; /* flags for recvfrom() */
  void error(const char* msg);
public:
  // receives and stores in recvbuf
  // len is the total size received excluding the flags (payload)
  unsigned char* receive(size_t& len);
  // sends the given buffer of size len excluding the flags (payload)
  void send(unsigned char* sendbuf, size_t len);
};
