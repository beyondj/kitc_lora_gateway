#ifndef _PIPE_HPP_
#define _PIPE_HPP_


#include <sensor/sensor.h>
#include <nbiot/nbiot.h>

class Pipe{
private:
	int pipeFd_[2];
	static Pipe* instance_;
	char buf_[1024];

	Pipe();

public:
	virtual ~Pipe();

	static Pipe& getInstance();
	
	void write(const NB_Data&);
	int read();
	char* getBuf();
	
};




#endif // _PIPE_HPP_
