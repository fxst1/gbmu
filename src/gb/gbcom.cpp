#include <gameboy.hpp>
#include <globals.hpp>
#include <errno.h>

extern "C" int errno;

GameboyServer::GameboyServer(const size_t ncom, unsigned short port)
{
	std::cout << "Server Startup : " << std::to_string(ncom) << " slots" << std::endl;
	this->_port = port;
	this->_host = (char*)"localhost";
	this->_started = false;
	this->_used = false;
	this->_socket = ::socket(AF_INET, SOCK_STREAM, 0);
	this->_clients = std::map<SOCKET, SOCKADDR_IN>();
	this->_ncom = 0;
	this->_listening = false;
	if (this->_socket == INVALID_SOCKET)
	{
		std::cerr << "Cannot create socket:" << strerror(errno) << std::endl;
	}
	else if (ncom > 0)
	{
		fcntl(this->_socket, F_SETFL, O_NONBLOCK);
		this->_started = true;
		this->_ncom = ncom;
		this->bind(port);
	}
}

GameboyServer::~GameboyServer(void)
{
	if (this->_socket != INVALID_SOCKET)
	{
		::shutdown(this->_socket, 2);
		::closesocket(this->_socket);
		this->_socket = -1;
		for (auto it = this->_clients.begin(); it != this->_clients.end(); it++)
		{
			::shutdown(it->first, 2);
			::closesocket(it->first);
		}
	}
}

void			GameboyServer::stop(void)
{
	this->_used = false;
	if (this->_socket != INVALID_SOCKET)
	{
		for (auto it = this->_clients.begin(); it != this->_clients.end(); it++)
		{
			::shutdown(it->first, 2);
			::closesocket(it->first);
		}
		this->_clients.clear();
	}
}

bool			GameboyServer::bind(unsigned short port)
{
	::SOCKADDR_IN		sin;

	bzero(&sin, sizeof(sin));

	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(port);
	sin.sin_family = AF_INET;

	this->_port = port;
	if (::bind(this->_socket, (SOCKADDR*)&sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		std::cerr << "Cannot bind: " << strerror(errno) << std::endl;
		return (false);
	}
	else if (::listen(this->_socket, this->_ncom) == SOCKET_ERROR)
	{
		std::cerr << "Cannot listen socket: " << strerror(errno) << std::endl;
		return (false);
	}
	return (true);
}

bool			GameboyServer::accept(void)
{
	::SOCKADDR_IN		csin;
	::SOCKET			csock;
	size_t				size;

	bzero(&csin, sizeof(csin));
	if (this->_clients.size() > this->_ncom)
		return (false);

	size = sizeof(csin);
	csock = ::accept(this->_socket, (SOCKADDR*)&csin, (unsigned int*)&size);
	if (csock == SOCKET_ERROR)
	{
//		std::cerr << "Cannot accept: " << strerror(errno) << std::endl;
		return (false);
	}
	else
		std::cout << "Client found" << std::endl;
	this->_clients[csock] = csin;
	return (true);
}

void			GameboyServer::relay(void)
{
	unsigned char	bytes[32] = {0};

	for (auto it = this->_clients.begin();
			it != this->_clients.end(); it++)
	{
		bzero(bytes, sizeof(bytes));
		while (::read(it->first, bytes, 32) > 0)
		{
			this->send(bytes, it->first);
		}
	}
}

void			GameboyServer::send(unsigned char* bytes, ::SOCKET sender)
{
	for (auto it = this->_clients.begin();
			it != this->_clients.end(); it++)
	{
		if (sender == it->first)
			continue ;
		if (::send(it->first, bytes, 32, 0) < 0)
			std::cerr << "Send error:" << strerror(errno) << std::endl;
	}
}

bool			GameboyServer::allow(SOCKET id)
{
	if (::send(id, "HLLO", 4, 0) < 0)
		return (false);
	return (true);
}

bool			GameboyServer::deny(SOCKET id)
{
	if (::send(id, "ERRO", 4, 0) < 0)
		return (false);
	return (true);
}

void			GameboyServer::kick(SOCKET id)
{
	this->_clients.erase(id);
	::shutdown(id, 2);
	::close(id);
}

GameboyClient::GameboyClient(void)
{
	this->_servers = std::vector<t_servercom*>();
	this->_host = NULL;
	this->_port = 11001;
	this->_username = (char*)"Undefined username";
	this->_connected = false;
	this->_socket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket == INVALID_SOCKET)
	{
		std::cerr << "Cannot create socket" << std::endl;
	}
}

GameboyClient::~GameboyClient(void)
{
	for (auto it = this->_servers.begin(); it != this->_servers.end(); it++)
		free(*it);

	if (this->_socket != INVALID_SOCKET)
	{
		::shutdown(this->_socket, 2);
		::closesocket(this->_socket);
	}
}

static unsigned short	__checksum(void *b, int len)
{
    unsigned short *buf = (unsigned short*)b;
    unsigned int sum=0;
    unsigned short result;

    for ( sum = 0; len > 1; len -= 2 )
        sum += *buf++;
    if ( len == 1 )
        sum += *(unsigned char*)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

bool			GameboyClient::ping(const char* hostname, unsigned short port)
{
	const int val=255;
	int i, sd;
	struct packet pckt;
	struct sockaddr_in r_addr;
	int loop;
	struct hostent *hname;
	struct sockaddr_in addr_ping,*addr;

	pid_t				pid = getpid();
	struct protoent*	proto = getprotobyname("ICMP");

	hname = gethostbyname(hostname);
	bzero(&addr_ping, sizeof(addr_ping));
	addr_ping.sin_family = hname->h_addrtype;
	addr_ping.sin_port = port;
	addr_ping.sin_addr.s_addr = *(long*)hname->h_addr;

	addr = &addr_ping;

	sd = socket(PF_INET, SOCK_RAW, proto->p_proto);
	if ( sd < 0 )
	{
		std::cerr << "Ping: Cannot create socket: " << strerror(errno) << std::endl;
		return (1);
	}
	if ( setsockopt(sd, SOL_IP, IP_TTL, &val, sizeof(val)) != 0)
	{
		std::cerr << "Ping: Cannot Set TTL option" << strerror(errno) << std::endl;
		return (1);
	}
	if ( fcntl(sd, F_SETFL, O_NONBLOCK) != 0 )
	{
		std::cerr << "Ping: Cannot request nonblocking I/O: " << strerror(errno) << std::endl;
		return (1);
	}
	for (loop=0;loop < 10; loop++)
	{
		int len = sizeof(r_addr);

		if ( recvfrom(sd, &pckt, sizeof(pckt), 0, (struct sockaddr*)&r_addr, (socklen_t*)&len) > 0 )
		{
			return (0);
		}

		bzero(&pckt, sizeof(pckt));
		pckt.hdr.type = ICMP_ECHO;
		pckt.hdr.un.echo.id = pid;
		for ( i = 0; i < (int)sizeof(pckt.msg)-1; i++ )
			pckt.msg[i] = i+'0';
		pckt.msg[i] = 0;
		pckt.hdr.un.echo.sequence = loop;
		pckt.hdr.checksum = __checksum(&pckt, sizeof(pckt));
		if ( sendto(sd, &pckt, sizeof(pckt), 0, (struct sockaddr*)addr, sizeof(*addr)) <= 0 )
			std::cerr << "Ping: Cannot send packet: " << strerror(errno) << std::endl;

		usleep(300000);
	}

	return (1);
}

bool			GameboyClient::disconnect(void)
{
	this->_connected = false;
	::shutdown(this->_socket, 2);
	::closesocket(this->_socket);
	this->_socket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket == INVALID_SOCKET)
	{
		std::cerr << "Cannot recreate socket" << std::endl;
		return (false);
	}
	this->_connected = true;
	return (true);
}

bool			GameboyClient::connect(const char* hostname, unsigned short port)
{
	struct hostent	*host = NULL;
	::SOCKADDR_IN	sin;

	bzero(&sin, sizeof(sin));
	host = ::gethostbyname(hostname);
	if (host == NULL)
	{
		std::cerr << "Unknow host " << hostname << std::endl;
		return (false);
	}

	sin.sin_addr = *(IN_ADDR*)host->h_addr;
	sin.sin_port = htons(port);
	sin.sin_family = AF_INET;

	if (::connect(this->_socket, (SOCKADDR*)&sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		this->_connected = false;
		std::cerr << "Cannot connect to host " << hostname << std::endl;
		return (false);
	}
	this->_host = (char*)hostname;
	this->_connected = true;
	return (true);
}

bool		GameboyClient::sync(const unsigned int freq)
{
	unsigned char	data[32] = "SYNC";

	memcpy(data + 4, &freq, sizeof(freq));
	if (::send(this->_socket, data, 8, 0) < 0)
	{
		std::cerr << "Cannot send datas to host" << std::endl;
		return (false);
	}
	return (true);
}

bool		GameboyClient::send(const unsigned char* data, size_t len)
{
	if (::send(this->_socket, data, len, 0) < 0)
	{
		std::cerr << "Cannot send datas to host" << std::endl;
		return (false);
	}
	return (true);
}

bool		GameboyClient::recv(unsigned char* data, size_t *len)
{
	unsigned char	bytes[32] = {0};

	if (::recv(this->_socket, bytes, 32, 0) < 0)
	{
		std::cerr << "Cannot receive datas from host" << std::endl;
		return (false);
	}

	if (!strncmp((const char*)data, "SYNC", 4))
	{
		gbuser->getCpu()->setClock( *((unsigned int*)data + 4) );
		*data = 0;
		*len = 0;
	}
	else if (!strncmp((const char*)data, "SEND", 4))
	{
		*data = ( (unsigned char)*(data + 4) );
		*len = 1;
	}
	else if (!strncmp((const char*)data, "HLLO", 4))
	{
		this->_connected = true;
		*data = 1;
		*len = 1;
	}
	else
	{
		this->_connected = false;
		*data = 0;
		*len = 0;		
		return (false);
	}
	return (true);
}
