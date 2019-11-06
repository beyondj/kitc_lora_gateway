#include "DataBase.h"
#include <cassert>

#define status 0
#define count 1

DataBase::DataBase() = default;
DataBase::~DataBase() = default;

DataBase* DataBase::instance_ = nullptr;

DataBase& DataBase::getInstance(){
	if(!instance_){
		instance_ = new DataBase();
	}
	return *instance_;
}

Status DataBase::query(const std::string& mac){
	auto search = dbInfo_.find(mac);
	if (search != dbInfo_.end()){
		return std::get<status>(search->second);
	}
	return Status::Newby;
}

void DataBase::saveClient(const std::string& mac){
	dbInfo_.emplace(mac, std::make_pair(Status::Good, 0));
}

void DataBase::reboot(const std::string& mac){
	auto search = dbInfo_.find(mac);
	if(search ==dbInfo_.end()){
		assert(!"no corresponding mac...");
	}

	std::get<status>(search->second) = Status::Bad;
	std::get<count>(search->second)++;
}

void DataBase::activate(const std::string& mac){
	auto search = dbInfo_.find(mac);
	if(search ==dbInfo_.end()){
		assert(!"no corresponding mac...");
	}

	std::get<status>(search->second) = Status::Good;

}

int DataBase::rebootCount(const std::string& mac){
	auto search = dbInfo_.find(mac);
	if(search ==dbInfo_.end()){
		assert(!"no corresponding mac...");
	}

	return std::get<count>(search->second);
}
