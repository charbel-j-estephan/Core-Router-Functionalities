#include "RouterEntry.h"

#include <iostream>

using namespace std; 


RouterEntry::RouterEntry() {
	networkPrefix = ""; 
	prefixLength = 0; 
	nextHop = ""; 
	metric = 0; 
}
RouterEntry::RouterEntry(const string& networkPrefix, int prefixLength,
	const string& nextHop, int metric) {
	this->networkPrefix = networkPrefix; 
	this->prefixLength = prefixLength;
	this->nextHop = nextHop;
	this->metric = metric;
}

string RouterEntry::getNetworkPrefix() const{
	return networkPrefix;
}
string RouterEntry::getNextHop() const {
	return nextHop;
}
int RouterEntry::getPrefixLength() const {
	return prefixLength;
}
int RouterEntry::getMetric() const {
	return metric;
}

void RouterEntry::setNetworkPrefix(const string& networkPrefix) {
	this->networkPrefix = networkPrefix;
}
void RouterEntry::setNextHop(const string& nextHop) {
	this->nextHop = nextHop;
}

void RouterEntry::setPrefixLength(int prefixLength) {
	this->prefixLength = prefixLength;
}
void RouterEntry::setMetric(int metric) {
	this->metric = metric; 
}

void RouterEntry::display() {
	cout << "Network: " << getNetworkPrefix() << " / " << getPrefixLength() << " Next Hop : " << getNextHop() << ", Metric : " << getMetric() << endl;
}