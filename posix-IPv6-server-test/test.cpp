#include <string>
#include <cerrno>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>

struct fatal_exception {
	fatal_exception(const std::string& what_) : priv_what(what_) {}
	const std::string& what() const { return priv_what; }
private:
	std::string priv_what;
};


static int create_server(int port, bool ipv6, bool udp) {
	// Create a socket
	int fd = socket(ipv6?AF_INET6:AF_INET, udp?SOCK_DGRAM:SOCK_STREAM, IPPROTO_TCP);
	if (fd==-1)
		throw fatal_exception("socket(): "+ std::string(strerror(errno)));
	try {
		// Make it non blocking
		long flags = fcntl(fd,F_GETFL,0);
		if(flags==-1)
			throw fatal_exception("fcntl(): "+ std::string(strerror(errno)));
		flags |= O_NONBLOCK;
		if(fcntl(fd,F_SETFL,flags)==-1)
			throw fatal_exception("fcntl(): "+ std::string(strerror(errno)));
		// Make it reusable
		int value=1;
		setsockopt(fd,IPPROTO_TCP,TCP_NODELAY,&value,sizeof(value));
		setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&value,sizeof(value));
#if defined(SO_NOSIGPIPE)
		setsockopt(fd,SOL_SOCKET,SO_NOSIGPIPE,&value,sizeof(value));
#endif
	
		sockaddr_in6 sad;
		size_t sad_size = sizeof(sockaddr_in6)-sizeof(uint32_t);
		sad.sin6_family = AF_INET6;
		sad.sin6_port = htons(port);
		sad.sin6_flowinfo = 0;
		sad.sin6_addr = in6addr_any;
		if (bind(fd, (struct sockaddr*)&sad, sad_size)==-1)
			throw fatal_exception("bind(): "+ std::string(strerror(errno)));
		if (listen(fd, 500)==-1)
			throw fatal_exception("listen(): "+ std::string(strerror(errno)));
	}
	catch(...) {
		close(fd);
		throw;
	}
	return fd;
}

int main(int argc, char** argv) {
	try {
		int fd = create_server(1414,true,false);
		while(true) {
			struct sockaddr_in6 peer_addr;
			int peer_len = sizeof(peer_addr);
			int sw = accept(fd, (struct sockaddr*) &peer_addr, (socklen_t*) &peer_len);
			if (sw==-1) {
				usleep(0);
				continue;
			}
			char dummy[1024];
			inet_ntop(AF_INET6, &(peer_addr.sin6_addr), dummy, 1024);
			write(sw, "Hello!\r\n", 9);
			shutdown(sw,2);
			close(sw);
		}
	}
	catch(...) {
		std::cerr << "Error" << std::endl;
	}
}

