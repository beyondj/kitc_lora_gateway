#include <pipe/pipe.h>
#include <unistd.h>
#include <stdio.h>
#include <nbiot/nbiot.h>

#define p_read  0
#define p_write 1

Pipe::Pipe() : buf_{}
{
	int ret = pipe(pipeFd_);
	if (ret<0){
		printf("pipe.cpp pipe() error!\n");
	}

}

Pipe::~Pipe(){
	close(pipeFd_[p_read]);
	close(pipeFd_[p_write]);

}

Pipe* Pipe::instance_ = nullptr;

Pipe& Pipe::getInstance(){
	if(!instance_){
		instance_ = new Pipe();
	}

	return *instance_;

}


void Pipe::write(const NB_Data& data){
	::write(pipeFd_[p_write], &data, sizeof(NB_Data));

}
int Pipe::read(){
	return ::read(pipeFd_[p_read], buf_, sizeof(NB_Data));
}

char* Pipe::getBuf(){
	return buf_;
}


