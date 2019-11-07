#ifndef _SENSOR_HPP_
#define _SENSOR_HPP_

#include <wiringPi.h>
#include <stdbool.h>

typedef struct _Sensor_Value{
	bool validity_;
	float humin_;
	float temper_;
}SensorValue;

typedef struct Data_{
	char mac_[18];
	int count_;
	SensorValue sensor_;
}Data;

SensorValue getSensorValue();


#endif //_SENSOR_HPP_
