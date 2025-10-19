#include "RoutingTable.h"
#include <iostream>
#include <sstream>
using namespace std;

RoutingTable::RoutingTable() {
}

bool RoutingTable::isEmpty() const {
    return routes.empty();
}

size_t RoutingTable::getRouteCount() const {
    return routes.size();
}

void RoutingTable::addRoute(const string& prefix, int prefixLen, const string& nextHop, int metric) {
    string key = prefix + "/" + to_string(prefixLen);
    RouterEntry entry(prefix, prefixLen, nextHop, metric);
    routes[key] = entry;
}

void RoutingTable::removeRoute(const string& prefix, int prefixLen) {
    string key = prefix + "/" + to_string(prefixLen);

    if (routes.find(key) != routes.end()) {
        routes.erase(key);
        cout << "Route removed: " << key << endl;
    }
    else {
        cout << "Route not found: " << key << endl;
    }
}

void RoutingTable::displayRoutingTable() const {
    cout << "\nRouting Table (" << routes.size() << " routes):" << endl;

    if (routes.empty()) {
        cout << "No routes configured." << endl;
        return;
    }

    for (const auto& pair : routes) {
        const RouterEntry& entry = pair.second;
        cout << "  " << entry.getNetworkPrefix() << "/" << entry.getPrefixLength()
            << " -> " << entry.getNextHop()
            << " (Metric:" << entry.getMetric() << ")" << endl;
    }
}

unsigned long RoutingTable::ipToInt(const string& ip) {
    unsigned long result = 0;
    stringstream ss(ip);
    string octet;
    int position = 24;

    while (getline(ss, octet, '.')) {
        unsigned long value = stoul(octet);
        result |= (value << position);
        position -= 8;
    }

    return result;
}

bool RoutingTable::ipMatchesPrefix(const string& destIP, const RouterEntry& entry) {
    unsigned long destIPInt = ipToInt(destIP);
    unsigned long prefixInt = ipToInt(entry.getNetworkPrefix());

    int prefixLen = entry.getPrefixLength();
    unsigned long mask = 0xFFFFFFFF << (32 - prefixLen);

    return (destIPInt & mask) == (prefixInt & mask);
}

RouterEntry* RoutingTable::findBestRoute(const string& destIP) {
    RouterEntry* bestMatch = nullptr;
    int longestMatch = -1;

    for (auto& pair : routes) {
        RouterEntry& entry = pair.second;

        if (ipMatchesPrefix(destIP, entry)) {
            int currentPrefixLen = entry.getPrefixLength();

            if (currentPrefixLen > longestMatch) {
                bestMatch = &entry;
                longestMatch = currentPrefixLen;
            }
            else if (currentPrefixLen == longestMatch && bestMatch != nullptr) {
                if (entry.getMetric() < bestMatch->getMetric()) {
                    bestMatch = &entry;
                }
            }
        }
    }

    return bestMatch;
}