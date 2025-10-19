#ifndef TRACEENTRY_H 
#define TRACEENTRY_H
#include <string>

class TraceEntry {
private:
    std::string routerID;
    time_t timestamp;
    std::string action;
    int queueDelay;
    int remainingTTL;
    std::string nextHop;
public:
    TraceEntry();
    TraceEntry(const std::string& routerID, const std::string& action,
        int queueDelay, int remainingTTL, const std::string& nextHop = "");

    std::string getRouterID() const;
    time_t getTimestamp() const;
    std::string getAction() const;
    int getQueueDelay() const;
    int getRemainingTTL() const;
    std::string getNextHop() const;

    void setRouterID(const std::string& routerID);
    void setAction(const std::string& action);
    void setQueueDelay(int delay);
    void setRemainingTTL(int ttl);
    void setNextHop(const std::string& nextHop);   
    void display() const;
};



#endif