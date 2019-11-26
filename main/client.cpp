#include <lora/lora.h>
#include <stdio.h>
#include <stdlib.h>
#include <sensor/sensor.h>
#include <unistd.h>
#include <mac/mac.h>
#include <sys/time.h>
#include <iostream>

typedef struct Reply_{
	int ack_;
	int needReboot_;
}Reply;


long long current_timestamp() {
	struct timeval te;
		
	gettimeofday(&te, NULL); // get current time
	long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
	return milliseconds;
}


long long txts = 0;
long long rxts = 0;


int count = 0;
void tx_f(txData *tx){
    LoRa_ctl *modem = (LoRa_ctl *)(tx->userPtr);
    Data data = *(Data*)(tx->buf);
   
	//txts = current_timestamp();	

	LoRa_receive(modem);
}



void rx_f(rxData *rx){

    LoRa_ctl *modem = (LoRa_ctl *)(rx->userPtr);

	//rxts = current_timestamp();

    //LoRa_stop_receive(modem);//manually stoping RxCont mode
	int CRCError = rx->CRC;
    if (CRCError){
		printf("[ERROR_CRC_BAD_NETWORK_COND] Ignore invalid data\n");	
	}
	else{
		printf("Reply from server \t");

		Reply reply = *(Reply*)(rx->buf);

		printf("Received ack: %d  NeedReboot: %d\n\n", reply.ack_, reply.needReboot_);
		//std::cout<<"Send Ts:"<<txts<<"  Receive Ts:"<<rxts<<std::endl;
		//std::cout<<"Response Time:"<<rxts-txts<<" miliseconds"<<std::endl<<std::endl;
	
		if ( reply.needReboot_){
				printf("Reboot reqested..\n\n");
					//system("sudo reboot");
		} 
	}
}


int main(){

	Data data;
	memset(&data, 0, sizeof(data));

	bool ret = getMacAddress(data.mac_);
	while(getMacAddress(data.mac_)){
		printf("ret=%d mac=%s\n",ret,data.mac_);	
	}


    char txbuf[255];
    char rxbuf[255];
    LoRa_ctl modem;

	lora_initiate(modem, rx_f, rxbuf, tx_f, txbuf, Bandwidth::good);

	sleep(1);
	data.sensor_ = getSensorValue();
	memcpy(modem.tx.data.buf, &data, sizeof(Data));
	modem.tx.data.size = sizeof(Data);	
	printf("[%s]\tSending seq: %d \nValidity: %d  Humidity: %.1f  Temperature: %.1f\n",
				data.mac_,data.count_, data.sensor_.validity_, data.sensor_.humin_, data.sensor_.temper_);


	LoRa_begin(&modem);
	LoRa_send(&modem);
  
//	LoRa_send(&modem);
	while(1){
	    sleep(5);
//	    LoRa_receive(&modem);
		data.count_++;
		data.sensor_ = getSensorValue();
		memcpy(modem.tx.data.buf, &data, sizeof(Data));
		modem.tx.data.size = sizeof(Data);	

		Data testD = *((Data*)(modem.tx.data.buf));
		//printf("Sending data...\n");
		printf("[%s]\tSending seq: %d\nValidity: %d  Humidity: %.1f  Temperature: %.1f\n",
				testD.mac_,testD.count_, testD.sensor_.validity_, testD.sensor_.humin_, testD.sensor_.temper_);

		LoRa_end(&modem);
		LoRa_begin(&modem);
		LoRa_send(&modem);
		//LoRa_receive(&modem);	
		//sleep(8);
	
	//LoRa_send(&modem);
	}

    
    while(LoRa_get_op_mode(&modem) != SLEEP_MODE){
        sleep(1);
    }

    printf("end\n");

    LoRa_end(&modem);
}
