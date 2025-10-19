#include "PacketHistory.h"
#include <iostream>
#include <iomanip>

using namespace std;

PacketHistory::PacketHistory() {
    packetID = 0;
}

PacketHistory::PacketHistory(int packetID) {
    this->packetID = packetID;
}

void PacketHistory::addTrace(const TraceEntry& entry) {
    traceList.push_back(entry);
    visitedRouters.insert(entry.getRouterID());
}

void PacketHistory::addTrace(const string& routerID, const string& action,
    int queueDelay, int remainingTTL, const string& nextHop) {
    TraceEntry entry(routerID, action, queueDelay, remainingTTL, nextHop);
    addTrace(entry);
}

bool PacketHistory::hasVisitedRouter(const string& routerID) const {
    return visitedRouters.find(routerID) != visitedRouters.end();
}

bool PacketHistory::detectLoop(const string& routerID) {
    if (hasVisitedRouter(routerID)) {
        cout << "LOOP DETECTED! Packet " << packetID
            << " has already visited router " << routerID << endl;
        return true;
    }
    return false;
}

void PacketHistory::displayHistory() const {
    cout << "\n========================================" << endl;
    cout << "PACKET FLOW HISTORY - Packet ID: " << packetID << endl;
    cout << "========================================" << endl;

    if (traceList.empty()) {
        cout << "No trace entries recorded." << endl;
        cout << "========================================" << endl;
        return;
    }

    int hopNumber = 1;
    for (const auto& entry : traceList) {
        cout << "Hop #" << hopNumber++ << ": ";
        entry.display();
    }

    cout << "----------------------------------------" << endl;
    cout << "Total Hops: " << getHopCount()
        << " | Total Delay: " << getTotalDelay() << "ms" << endl;
    cout << "Final Action: " << getFinalAction() << endl;
    cout << "========================================" << endl;
}

void PacketHistory::displayCompactHistory() const {
    cout << "Packet " << packetID << ": ";

    if (traceList.empty()) {
        cout << "No history" << endl;
        return;
    }

    for (auto it = traceList.begin(); it != traceList.end(); ++it) {
        cout << it->getRouterID();
        if (next(it) != traceList.end()) {
            cout << " => ";
        }
    }

    cout << " [" << getFinalAction() << ", "
        << getHopCount() << " hops, "
        << getTotalDelay() << "ms]" << endl;
}

void PacketHistory::displayReverseHistory() const {
    cout << "\n=== REVERSE PACKET TRACE (Packet " << packetID << ") ===" << endl;

    if (traceList.empty()) {
        cout << "No trace entries." << endl;
        return;
    }

    int hopNumber = static_cast<int>(traceList.size());
    for (auto it = traceList.rbegin(); it != traceList.rend(); ++it) {
        cout << "Hop #" << hopNumber-- << ": ";
        it->display();
    }
}

int PacketHistory::getPacketID() const {
    return packetID;
}

size_t PacketHistory::getHopCount() const {
    return traceList.size();
}

list<TraceEntry> PacketHistory::getTraceList() const {
    return traceList;
}

set<string> PacketHistory::getVisitedRouters() const {
    return visitedRouters;
}

int PacketHistory::getTotalDelay() const {
    int total = 0;
    for (const auto& entry : traceList) {
        total += entry.getQueueDelay();
    }
    return total;
}

string PacketHistory::getFinalAction() const {
    if (traceList.empty()) {
        return "UNKNOWN";
    }
    return traceList.back().getAction();
}

string PacketHistory::getLastRouter() const {
    if (traceList.empty()) {
        return "NONE";
    }
    return traceList.back().getRouterID();
}

void PacketHistory::clear() {
    traceList.clear();
    visitedRouters.clear();
}