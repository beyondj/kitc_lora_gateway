#include <lora/lora.h>
#include <stdio.h>
#include <stdlib.h>
#include <sensor/sensor.h>
#include <unistd.h>
#include <mac/mac.h>

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

	printf("received ack:%d\n",*(int*)rx->buf);
   /*// printf("CRC error: %d;\t", rx->CRC);
   // printf("Data size: %d;\t", rx->size);
    printf("received string: \"%s\";\t", rx->buf);//Data we've received
    if( strcmp(rx->buf,"Rebo") == 0){
		printf("REBOOT!!!\n");
		system("sudo reboot");
	}
	printf("RSSI: %d;\t", rx->RSSI);
    printf("SNR: %f\n", rx->SNR);
   */
//    LoRa_receive(modem);
//    LoRa_sleep(modem);
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

	lora_initiate(modem, rx_f, rxbuf, tx_f, txbuf, Bandwidth::best);

/*
    //See for typedefs, enumerations and there values in LoRa.h header file
    modem.spiCS = 0;//Raspberry SPI CE pin number
    modem.tx.callback = tx_f;
    modem.tx.data.buf = txbuf;
    modem.rx.callback = rx_f;
    modem.rx.data.buf = rxbuf;
    modem.rx.data.userPtr = (void *)(&modem);//To handle with chip from rx callback
    modem.tx.data.userPtr = (void *)(&modem);//To handle with chip from tx callback
    //memcpy(modem.tx.data.buf, "Ping", 5);//copy data we'll sent to buffer
    memcpy(modem.tx.data.buf, &data, sizeof(Data));
	modem.tx.data.size = sizeof(Data);

	//modem.tx.data.size = 5;//Payload len
    modem.eth.preambleLen=6;
    //modem.eth.bw = BW62_5;//Bandwidth 62.5KHz
    //modem.eth.sf = SF12;//Spreading Factor 12
    modem.eth.bw = BW125;//Bandwidth 62.5KHz
    modem.eth.sf = SF9;//Spreading Factor 12
    modem.eth.ecr = CR8;//Error coding rate CR4/8
    modem.eth.CRC = 1;//Turn on CRC checking
    modem.eth.freq = 434800000;// 434.8MHz
    modem.eth.resetGpioN = 4;//GPIO4 on lora RESET pin
    modem.eth.dio0GpioN = 17;//GPIO17 on lora DIO0 pin to control Rxdone and Txdone interrupts
    modem.eth.outPower = OP20;//Output power
    modem.eth.powerOutPin = PA_BOOST;//Power Amplifire pin
    modem.eth.AGC = 1;//Auto Gain Control
    modem.eth.OCP = 240;// 45 to 240 mA. 0 to turn off protection
    modem.eth.implicitHeader = 0;//Explicit header mode
    modem.eth.syncWord = 0x12;
    //For detail information about SF, Error Coding Rate, Explicit header, Bandwidth, AGC, Over current protection and other features refer to sx127x datasheet https://www.semtech.com/uploads/documents/DS_SX1276-7-8-9_W_APP_V5.pdf

*/
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