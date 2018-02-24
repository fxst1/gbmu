#ifdef WIN32 /* si vous êtes sous Windows */

#define		WIN_UNIX_SOCK	1
#include <winsock2.h> 

#elif defined (linux) /* si vous êtes sous Linux */

#define		WIN_UNIX_SOCK	1
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <resolv.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#else /* sinon vous êtes sur une plateforme non supportée */

#define		WIN_UNIX_SOCK	0
//#error not defined for this platform
#endif

#define PACKETSIZE  64
struct packet
{
	struct icmphdr hdr;
	char msg[PACKETSIZE-sizeof(struct icmphdr)];
};