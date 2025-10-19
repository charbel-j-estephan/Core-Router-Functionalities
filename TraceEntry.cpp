#include "TraceEntry.h"
#include <ctime>
#include <iostream> 
using namespace std; 

TraceEntry::TraceEntry() {
    routerID = ""; 
    action = ""; 
    queueDelay = 0; 
    remainingTTL = 0;
    nextHop = "";
	timestamp = time(nullptr);
}; 


TraceEntry::TraceEntry(const string& routerID, const string& action,
    int queueDelay, int remainingTTL, const string& nextHop) {
	this->routerID = routerID;
    this->action = action; 
    this->queueDelay = queueDelay; 
    this->remainingTTL = remainingTTL; 
    this->nextHop = nextHop; 
	timestamp = time(nullptr);
}; 

string TraceEntry::getRouterID() const {
    return routerID; 
};
time_t TraceEntry::getTimestamp() const {
    return timestamp; 
};
string TraceEntry::getAction() const {
    return action; 
};
int TraceEntry::getQueueDelay() const {
    return queueDelay; 
};
int TraceEntry::getRemainingTTL() const {
    return remainingTTL; 
};
string TraceEntry::getNextHop() const {
    return nextHop;; 
};

void TraceEntry::setRouterID(const string& routerID) {
    this->routerID = routerID;
};
void TraceEntry::setAction(const string& action) {
    this->action = action; 
};
void TraceEntry::setQueueDelay(int queueDelay) {
    this->queueDelay = queueDelay;
};
void TraceEntry::setRemainingTTL(int remainingTTL) {
	this->remainingTTL = remainingTTL;
};
void TraceEntry::setNextHop(const string& nextHop) {
    this->nextHop = nextHop; 
};

void TraceEntry::display() const {
	cout << "Router ID: " << routerID << endl;
	cout << "Timestamp: " << timestamp << endl;
	cout << "Action: " << action << endl;
	cout << "Queue Delay: " << queueDelay << " ms" << endl; 
	cout << "Remaining TTL: " << remainingTTL << endl;
	cout << "Next Hop: " << nextHop << endl;

};