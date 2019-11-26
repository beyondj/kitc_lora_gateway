#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <stdbool.h>
#define  BUFF_SIZE   1024
#define  PORT		 3200

#include <nbiot/nbiot.h>




int   main( void)
{
	int   server_socket;
	int   client_addr_size;

	struct sockaddr_in   server_addr;
	struct sockaddr_in   client_addr;

	char   buff_rcv[BUFF_SIZE+5];
	char   buff_snd[BUFF_SIZE+5];



	server_socket  = socket( PF_INET, SOCK_STREAM, 0);
	if( -1 == server_socket)
	{
		printf( "server socket 생성 실패\n");
		exit( 1);
	}

	memset( &server_addr, 0, sizeof( server_addr));
	server_addr.sin_family     = AF_INET;
	server_addr.sin_port       = htons(PORT);
	server_addr.sin_addr.s_addr= htonl( INADDR_ANY);

	int opt = 1;
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	if( -1 == bind( server_socket, (struct sockaddr*)&server_addr, sizeof( server_addr) ) )
	{
		printf( "bind() 실행 에러\n");
		exit( 1);
	}

	if( -1 == listen(server_socket, 5))
	{
		printf( "listen() 실행 실패\n");
		exit( 1);
	}

	int epoll_fd = epoll_create(128);
	if(epoll_fd <0) {
		printf("epoll_create() error\n");
		close(server_socket);
		exit(1);
	}

	struct epoll_event events;
	memset(&events, 0, sizeof(epoll_event));
	events.events = EPOLLIN;
	events.data.fd = server_socket;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_socket, &events) <0 ){
		printf("epoll_ctl() error\n");
		close(epoll_fd);
		close(server_socket);
		exit(1);
	}

	int MAX_EVENTS = 128;
	struct epoll_event epoll_events[MAX_EVENTS];
	int event_count;

	while(1){
		event_count = epoll_wait(epoll_fd, epoll_events, MAX_EVENTS, -1);
		if (event_count < 0){
			printf("epoll_wait() error\n");
			exit(1);
		}

		for(int i =0;i < event_count; i++){
			if(epoll_events[i].data.fd == server_socket){
				int client_fd;
				int client_len;
				struct sockaddr_in client_addr;
				client_len = sizeof(client_addr);
				client_fd = accept(server_socket,(struct sockaddr*)&client_addr, (socklen_t*)&client_len);
				printf("accept client...\n");
				if(client_fd <0){
					printf("accept is error\n");
					exit(1);
				}

				struct epoll_event events;
				memset(&events, 0, sizeof(epoll_event));
				events.events=EPOLLIN;
				events.data.fd =client_fd;

				if( epoll_ctl(epoll_fd, EPOLL_CTL_ADD,client_fd, &events) <0 ){
					printf("epoll_ctl_error ...\n");
					close(client_fd);
					continue;
				}
			}

			else{
				int client_fd = epoll_events[i].data.fd;
				NB_Data nbData_;
				memset(&nbData_, 0, sizeof(NB_Data));
				int str_len = read ( client_fd, &nbData_, sizeof(NB_Data));

				if(str_len == 0){
					printf("Client disconnected...\n");
					close(client_fd);
				}
				else{		  
					  
					printf("------------------------Sensor------------------------\n");
					printf("[%s]\tReceived seq: %d\nValidity : %d  Huminity: %.1f  Temperature: %.1f\n",
									 nbData_.sensorData_.mac_, nbData_.sensorData_.count_,  nbData_.sensorData_.sensor_.validity_,nbData_.sensorData_.sensor_.humin_,
									 nbData_.sensorData_.sensor_.temper_);
					printf("-----------------------Gateway-------------------------\n");
					printf("[%s]\trssi_: %d\nStatus: %d RebootCount: %d NeedReboot: %d\n",
									 nbData_.gatewayData_.mac_, nbData_.gatewayData_.rssi_, nbData_.gatewayData_.status_,
									 nbData_.gatewayData_.rcount_, nbData_.gatewayData_.needReboot_);

					printf("-------------------------------------------------------\n");
					printf("\n\n\n");
				
				}

			}
		}
	
	}


	return 0;
}
