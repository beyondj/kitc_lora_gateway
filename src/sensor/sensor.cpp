#include <sensor/sensor.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#define MAX_TIMINGS 85                                                       
#define DHT_PIN 3                                                            

int data[5]= {0,0,0,0,0};                                                    

SensorValue getSensorValue(){

	SensorValue ret = {0,0.0,0.0};

	if (wiringPiSetup() == -1){
		assert(!"wiringPiSetup is error... in sensor.c");
		exit(1);
	}

	uint8_t laststate = HIGH;                                                
	uint8_t counter = 0;                                                     
		
	uint8_t j = 0, i;                                                        

	data[0] = data[1] = data[2] = data[3] = data[4] = 0;                     

	pinMode(DHT_PIN, OUTPUT);                                                
	digitalWrite(DHT_PIN, LOW);                                              
	delay(18);                                                               

	pinMode(DHT_PIN, INPUT);                                                 

	for( i = 0 ;i < MAX_TIMINGS; i++){                                       
			counter = 0;                                                         
			while( digitalRead(DHT_PIN) == laststate){                           
					counter++;                                                       
					delayMicroseconds(1);                                            
					if(counter== 255){                                               
							break;                                                       
					}                                                                
			}                                                                    

			laststate = digitalRead(DHT_PIN);                                    

			if( counter ==255)                                                   
					break;                                                           

			if( (i >=4) && ( i% 2 == 0 ) ){                                      
					data[j/8] <<=1;                                                  
					if(counter > 50)                                                 
							data[j/8] |= 1;                                              
					j++;                                                             

			}                                                                    
	}                                                                        

	if( (j >=40) &&                                                                 
					(data[4] == ( ( data[0] + data[1] + data[2]+ data[3]) & 0xFF) ) ){      

			float h = (float)((data[0] <<8) + data[1]) / 10;                            

			if( h >100)                                                                 
			{                                                                           
					h = data[0];                                                            
			}                                                                           

			float c = (float) (((data[2] & 0x7F) << 8 ) + data[3])/10;                  

			if( c> 125){                                                                
					c = data[2];                                                            
			}                                                                           

			if( data[2] & 0x80 ){                                                       
					c = -c;                                                                 
			}                                                                           
			float f = c*1.8f +32;                                                       
			
			ret.validity_ = true;
			ret.humin_ = h;
			ret.temper_ = c;

			//printf("Humidity = %.1f %% Temperature = %.1f *C /(%.1f *F)\n", h, c, f);   
			//printf("[Sensor Data] Humidity = %.1f %% Temperature = %.1f *C \n", h, c);   
	}                                                                               
	else{   
			ret.validity_ = false;
			ret.humin_ = 0.0;
			ret. temper_ = 0.0;
			//printf("[Sensor Data] Invalid. Humidity = 0.0 Temperature = 0.0 \n");                                           
		}

	return ret;
}


