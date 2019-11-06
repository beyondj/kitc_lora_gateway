#include "epolls.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>

#include <sys/epoll.h>


int createSocket(){
	int server_socket;
	
	struct sockaddr_in server_addr;
	server_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (-1 == server_socket){
			printf( "server socket creation failed \n");
			exit( 1);
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family     = AF_INET;
	server_addr.sin_port       = htons(4000);
	server_addr.sin_addr.s_addr= htonl(INADDR_ANY);

	if (-1 == bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr))){
		printf( "bind() error...\n");
		exit(1);
	}

	if (listen(server_socket, 5) < 0) {
		printf("listen() error\n");
		exit(1);
	}

	return  server_socket;
}



int registerToEpoll(int server_socket){

	int epoll_fd = epoll_create(128);
	if (epoll_fd<0) {
			printf("epoll_create() error\n");
			close(server_socket);
			exit(1);
	}

	struct epoll_event events;
	events.events = EPOLLIN;
	events.data.fd = server_socket;

	if( epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_socket, &events) < 0) {
		printf("epoll_ctl() error\n");
		close(epoll_fd);
		close(server_socket); 
		exit(1);
	}

	return epoll_fd;
}





void startEpollWait(int epoll_fd, int server_socket){
	int MAX_EVENTS = 128;
	struct epoll_event epoll_events[MAX_EVENTS];
	int event_count;
	int i = 0;
	while(1){
		event_count = epoll_wait(epoll_fd, epoll_events, MAX_EVENTS, -1);
		if (event_count < 0) {
				printf("epoll_wait() error\n");
				exit(1);
		}

		for (i = 0 ; i < event_count ; i++){
			if (epoll_events[i].data.fd == server_socket){
				int client_fd;
				int client_len;
				struct sockaddr_in client_addr;
				client_len = sizeof(client_addr);
				client_fd = accept(server_socket,(struct sockaddr*)&client_addr, (socklen_t*)&client_len);
				if (client_fd < 0){
					printf("accept is error\n");
					exit(1);
				}

				struct epoll_event events;
				events.events = EPOLLIN;
								events.data.fd = client_fd;

				if( epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &events) < 0){
					printf("epoll_ctl error..\n");
					close(client_fd);
					continue;
				}

			}
			else{
				int client_fd = epoll_events[i].data.fd;
				char data[1024];
				memset(data,0, sizeof(char)*1024);
				int str_len = read(client_fd, data, sizeof(data));
				if (str_len == 0) {
						printf("Client Disconnect\n");
						close(client_fd);
						epoll_ctl(epoll_fd, EPOLL_CTL_DEL,client_fd, NULL);
				}
				else{
						printf("Receive data...%s\n",data);
				}
			}
		} //for

	}

}
