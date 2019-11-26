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

void tx_f(txData *tx){
    LoRa_ctl *modem = (LoRa_ctl *)(tx->userPtr);
    //printf("tx done;\t");

	Reply* reply =(Reply*) tx->buf;
	printf("#######Sending to Sensor ack: %d  NeedReboot: %d\n\n\n", reply->ack_, reply->needReboot_);
   // printf("sent reply ack = %d, needReboot = %d\n\n", reply->ack_, reply->needReboot_);//Data we've sent

	LoRa_receive(modem);
//    LoRa_sleep(modem);
}



int errorCount = 0;
void _processDBbyStatus(const std::string& mac){
	DataBase& db = DataBase::getInstance();
	Status status = db.query(mac);

	if (status == Status::Newby){
		std::cout<<"New sensor is detected. Insert sensor information in DB"<<std::endl;
		db.saveClient(mac);
	}
	else if(status == Status::Bad){
		std::cout<<"Abnormally behaviored sensor is rebooted"<<std::endl;
		db.activate(mac);
	}
	else{} //status == Status::Good
}

int _processReplybyValidity(bool validity, const std::string& mac){
	DataBase& db = DataBase::getInstance();
	if(validity == 0){
		db.increaseErrorCount(mac);
			
		if (db.getErrorCount(mac) > 5){
			db.reboot(mac);
			return 1;
		}
	}

	else{
		db.resetErrorCount(mac);
	}

	return 0;
}

void _setStatusandRcount(const std::string& mac, GatewayData& gatewayData){
	DataBase& db = DataBase::getInstance();
	db.setInfo(mac,gatewayData);
}

void rx_f(rxData *rx){
    LoRa_ctl *modem = (LoRa_ctl *)(rx->userPtr);
    LoRa_stop_receive(modem);//manually stoping RxCont mode
	Data data = *(Data*)(rx->buf);

	Reply reply;
	memset(&reply, 0, sizeof(Reply));


	int RSSI = rx->RSSI;
	//LoRa_end(modem);
	//lora communication error... do nothing..
	if (rx->CRC){
		printf("[ERROR_CRC_BAD_NETWORK_COND)] Ignore invalid data & ack may not reach properly\n");
		reply.ack_ = -1;
	}

	else {
		NB_Data nbData_;
		memset(&nbData_ , 0, sizeof(NB_Data));

		nbData_.sensorData_ = *(Data*)(rx->buf);
	

		std::string mac{nbData_.sensorData_.mac_};

		//process db related things and prepare for the reply
		_processDBbyStatus(mac);
		reply.needReboot_ = _processReplybyValidity(nbData_.sensorData_.sensor_.validity_, mac);
		reply.ack_ = nbData_.sensorData_.count_;
		
		//fill the mac_, status_, rcount_
		_setStatusandRcount(mac, nbData_.gatewayData_);

		//fill the others
		nbData_.gatewayData_.rssi_ = RSSI;
		nbData_.gatewayData_.needReboot_ = reply.needReboot_;





		//reply.ack_ = data.count_;
	

		//Data data = *(Data*)(rx->buf);
		//std::string mac{data.mac_};

		Pipe::getInstance().write(nbData_);

	    //_processDBbyStatus(mac);
		//reply.needReboot_ = _processReplybyValidity(data.sensor_.validity_, mac);

		//printf("[%s]\tReceived seq: %d\nValidity : %d  Huminity: %.1f  Temperature: %.1f\n",
		//data.mac_,data.count_, data.sensor_.validity_, data.sensor_.humin_, data.sensor_.temper_);
	
		printf("------------------------Sensor------------------------\n");
		printf("[%s]\tReceived seq: %d\nValidity : %d  Huminity: %.1f  Temperature: %.1f\n",
		nbData_.sensorData_.mac_  , nbData_.sensorData_.count_  ,nbData_.sensorData_.sensor_.validity_,  nbData_.sensorData_.sensor_.humin_,
		nbData_.sensorData_.sensor_.temper_);
		printf("-----------------------Gateway-------------------------\n");
		printf("[%s]\trssi_: %d\nStatus: %d RebootCount: %d NeedReboot: %d\n",
		nbData_.gatewayData_.mac_, nbData_.gatewayData_.rssi_, nbData_.gatewayData_.status_,
		nbData_.gatewayData_.rcount_, nbData_.gatewayData_.needReboot_);
		
		printf("-------------------------------------------------------\n");
	
	
	}
	

	
	
    //printf("SNR: %f\n\n", rx->SNR);
   
   memcpy(modem->tx.data.buf,&reply, sizeof(Reply));
   modem->tx.data.size = sizeof(Reply);

	LoRa_end(modem);
	LoRa_begin(modem);
    LoRa_send(modem);
}


/*void * wifiThread(void* arg){
	int server_socket = createSocket();
	int epoll_fd = registerToEpoll(server_socket);
	startEpollWait(epoll_fd, server_socket);
}*/

void* becomeNBIotClient(void* arg){
	int client_socket = connectToNbIotServer(); 
	
	while(Pipe::getInstance().read()){
		write(client_socket, Pipe::getInstance().getBuf() ,sizeof(NB_Data) );
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


	lora_initiate(modem,rx_f,rxbuf,tx_f,txbuf, Bandwidth::good);

    LoRa_begin(&modem);
    LoRa_receive(&modem);
    
	while( 1 ) {
		sleep(7200);
	}

    printf("end\n");

    //LoRa_end(&modem);

	return 0;
}
