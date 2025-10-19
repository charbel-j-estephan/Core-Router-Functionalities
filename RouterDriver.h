#ifndef ROUTERDRIVER_H
#define ROUTERDRIVER_H

#include <string>
#include "qoservice.h"
#include "RoutingTable.h"

class RouterDriver {
private:
    QoService* qos;
    RoutingTable* routingTable;
    std::string inputFile;

    void initializeComponents();
    void configureRoutingTable();
    void processPackets();
    void displayStatistics();
    void cleanup();

public:
    RouterDriver(const std::string& filename = "file.txt", int maxQueueSize = 10);
    ~RouterDriver();

    void run();
};

#endif