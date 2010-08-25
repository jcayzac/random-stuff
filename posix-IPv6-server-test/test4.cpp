#include <iostream>
#include <string>
#include <cerrno>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>


int main() {
	int fd = socket(AF_INET6,SOCK_STREAM,IPPROTO_TCP);
	fcntl(fd,F_SETFL,O_NONBLOCK|fcntl(fd,F_GETFL,0));

	sockaddr_in6 sad;
	size_t sad_size = sizeof(sockaddr_in6)-sizeof(uint32_t);
	sad.sin6_family = AF_INET6;
	sad.sin6_port = htons(1414);
	sad.sin6_flowinfo = 0;
	sad.sin6_addr = in6addr_any;
	bind(fd, (struct sockaddr*)&sad, sad_size);
	listen(fd, 500);
	while(true) {
		std::cerr << "Accepting connections..." << std::endl;
		struct sockaddr_in6 peer_addr;
		int peer_len = sizeof(peer_addr);
		int sw = accept(fd, (struct sockaddr*) &peer_addr, (socklen_t*) &peer_len);
		if (sw==-1) {
			usleep(0);
			continue;
		}
		char dummy[1024];
		inet_ntop(AF_INET6, &(peer_addr.sin6_addr), dummy, 1024);
		std::cerr << "Connexion from " << dummy << std::endl;
		if (!fork()) continue;
		for (int i=0; i<10; ++i) {
			sprintf(dummy,"%i\r\n",i);
			write(sw,dummy,strlen(dummy));
			sleep(1);
		}
		shutdown(sw,2);
		close(sw);
		exit(0);
	}
	return 0;
}

