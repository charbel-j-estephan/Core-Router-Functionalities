#ifndef QOSERVICE_H
#define QOSERVICE_H

#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include "packets.h" 

class QoService {
private:
    std::queue<packets> highPriorityQueue;
    std::queue<packets> mediumPriorityQueue;
    std::queue<packets> lowPriorityQueue;

    int highCount;
    int mediumCount;
    int lowCount;

    int maxQueueSize;
    bool forwardedStatus;

public:
    QoService(int maxSize = 10);

    std::vector<packets> readPacketsFromFile(const std::string& filename);

    void classifyPackets(const std::vector<packets>& packetVec);
    void setForwardedStatus(bool ft);
    bool getForwardedStatus();

    packets getNextPacket();

    void forwardOrDrop();

    bool allQueuesEmpty() const;
    void displayQueueStatus() const;
};

#endif
