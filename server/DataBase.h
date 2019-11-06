#ifndef _DATABASE_HPP_
#define _DATABASE_HPP_

#include <map>
#include <utility>

enum class Status{
	Newby,
	Good,
	Bad
};

class DataBase{
private:
	std::map<std::string, std::pair<Status, int>> dbInfo_;
	static DataBase* instance_;
	// no public CTOR
	DataBase();
public:
	virtual ~DataBase();

	static DataBase& getInstance();

	Status query(const std::string&);
	void saveClient(const std::string&);
	void reboot(const std::string&);
	int rebootCount(const std::string&);
	void activate(const std::string&);
};


#endif //_DATABASE_HPP_
