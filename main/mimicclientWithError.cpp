#include <lora/lora.h>
#include <stdio.h>
#include <stdlib.h>
#include <sensor/sensor.h>
#include <unistd.h>
#include <mac/mac.h>

typedef struct Reply_{
	int ack_;
	int needReboot_;
}Reply;

int count = 0;
void tx_f(txData *tx){
    LoRa_ctl *modem = (LoRa_ctl *)(tx->userPtr);
    //printf("tx done#########;\t");
    //printf("sent string: \"%s\"\n\n", tx->buf);//Data we've sent
    Data data = *(Data*)(tx->buf);
	//printf("Data sequence number: %d\n", data.count_);

	//printf("sent string: \"%d\"\n\n", atoi(tx->buf));//Data we've sent
   
	

    //LoRa_receive(modem);
	//printf("BEFORE LORA_RECEIVE!!\n");
	LoRa_receive(modem);
	//printf("AFTER LORA_RECEIVE!!\n");
}



void rx_f(rxData *rx){
	//printf("@@@@@@@@@@@@@@RECEIVE######\n");

    LoRa_ctl *modem = (LoRa_ctl *)(rx->userPtr);
    //LoRa_stop_receive(modem);//manually stoping RxCont mode
	int CRCError = rx->CRC;
    if (CRCError){
		printf("[ERROR_CRC_BAD_NETWORK_COND] Ignore invalid data\n");	
	}
	else{
		printf("Reply from server \t");

		Reply reply = *(Reply*)(rx->buf);

		printf("Received ack: %d  needReboot: %d\n\n", reply.ack_, reply.needReboot_);

		if ( reply.needReboot_){
				printf("Reboot reqested..\n\n");
					//system("sudo reboot");
		} 
	}
}

void setData(Data* errorData, char* fakeMac, int count, bool validity, float humin, float temper){
	memcpy(errorData->mac_, fakeMac, sizeof(char)*18);
	errorData->count_ = count;

	errorData->sensor_.validity_ = validity;
	if( validity == 0){
		errorData->sensor_.humin_ = 0.0;
		errorData->sensor_.temper_ = 0.0;
	}
	else{
		errorData->sensor_.humin_ = humin;
		errorData->sensor_.temper_ = temper;
	}
}

int main(){

	Data data;
	memset(&data, 0, sizeof(data));

	bool ret = getMacAddress(data.mac_);
	while(getMacAddress(data.mac_)){
		printf("ret=%d mac=%s\n",ret,data.mac_);	
	}


    char txbuf[255]{};
    char rxbuf[255]{};
    LoRa_ctl modem;

	lora_initiate(modem, rx_f, rxbuf, tx_f, txbuf, Bandwidth::good);

	sleep(1);
//	data.sensor_ = getSensorValue();
//	memcpy(modem.tx.data.buf, &data, sizeof(Data));
//	modem.tx.data.size = sizeof(Data);	
//	printf("[%s]\tSending seq: %d \nValidity: %d  Humidity: %.1f  Temperature: %.1f\n",
//				data.mac_,data.count_, data.sensor_.validity_, data.sensor_.humin_, data.sensor_.temper_);



	Data errData;
	memset(&errData, 0, sizeof(Data));
	for( int k = 0 ; k < 2; k++){
	for(int i = 0; i< 7; i++){
	
		if( i ==6){
			setData(&errData,(char*)"00:00:00:00:00:01\0", 7*k+i, true, 1.0,46.0);
		}
		else{
			setData(&errData,(char*)"00:00:00:00:00:01\0", 7*k+i, false, 0.0,0.0);
		}
		memcpy(modem.tx.data.buf, &errData, sizeof(Data));
		modem.tx.data.size = sizeof(Data);	
//		printf("[%s]\tSending seq: %d \nValidity: %d  Humidity: %.1f  Temperature: %.1f\n",
//				errData.mac_,errData.count_, errData.sensor_.validity_, errData.sensor_.humin_, errData.sensor_.temper_);
		LoRa_begin(&modem);
		LoRa_send(&modem);
		sleep(8);
		LoRa_end(&modem);
	}

}
		setData(&errData,(char*)"00:00:00:00:00:02\0", 0, true, 1.0,31.0);

		memcpy(modem.tx.data.buf, &errData, sizeof(Data));
		modem.tx.data.size = sizeof(Data);	
		LoRa_begin(&modem);
		LoRa_send(&modem);
		sleep(5);
		LoRa_end(&modem);



	//LoRa_begin(&modem);
	//LoRa_send(&modem);
  /*
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
*/
    
    while(LoRa_get_op_mode(&modem) != SLEEP_MODE){
        sleep(1);
    }

    printf("end\n");

    LoRa_end(&modem);
}
