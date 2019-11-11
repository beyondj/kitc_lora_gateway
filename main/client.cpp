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
    printf("tx done#########;\t");
    //printf("sent string: \"%s\"\n\n", tx->buf);//Data we've sent
    Data data = *(Data*)(tx->buf);
	printf("send string: %d\n", data.count_);

	//printf("sent string: \"%d\"\n\n", atoi(tx->buf));//Data we've sent
   
	

    //LoRa_receive(modem);
	printf("BEFORE LORA_RECEIVE!!\n");
	LoRa_receive(modem);
	printf("AFTER LORA_RECEIVE!!\n");
}



void rx_f(rxData *rx){
	printf("@@@@@@@@@@@@@@RECEIVE######\n");

    LoRa_ctl *modem = (LoRa_ctl *)(rx->userPtr);
    LoRa_stop_receive(modem);//manually stoping RxCont mode
    printf("rx done;\t");
	
	Reply reply = *(Reply*)(rx->buf);

	printf("received ack:%d  need reboot:%d", reply.ack_, reply.needReboot_);
	
	int CRCError = rx->CRC;
	if ( CRCError==0 && reply.needReboot_){
		//printf("RRRRRRRRRRRRRRRRRREEEEEEEEEEEEEBOOOOOOOOOOOOOOOOOOOOOOTTTTTTTTTT!!!!!\n\n");
		system("sudo reboot");
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

    LoRa_begin(&modem);
	//pthread_t tid;
	//pthread_create(&tid, NULL, test , &modem);
	//pthread_detach(tid);
	sleep(1);
	LoRa_send(&modem);
  
//	LoRa_send(&modem);
	while(1){
	    sleep(8);
//	    LoRa_receive(&modem);
		data.count_++;
		data.sensor_ = getSensorValue();
		memcpy(modem.tx.data.buf, &data, sizeof(Data));
		modem.tx.data.size = sizeof(Data);	

		Data testD = *((Data*)(modem.tx.data.buf));
		printf("Sending data...\n");
		printf("mac=%s count=%d validity=%d  humidity:%.1f  temper:%.1f!!\n",
				testD.mac_,testD.count_, testD.sensor_.validity_, testD.sensor_.humin_, testD.sensor_.temper_);

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
