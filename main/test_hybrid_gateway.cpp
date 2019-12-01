#include <iostream>
#include <lora/lora.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <epollwifi/epolls.h>
#include <db/DataBase.h>
#include <sensor/sensor.h>
#include <pthread.h>
#include <pipe/pipe.h>
#include <nbiot/nbiot.h>

typedef struct Reply_{
	int ack_;
	int needReboot_;
}Reply;


void* becomeNBIotClient(void* arg){
	int client_socket = connectToNbIotServer(); 

	while(1){
		NB_Data nbData_;
		memset(&nbData_, 0, sizeof(NB_Data));
		nbData_.gatewayData_.rssi_ =10;
		nbData_.gatewayData_.status_ =0;
		nbData_.gatewayData_.rcount_ =10;
		nbData_.gatewayData_.needReboot_ =1 ;

		printf("------------------------Sensor------------------------\n");
		printf("[%s]\tReceived seq: %d\nValidity : %d  Huminity: %.1f  Temperature: %.1f\n",
		nbData_.sensorData_.mac_  , nbData_.sensorData_.count_  ,nbData_.sensorData_.sensor_.validity_,  nbData_.sensorData_.sensor_.humin_,
		nbData_.sensorData_.sensor_.temper_);
		printf("-----------------------Gateway-------------------------\n");
		printf("[%s]\trssi_: %d\nStatus: %d RebootCount: %d NeedReboot: %d\n",
		nbData_.gatewayData_.mac_, nbData_.gatewayData_.rssi_, nbData_.gatewayData_.status_,
		nbData_.gatewayData_.rcount_, nbData_.gatewayData_.needReboot_);
		
		printf("-------------------------------------------------------\n");
	
		write(client_socket, Pipe::getInstance().getBuf() ,sizeof(NB_Data) );

		sleep(10);
	}


}

int main(){
	DataBase::getInstance();
    Pipe::getInstance();
	char txbuf[255];
    char rxbuf[255];
    LoRa_ctl modem;
	pthread_t thread_t;
	
	if (pthread_create(&thread_t, NULL, becomeNBIotClient, NULL) <0){
		perror("thread create error:");
		exit(0);
	}
	pthread_detach(thread_t);

	sleep(1);

    
	while( 1 ) {
		sleep(7200);
	}

    printf("end\n");

    //LoRa_end(&modem);

	return 0;
}
