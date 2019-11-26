#include <db/DataBase.h>
#include <cassert>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <nbiot/nbiot.h>
#include <mac/mac.h>

#define status 0
#define count 1


DataBase::DataBase() 
{
	memset(mac_, 0, sizeof(char)*18);
	bool ret = getMacAddress(mac_);
	if (ret){
		printf("gateway mac is weired....\n");
	}
	//logfd_ = open("../../server.log",O_RDWR | O_APPEND | O_CREAT, S_IRUSR| S_IWUSR | S_IRGRP |S_IWGRP | S_IROTH| S_IWOTH);
}


DataBase::~DataBase() {
		//std::cout<<"DTOR..."<<std::endl;
		//close(logfd_);
}


DataBase* DataBase::instance_ = nullptr;

DataBase& DataBase::getInstance(){
	if(!instance_){
		instance_ = new DataBase();
		std::cout<<"Automatic sensor detection module is active..."<<std::endl<<std::endl;
	}
	return *instance_;
}

Status DataBase::query(const std::string& mac){
	//std::cout<<"query("<<mac<<")"<<std::endl;
	auto search = dbInfo_.find(mac);
	if (search != dbInfo_.end()){
		return std::get<status>(search->second);
	}
	return Status::Newby;
}

void DataBase::saveClient(const std::string& mac){
	std::cout<<"Sensor mac address: "<<mac<<", status: Good"<<std::endl<<std::endl;
	
	dbInfo_.emplace(mac, std::make_pair(Status::Good, 0));
	macVCount_.emplace(mac, 0);
	showDataBaseStatus();
}

void DataBase::reboot(const std::string& mac){
	std::cout<<"Abnormal sensor behavior detected. Requesst sensor to recover."<<std::endl;
	std::cout<<"Update DB information about the sensor"<<std::endl;
	std::cout<<"Sensor mac address: "<<mac<<" status: Bad"<<std::endl<<std::endl;
	
	auto search = dbInfo_.find(mac);
	if(search ==dbInfo_.end()){
	//	assert(!"no corresponding mac...");
		printf("no correspongding mac inforamtion in DB\n");
	}
	else{
		std::get<status>(search->second) = Status::Bad;
		std::get<count>(search->second)++;
		showDataBaseStatus();
	}
}

void DataBase::activate(const std::string& mac){
	auto search = dbInfo_.find(mac);
		std::cout<<"Sensor mac address: "<<mac<<", change status: Bad->Good"<<std::endl<<std::endl;
	if(search ==dbInfo_.end()){
		printf("no correspongding mac inforamtion in DB\n");
		//assert(!"no corresponding mac...");
	}
	else{
		std::get<status>(search->second) = Status::Good;
	}
	showDataBaseStatus();
}

int DataBase::rebootCount(const std::string& mac){
	auto search = dbInfo_.find(mac);
	if(search ==dbInfo_.end()){
		printf("no correspongding mac inforamtion in DB\n");
		//assert(!"no corresponding mac...");
		return 0;
	}
	
	return std::get<count>(search->second);
}

std::string stringDic(Status enumclass){
	if(enumclass == Status::Good)
		return std::string{"Good  "} ;

	return std::string{"Bad   "};
}

void DataBase::showDataBaseStatus(){
//	system("echo \"--------------------------------------\" >> ../../server.log");
//	system("echo \"| mac               | status | r cnt |\" >> ../../server.log");
//	system("echo \"--------------------------------------\" >> ../../server.log");

	
	std::string logstart =  std::string{"--------------------------------------"}+
							std::string{"\n| mac               | status | r cnt |"}+
							std::string{"\n--------------------------------------\n"};

	std::cout<<logstart;
/*	int length = logstart.length();
	char buf[1024];
	memset(buf, 0, sizeof(char)*1024);

	memcpy(buf, logstart.c_str(), sizeof(char)*length+1);

	write(logfd_, buf, sizeof(char)*length+1);
*/
//	std::cout<<"--------------------------------------"<<std::endl;
//	std::cout<<"| mac               | status | r cnt |"<<std::endl;
//	std::cout<<"--------------------------------------"<<std::endl;

	


	for ( auto &item : dbInfo_){
	std::cout<<"| "<<item.first<<" | "<<stringDic(std::get<status>(item.second))<<" |     "
	<<std::get<count>(item.second)<<" | "<<std::endl;
	std::cout<<"--------------------------------------"<<std::endl;
		
/*		std::string line = "| "+item.first+" | "+stringDic(std::get<status>(item.second))+
	" |     "+std::to_string(std::get<count>(item.second))+" | \n--------------------------------------\n";

	
		int line_length = line.length();
		memcpy(buf, line.c_str(), sizeof(char)* line_length+1);

		write(logfd_, buf, sizeof(char)*line_length+1);*/
	} 



	std::cout<<std::endl;
}
void DataBase::increaseErrorCount(const std::string& mac){
	macVCount_.at(mac)++;
}

void DataBase::resetErrorCount(const std::string& mac){
	macVCount_.at(mac) = 0;
}
int DataBase::getErrorCount(const std::string& mac){
	return macVCount_.at(mac);
}

int DataBase::statusToint(const Status& statuss){
	if (statuss == Status::Newby) {
		return 0;
	}
	if (statuss == Status::Good){
		return 1;
	}
	
	return 2;
}

void DataBase::setInfo(const std::string& mac, GatewayData& gatewayData){
	auto search = dbInfo_.find(mac);
	if(search == dbInfo_.end()){
		printf("no correspongding mac information in DB\n");
	}
	else{
		gatewayData.status_ = statusToint(std::get<status>(search->second));
		gatewayData.rcount_ = std::get<count>(search->second);
		memcpy(gatewayData.mac_, mac_, sizeof(char)*18);
	}
}


