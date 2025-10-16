#ifndef ROUTINGTABLE_H
#define ROUTINGTABLE_H
#include <map>
#include "RouterEntry.h"
class RoutingTable {
private:
	std::map<std::string, RouterEntry> routes;

public:
	RoutingTable();
	bool isEmpty() const;
	size_t getRouteCount() const;
	void addRoute(const std::string& prefix, int prefixLen, const std::string& nextHop, int metric = 1);
	void removeRoute(const std::string& prefix, int prefixLen);
	void displayRoutingTable() const;
	unsigned long ipToInt(const std::string& ip);
	bool ipMatchesPrefix(const std::string& destIP, const RouterEntry& entry);
	RouterEntry* findBestRoute(const std::string& destIP);
};

#endif