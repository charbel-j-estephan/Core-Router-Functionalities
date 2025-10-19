#include "qoservice.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

QoService::QoService(int maxSize)
    : highCount(0), mediumCount(0), lowCount(0), maxQueueSize(maxSize), forwardedStatus(false) {
}

vector<packets> QoService::readPacketsFromFile(const string& filename) {
    vector<packets> packetList;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Cannot open file " << filename << endl;
        return packetList;
    }

    string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
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
    cout << "Loaded " << packetList.size() << " packets" << endl;
    return packetList;
}

void QoService::classifyPackets(const vector<packets>& packetVec) {
    for (size_t i = 0; i < packetVec.size(); i++) {
        packets packet = packetVec[i];
        int port = packet.getPort();

        if (port >= 0 && port <= 1023) {
            packet.setPriority("High");
            if (highCount < maxQueueSize) {
                highPriorityQueue.push(packet);
                highCount++;
            }
        }
        else if (port >= 1024 && port <= 49151) {
            packet.setPriority("Medium");
            if (mediumCount < maxQueueSize) {
                mediumPriorityQueue.push(packet);
                mediumCount++;
            }
        }
        else {
            packet.setPriority("Low");
            if (lowCount < maxQueueSize) {
                lowPriorityQueue.push(packet);
                lowCount++;
            }
        }
    }
}

packets QoService::getNextPacket() {
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

    return packets();
}

void QoService::forwardOrDrop() {
    int forwarded = 0;
    int dropped = 0;

    while (!allQueuesEmpty()) {
        packets packet = getNextPacket();

        if (packet.getId() == 0) {
            break;
        }

        packet.decrementTTL();

        if (packet.getTTL() > 0) {
            setForwardedStatus(true);
            cout << "FORWARDED: ";
            packet.display();
            forwarded++;
        }
        else {
            cout << "DROPPED: Packet " << packet.getId() << " (TTL expired)" << endl;
            setForwardedStatus(false);
            dropped++;
        }
    }

    cout << "Forwarded: " << forwarded << ", Dropped: " << dropped << endl;
}

bool QoService::allQueuesEmpty() const {
    return highPriorityQueue.empty() && mediumPriorityQueue.empty() && lowPriorityQueue.empty();
}

void QoService::displayQueueStatus() const {
    cout << "Queue Status - High:" << highCount
        << " Medium:" << mediumCount
        << " Low:" << lowCount << endl;
}

void QoService::setForwardedStatus(bool ft) {
    forwardedStatus = ft;
}

bool QoService::getForwardedStatus() {
    return forwardedStatus;
}