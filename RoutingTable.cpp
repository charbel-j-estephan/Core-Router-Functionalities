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
	cout << "Route added: " << key << " -> " << nextHop << endl;
}
void RoutingTable::removeRoute(const string& prefix, int prefixLen) {
	string key = prefix + "/" + to_string(prefixLen);

	if (routes.find(key) != routes.end()) {
		routes.erase(key); 
		cout << "route removed: " << key << endl;
	}
	else {
		cout << "route not found:"<<key << endl;
	}
}
void RoutingTable::displayRoutingTable() const {
    cout << "\n========================================" << endl;
    cout << "           ROUTING TABLE                " << endl;
    cout << "========================================" << endl;
    cout << "Network/Prefix\t\tNext Hop\tMetric" << endl;
    cout << "----------------------------------------" << endl;

    if (routes.empty()) {
        cout << "No routes in table." << endl;
        return;
    }

    for (const auto& pair : routes) {
        const RouterEntry& entry = pair.second;  

        cout << entry.getNetworkPrefix() << "/" << entry.getPrefixLength()
            << "\t\t" << entry.getNextHop()
            << "\t\t" << entry.getMetric() << endl;
    }

    cout << "========================================" << endl;
    cout << "Total routes: " << routes.size() << endl;
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
RouterEntry* RoutingTable::findBestRoute(const std::string& destIP) {
    // Step 1: Initialize tracking variables
    RouterEntry* bestMatch = nullptr;
    int longestMatch = -1;

    // Step 2: Loop through all routes in the map
    for (auto& pair : routes) {
        RouterEntry& entry = pair.second;  // Get the route entry

        // Step 3: Check if this route matches the destination
        if (ipMatchesPrefix(destIP, entry)) {
            // Step 4: Is this match more specific than our current best?
            int currentPrefixLen = entry.getPrefixLength();

            if (currentPrefixLen > longestMatch) {
                // This is a better match!
                bestMatch = &entry;  // Store pointer to this entry
                longestMatch = currentPrefixLen;
            }
            // Optional: If prefix lengths are equal, compare metrics
            else if (currentPrefixLen == longestMatch && bestMatch != nullptr) {
                if (entry.getMetric() < bestMatch->getMetric()) {
                    bestMatch = &entry;  // Lower metric wins
                }
            }
        }
    }

    // Step 5: Return the best match (could be nullptr if no matches)
    return bestMatch;
}