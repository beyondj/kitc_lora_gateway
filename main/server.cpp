#include <iostream>
#include <lora/lora.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <epollwifi/epolls.h>
#include <db/DataBase.h>
#include <sensor/sensor.h>
#include <pthread.h>

typedef struct Reply_{
	int ack_;
	int needReboot_;
}Reply;

void tx_f(txData *tx){
    LoRa_ctl *modem = (LoRa_ctl *)(tx->userPtr);
    printf("tx done;\t");

	Reply* reply =(Reply*) tx->buf;
    printf("sent reply ack = %d, needReboot = %d\n\n", reply->ack_, reply->needReboot_);//Data we've sent

	LoRa_receive(modem);
//    LoRa_sleep(modem);
}



int errorCount = 0;
void _processDBbyStatus(const std::string& mac){
	DataBase& db = DataBase::getInstance();
	Status status = db.query(mac);

	if (status == Status::Newby){
		std::cout<<"Newby..."<<std::endl;
		db.saveClient(mac);
	}
	else if(status == Status::Bad){
		db.activate(mac);
	}
	else{} //status == Status::Good
}

int _processReplybyValidity(bool validity, const std::string& mac){
	if(validity == 0){
		errorCount++;
			
		if (errorCount > 5){
			DataBase& db = DataBase::getInstance();
			db.reboot(mac);
			return 1;
		}
	}

	else{
		errorCount = 0;
	}

	return 0;
}

void rx_f(rxData *rx){
    LoRa_ctl *modem = (LoRa_ctl *)(rx->userPtr);
    LoRa_stop_receive(modem);//manually stoping RxCont mode
    printf("rx done;\t");
    printf("CRC error: %d;\t", rx->CRC);
	Data data = *(Data*)(rx->buf);
    printf("Mac:%s \nReceived count: %d  validity_ :%d   humiditiy: %.1f  Temperatur: %.1f!!!!!!!!!!!\n",
	data.mac_,data.count_, data.sensor_.validity_, data.sensor_.humin_, data.sensor_.temper_);

    //printf("Mac:%s \nReceived count: %d  validity_ :%d   humiditiy: %.1f  Temperatur: %.1f!!!!!!!!!!!\n\n",
	Reply reply;
	memset(&reply, 0, sizeof(Reply));

	
	//lora communication error... do nothing..
	if (rx->CRC){
		reply.ack_ = -1;
	}

	else {
		reply.ack_ = data.count_;
		
		Data data = *(Data*)(rx->buf);
		std::string mac{data.mac_};

	    _processDBbyStatus(mac);
		reply.needReboot_ = _processReplybyValidity(data.sensor_.validity_, mac);


	}
	

	
	
	printf("RSSI: %d;\t", rx->RSSI);
    //printf("SNR: %f\n\n", rx->SNR);
   
   memcpy(modem->tx.data.buf,&reply, sizeof(Reply));
   modem->tx.data.size = sizeof(Reply);

	LoRa_begin(modem);
    LoRa_send(modem);
}


void * wifiThread(void* arg){
	int server_socket = createSocket();
	int epoll_fd = registerToEpoll(server_socket);
	startEpollWait(epoll_fd, server_socket);
}

int main(){

	DataBase::getInstance();

	pthread_t thread_t;
	if (pthread_create(&thread_t, NULL, wifiThread, NULL) <0){
		perror("thread create error:");
		exit(0);
	}
	pthread_detach(thread_t);

	sleep(1);


    char txbuf[255];
    char rxbuf[255];
    LoRa_ctl modem;

	lora_initiate(modem,rx_f,rxbuf,tx_f,txbuf, Bandwidth::good);

    LoRa_begin(&modem);
    LoRa_receive(&modem);
    
	while( 1 ) {
	//	printf("Dahyeon loves Jiyoung S2...\n");
		sleep(7200);
	}
    //while(LoRa_get_op_mode(&modem) != SLEEP_MODE){
    //    sleep(1);
    //}

    printf("end\n");

    LoRa_end(&modem);
}
