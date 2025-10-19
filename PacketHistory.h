#ifndef PACKETHISTORY_H 
#define PACKETHISTORY_H

#include <list>
#include <set>
#include <string>
#include "TraceEntry.h"

class PacketHistory {
private:
    int packetID;
    std::list<TraceEntry> traceList;  
    std::set<std::string> visitedRouters; 

public:
    PacketHistory();
    PacketHistory(int packetID);

    void addTrace(const TraceEntry& entry);
    void addTrace(const std::string& routerID, const std::string& action,
        int queueDelay, int remainingTTL, const std::string& nextHop = "");

    bool hasVisitedRouter(const std::string& routerID) const;
    bool detectLoop(const std::string& routerID);

    void displayHistory() const;
    void displayCompactHistory() const;
    void displayReverseHistory() const;  

    int getPacketID() const;
    size_t getHopCount() const;
    std::list<TraceEntry> getTraceList() const;
    std::set<std::string> getVisitedRouters() const;

    int getTotalDelay() const;
    std::string getFinalAction() const;
    std::string getLastRouter() const;

    void clear();
};

#endif


