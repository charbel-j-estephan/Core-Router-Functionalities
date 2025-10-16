#include "qoservice.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

QoService::QoService(int maxSize) : highCount(0), mediumCount(0), lowCount(0), maxQueueSize(maxSize), forwardedStatus(false) {
    cout << "QoService initialized with max queue size = " << maxQueueSize << endl;
}

vector<packets> QoService::readPacketsFromFile(const string& filename) {
    vector<packets> packetList;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Unable to open file " << filename << endl;
        return packetList;
    }

    string line;
    // Skip header line if present
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<string> tokens;

        // Parse comma-separated values
        while (getline(ss, token, ',')) {
            // Trim whitespace
            size_t start = token.find_first_not_of(" \t\r\n");
            size_t end = token.find_last_not_of(" \t\r\n");
            if (start != string::npos && end != string::npos) {
                tokens.push_back(token.substr(start, end - start + 1));
            }
            else if (start != string::npos) {
                tokens.push_back(token.substr(start));
            }
            else {
                tokens.push_back("");
            }
        }

        // Create packet from parsed data
        if (tokens.size() >= 5) {
            int id = stoi(tokens[0]);
            string source = tokens[1];
            string destination = tokens[2];
            int port = stoi(tokens[3]);
            int ttl = stoi(tokens[4]);

            packets packet(id, source, destination, port, ttl);
            packetList.push_back(packet);
        }
    }

    file.close();
    cout << "Successfully read " << packetList.size() << " packets from file." << endl;
    return packetList;
}

void QoService::classifyPackets(const vector<packets>& packetVec) {
    for (size_t i = 0; i < packetVec.size(); i++) {
        packets packet = packetVec[i];
        int port = packet.getPort();

        // Classification rules based on port ranges
        if (port >= 0 && port <= 1023) {
            // Well-known ports - High priority
            packet.setPriority("High");
            if (highCount < maxQueueSize) {
                highPriorityQueue.push(packet);
                highCount++;
            }
            else {
                cout << "High priority queue full! Packet ID " << packet.getId() << " dropped." << endl;
            }
        }
        else if (port >= 1024 && port <= 49151) {
            // Registered ports - Medium priority
            packet.setPriority("medium");
            if (mediumCount < maxQueueSize) {
                mediumPriorityQueue.push(packet);
                mediumCount++;
            }
            else {
                cout << "Medium priority queue full! Packet ID " << packet.getId() << " dropped." << endl;
            }
        }
        else {
            // Dynamic/Private ports - Low priority
            packet.setPriority("Low");
            if (lowCount < maxQueueSize) {
                lowPriorityQueue.push(packet);
                lowCount++;
            }
            else {
                cout << "Low priority queue full! Packet ID " << packet.getId() << " dropped." << endl;
            }
        }
    }

    cout << "Packets classified and enqueued." << endl;
}

packets QoService::getNextPacket() {
    // Priority order: High > Medium > Low
    if (!highPriorityQueue.empty() && highCount > 0) {
        packets packet = highPriorityQueue.front();
        highPriorityQueue.pop();
        highCount--;
        return packet;
    }
    else if (!mediumPriorityQueue.empty() && mediumCount > 0) {
        packets packet = mediumPriorityQueue.front();
        mediumPriorityQueue.pop();
        mediumCount--;
        return packet;
    }
    else if (!lowPriorityQueue.empty() && lowCount > 0) {
        packets packet = lowPriorityQueue.front();
        lowPriorityQueue.pop();
        lowCount--;
        return packet;
    }

    // Return empty packet if all queues are empty
    return packets();
}

void QoService::forwardOrDrop() {
    cout << "\n Processing Packets" << endl;

    int forwarded = 0;
    int dropped = 0;

    while (!allQueuesEmpty()) {
        packets packet = getNextPacket();

        // Check if packet is valid (not default constructed)
        if (packet.getId() == 0) {
            break;
        }

        // Decrement TTL before checking
        packet.decrementTTL();

        if (packet.getTTL() > 0) {
            setForwardedStatus(true);
            cout << "FORWARDED: ";
            packet.display();
            forwarded++;
        }
        else {
            cout << "DROPPED (TTL expired): Packet ID " << packet.getId() << endl;
            setForwardedStatus(false);
            dropped++;
        }
    }

    cout << "\n Processing Completed" << endl;
    cout << "Total forwarded: " << forwarded << endl;
    cout << "Total dropped: " << dropped << endl;
}

bool QoService::allQueuesEmpty() const {
    return highPriorityQueue.empty() && mediumPriorityQueue.empty() && lowPriorityQueue.empty();
}

void QoService::displayQueueStatus() const {
    cout << "\nQueue Status" << endl;
    cout << "High Priority Queue: " << highCount << "/" << maxQueueSize << endl;
    cout << "Medium Priority Queue: " << mediumCount << "/" << maxQueueSize << endl;
    cout << "Low Priority Queue: " << lowCount << "/" << maxQueueSize << endl;
}
void QoService::setForwardedStatus(bool ft) {
    forwardedStatus = ft;
}

bool QoService::getForwardedStatus() {
    return forwardedStatus;
}
