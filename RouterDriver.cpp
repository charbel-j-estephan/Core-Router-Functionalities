#include "RouterDriver.h"
#include <iostream>
#include <vector>

using namespace std;

RouterDriver::RouterDriver(const string& filename, int maxQueueSize)
    : inputFile(filename) {
    qos = new QoService(maxQueueSize);
    routingTable = new RoutingTable();
}

RouterDriver::~RouterDriver() {
    cleanup();
}

void RouterDriver::initializeComponents() {
    cout << "Initializing Router Components..." << endl;
}

void RouterDriver::configureRoutingTable() {
    cout << "Configuring Routing Table..." << endl;

    routingTable->addRoute("192.168.1.0", 24, "Router_A", 1);
    routingTable->addRoute("192.168.2.0", 24, "Router_B", 1);
    routingTable->addRoute("192.168.0.0", 16, "Router_C", 2);
    routingTable->addRoute("10.0.0.0", 8, "Router_D", 3);
    routingTable->addRoute("172.16.0.0", 12, "Router_E", 2);
    routingTable->addRoute("172.20.0.0", 16, "Router_F", 1);
    routingTable->addRoute("0.0.0.0", 0, "DefaultGateway", 10);

    routingTable->displayRoutingTable();
}

void RouterDriver::processPackets() {
    cout << "\nReading packets from file..." << endl;

    vector<packets> packetList = qos->readPacketsFromFile(inputFile);

    if (packetList.empty()) {
        cout << "Error: No packets loaded. Exiting..." << endl;
        return;
    }

    cout << "Classifying packets by QoS..." << endl;
    qos->classifyPackets(packetList);
    qos->displayQueueStatus();

    cout << "\nProcessing packets..." << endl;

    int forwardedCount = 0;
    int droppedTTL = 0;
    int droppedNoRoute = 0;
    int packetNumber = 1;

    while (!qos->allQueuesEmpty()) {
        packets packet = qos->getNextPacket();

        if (packet.getId() == 0) {
            break;
        }

        cout << "\nPacket " << packetNumber << " [ID:" << packet.getId()
            << "] " << packet.getSource() << " -> " << packet.getDestination()
            << " TTL:" << packet.getTTL();

        packet.decrementTTL();

        if (packet.getTTL() <= 0) {
            cout << " [DROPPED - TTL Expired]" << endl;
            droppedTTL++;
            packetNumber++;
            continue;
        }

        RouterEntry* route = routingTable->findBestRoute(packet.getDestination());

        if (route != nullptr) {
            cout << " [FORWARDED to " << route->getNextHop() << "]" << endl;
            qos->setForwardedStatus(true);
            forwardedCount++;
        }
        else {
            cout << " [DROPPED - No Route]" << endl;
            qos->setForwardedStatus(false);
            droppedNoRoute++;
        }

        packetNumber++;
    }

    cout << "\n--- Processing Summary ---" << endl;
    cout << "Total Packets: " << (packetNumber - 1) << endl;
    cout << "Forwarded: " << forwardedCount << endl;
    cout << "Dropped (TTL): " << droppedTTL << endl;
    cout << "Dropped (No Route): " << droppedNoRoute << endl;
}

void RouterDriver::displayStatistics() {
    cout << "\n--- Final Statistics ---" << endl;
    cout << "Total Routes: " << routingTable->getRouteCount() << endl;
    qos->displayQueueStatus();
}

void RouterDriver::cleanup() {
    delete qos;
    delete routingTable;
}

void RouterDriver::run() {
    cout << "========================================" << endl;
    cout << "     ROUTER SIMULATION SYSTEM" << endl;
    cout << "========================================" << endl;

    initializeComponents();
    configureRoutingTable();
    processPackets();
    displayStatistics();

    cout << "\n========================================" << endl;
    cout << "     SIMULATION COMPLETED" << endl;
    cout << "========================================" << endl;
}