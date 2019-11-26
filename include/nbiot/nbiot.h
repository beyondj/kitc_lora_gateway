#ifndef _NBIOT_HPP_
#define _NBIOT_HPP_

#include <sensor/sensor.h>

typedef struct _GATEWAY_DATA{
	char mac_[18];
	int rssi_;
	int status_; // need to change Status -> int
	int rcount_;
	int needReboot_;
}GatewayData;

typedef struct _NBIOT_DATA{
	Data sensorData_;
	GatewayData gatewayData_;
}NB_Data;

int connectToNbIotServer();

#endif //_NBIOT_HPP_
