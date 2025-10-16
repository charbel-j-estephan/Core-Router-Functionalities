#ifndef ROUTERENTRY_H 
#define ROUTERENTRY_H
#include <string>
class RouterEntry {
private: 
	std::string networkPrefix; 
	int prefixLength; 
	std::string nextHop; 
	int metric; 
public: 
	RouterEntry(); 
	RouterEntry(const std::string& networkPrefix, int prefixLength,const std::string& nextHop, int metric);

	std::string getNetworkPrefix() const;
	std::string getNextHop() const;
	int getPrefixLength() const;
	int getMetric() const;

	void setNetworkPrefix(const std::string& networkPrefix); 
	void setNextHop(const std::string& nextHop); 
	void setPrefixLength(int prefixLength); 
	void setMetric(int metric);

	void display(); 

};

#endif