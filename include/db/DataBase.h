#ifndef _DATABASE_HPP_
#define _DATABASE_HPP_

#include <map>
#include <utility>
#include <nbiot/nbiot.h>

enum class Status{
	Newby,
	Good,
	Bad
};

class DataBase{
private:
	char mac_[18];
	std::map<std::string, std::pair<Status, int>> dbInfo_;
	std::map<std::string, int> macVCount_;
	static DataBase* instance_;
	// no public CTOR
	DataBase();

//	int logfd_;
public:
	virtual ~DataBase();

	static DataBase& getInstance();

	Status query(const std::string&);
	void saveClient(const std::string&);
	void reboot(const std::string&);
	int rebootCount(const std::string&);
	void activate(const std::string&);
	void showDataBaseStatus();
	void increaseErrorCount(const std::string&);
	void resetErrorCount(const std::string&);
	int getErrorCount(const std::string&);
	void setInfo(const std::string&, GatewayData&);
	int statusToint(const Status&);

};


#endif //_DATABASE_HPP_
