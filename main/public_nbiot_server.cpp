#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdbool.h>
#define  BUFF_SIZE   1024
#define  PORT		 3200

#include <nbiot/nbiot.h>




int   main( void)
{
		int   server_socket;
		int   client_socket;
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

		while( 1)
		{
				client_addr_size  = sizeof( client_addr);
				client_socket     = accept( server_socket, (struct sockaddr*)&client_addr, (socklen_t*)&client_addr_size);
				printf("client... accepted...!\n");
				if ( -1 == client_socket)
				{
						printf( "클라이언트 연결 수락 실패\n");
						exit( 1);
				}

			while(1){
					NB_Data  nbData_;
					memset(&nbData_, 0, sizeof(NB_Data));
					read ( client_socket, &nbData_, sizeof(NB_Data));
					 //printf("Mac:%s \nReceived count: %d  validity_ :%d   humiditiy: %.1f  Temperatur: %.1f\n",
					 // data.mac_,data.count_, data.sensor_.validity_, data.sensor_.humin_, data.sensor_.temper_);

					  
					 printf("------------------------Sensor------------------------\n");
					 printf("[%s]\tReceived seq: %d\nValidity : %d  Huminity: %.1f  Temperature: %.1f\n",
									 nbData_.sensorData_.mac_,nbData_.sensorData_.sensor_.validity_,nbData_.sensorData_.sensor_.humin_,
									 nbData_.sensorData_.sensor_.temper_);
					 printf("-----------------------Gateway-------------------------\n");
					 printf("[%s]\trssi_: %d\nStatus: %d RebootCount: %d NeedReboot: %d",
									 nbData_.gatewayData_.mac_, nbData_.gatewayData_.rssi_, nbData_.gatewayData_.status_,
									 nbData_.gatewayData_.rcount_, nbData_.gatewayData_.needReboot_);

					printf("\n");
																	  

					//printf( "receive: %s\n", buff_rcv);
				}
				sprintf( buff_snd, "%d : %s", strlen( buff_rcv), buff_rcv);
				//write( client_socket, buff_snd, strlen( buff_snd)+1);          // +1: NULL까지 포함해서 전송
				//close( client_socket);
		}
}
