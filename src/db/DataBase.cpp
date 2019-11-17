#include <db/DataBase.h>
#include <cassert>

#define status 0
#define count 1

#include <iostream>

DataBase::DataBase() = default;

DataBase::~DataBase() {
		std::cout<<"DTOR..."<<std::endl;
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
	std::cout<<"--------------------------------------"<<std::endl;
	std::cout<<"| mac               | status | r cnt |"<<std::endl;
	std::cout<<"--------------------------------------"<<std::endl;
	for ( auto &item : dbInfo_){
	std::cout<<"| "<<item.first<<" | "<<stringDic(std::get<status>(item.second))<<" |     "
	<<std::get<count>(item.second)<<" | "<<std::endl;
	std::cout<<"--------------------------------------"<<std::endl;
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

