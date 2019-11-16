#include <nbiot/nbiot.h>

#include <arpa/inet.h>       
#include <netinet/in.h>      
#include <stdio.h>           
#include <stdlib.h>          
#include <sys/socket.h>      
#include <unistd.h>   


#define SERVER_PORT 3200

int connectToNbIotServer(){
	int sock_fd = -1;

	struct sockaddr_in6 server_addr;                                                   
	int ret;                                                                           
	char buf[1024]={'h','i','n','b','\0'};                                             


	/* Create socket for communication with server */                                  
	sock_fd = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);                              
	if (sock_fd == -1) {                                                               
			perror("nbiot.cpp socket()");                                                            
			return EXIT_FAILURE;                                                           
	}                                                                                  

	/* Connect to server running on localhost */                                       
	server_addr.sin6_family = AF_INET6;                                                
	inet_pton(AF_INET6, "64:ff9b::3a78:665a", &server_addr.sin6_addr);                 
	server_addr.sin6_port = htons(SERVER_PORT);                                        

		/* Try to do TCP handshake with server */                                          
	ret = connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));       
	if (ret == -1) {
		perror("nbiot.cpp connect()");
		close(sock_fd);
		return EXIT_FAILURE;
	}

	return sock_fd;

}
