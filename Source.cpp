#include <iostream>
#include <vector>
#include "qoservice.h"
#include "packets.h"
#include "RoutingTable.h"

using namespace std;

int main() {
    cout << "=============================================" << endl;
    cout << "      ROUTER SIMULATION SYSTEM              " << endl;
    cout << "   Activity 1: QoS  +  Activity 2: Routing  " << endl;
    cout << "=============================================" << endl;

    // ========================================
    // STEP 1: Initialize Components
    // ========================================
    cout << "\n[STEP 1] Initializing Router Components..." << endl;
    cout << "-------------------------------------------" << endl;

    QoService qos(10);  // Max queue size = 10
    RoutingTable routingTable;

    // ========================================
    // STEP 2: Configure Routing Table
    // ========================================
    cout << "\n[STEP 2] Configuring Routing Table..." << endl;
    cout << "-------------------------------------------" << endl;

    // Add routes for different networks
    routingTable.addRoute("192.168.1.0", 24, "Router_A", 1);
    routingTable.addRoute("192.168.2.0", 24, "Router_B", 1);
    routingTable.addRoute("192.168.0.0", 16, "Router_C", 2);
    routingTable.addRoute("10.0.0.0", 8, "Router_D", 3);
    routingTable.addRoute("172.16.0.0", 12, "Router_E", 2);
    routingTable.addRoute("172.20.0.0", 16, "Router_F", 1);
    routingTable.addRoute("0.0.0.0", 0, "DefaultGateway", 10);  // Default route

    // Display the routing table
    routingTable.displayRoutingTable();

    // ========================================
    // STEP 3: Read Packets from File
    // ========================================
    cout << "\n[STEP 3] Reading Packets from File..." << endl;
    cout << "-------------------------------------------" << endl;

    vector<packets> packetList = qos.readPacketsFromFile("file.txt");

    if (packetList.empty()) {
        cout << "ERROR: No packets read from file. Exiting..." << endl;
        return 1;
    }

    cout << "Successfully loaded " << packetList.size() << " packets." << endl;

    // ========================================
    // STEP 4: Display All Packets (Before Classification)
    // ========================================
    cout << "\n[STEP 4] Packets Before Classification:" << endl;
    cout << "-------------------------------------------" << endl;

    for (size_t i = 0; i < packetList.size(); i++) {
        cout << "\nPacket #" << (i + 1) << ":" << endl;
        packetList[i].display();
    }

    // ========================================
    // STEP 5: QoS Classification (Activity 1)
    // ========================================
    cout << "\n[STEP 5] QoS Classification (Activity 1)..." << endl;
    cout << "-------------------------------------------" << endl;

    qos.classifyPackets(packetList);
    qos.displayQueueStatus();

    // ========================================
    // STEP 6: Process Packets (QoS + Routing)
    // ========================================
    cout << "\n[STEP 6] Processing Packets (QoS + Routing)..." << endl;
    cout << "==============================================" << endl;

    int forwardedCount = 0;
    int droppedTTL = 0;
    int droppedNoRoute = 0;
    int packetNumber = 1;

    while (!qos.allQueuesEmpty()) {
        // Get next packet based on WFQ priority
        packets packet = qos.getNextPacket();

        // Check if valid packet (not default constructed)
        if (packet.getId() == 0) {
            break;
        }

        cout << "\n--- Processing Packet #" << packetNumber << " ---" << endl;
        cout << "Packet ID: " << packet.getId() << endl;
        cout << "Source: " << packet.getSource() << endl;
        cout << "Destination: " << packet.getDestination() << endl;
        cout << "Port: " << packet.getPort() << endl;
        cout << "Initial TTL: " << packet.getTTL() << endl;

        // Decrement TTL (simulating one hop)
        packet.decrementTTL();
        cout << "TTL after decrement: " << packet.getTTL() << endl;

        // Check TTL
        if (packet.getTTL() <= 0) {
            cout << "❌ STATUS: DROPPED (TTL Expired)" << endl;
            droppedTTL++;
            packetNumber++;
            continue;
        }

        // Find route for this packet (Activity 2)
        cout << "Searching for route to " << packet.getDestination() << "..." << endl;
        RouterEntry* route = routingTable.findBestRoute(packet.getDestination());

        if (route != nullptr) {
            // Route found - Forward packet
            cout << "✅ ROUTE FOUND!" << endl;
            cout << "   Network: " << route->getNetworkPrefix() << "/" << route->getPrefixLength() << endl;
            cout << "   Next Hop: " << route->getNextHop() << endl;
            cout << "   Metric: " << route->getMetric() << endl;
            cout << "✅ STATUS: FORWARDED to " << route->getNextHop() << endl;

            qos.setForwardedStatus(true);
            forwardedCount++;
        }
        else {
            // No route found - Drop packet
            cout << "❌ NO ROUTE FOUND!" << endl;
            cout << "❌ STATUS: DROPPED (No Route to Host)" << endl;

            qos.setForwardedStatus(false);
            droppedNoRoute++;
        }

        packetNumber++;
    }

    // ========================================
    // STEP 7: Final Statistics
    // ========================================
    cout << "\n=============================================" << endl;
    cout << "           FINAL STATISTICS                  " << endl;
    cout << "=============================================" << endl;

    cout << "\n📊 Packet Processing Summary:" << endl;
    cout << "   Total Packets Processed: " << (packetNumber - 1) << endl;
    cout << "   ✅ Forwarded: " << forwardedCount << endl;
    cout << "   ❌ Dropped (TTL): " << droppedTTL << endl;
    cout << "   ❌ Dropped (No Route): " << droppedNoRoute << endl;

    cout << "\n📊 Queue Status:" << endl;
    qos.displayQueueStatus();

    if (qos.allQueuesEmpty()) {
        cout << "\n✅ All queues are empty. Processing complete!" << endl;
    }
    else {
        cout << "\n⚠️  Warning: Some queues still have packets!" << endl;
    }

    cout << "\n📊 Routing Table Status:" << endl;
    cout << "   Total Routes: " << routingTable.getRouteCount() << endl;
    cout << "   Table Empty: " << (routingTable.isEmpty() ? "Yes" : "No") << endl;

    // ========================================
    // STEP 8: Additional Testing (Optional)
    // ========================================
    cout << "\n[STEP 8] Additional Route Testing..." << endl;
    cout << "-------------------------------------------" << endl;

    // Test some specific IPs
    vector<string> testIPs = {
        "192.168.1.100",
        "10.50.50.50",
        "8.8.8.8",
        "172.16.5.20"
    };

    for (const string& ip : testIPs) {
        RouterEntry* route = routingTable.findBestRoute(ip);
        cout << "\nTest IP: " << ip << endl;
        if (route != nullptr) {
            cout << "  → Route: " << route->getNetworkPrefix() << "/"
                << route->getPrefixLength() << endl;
            cout << "  → Next Hop: " << route->getNextHop() << endl;
        }
        else {
            cout << "  → No route found" << endl;
        }
    }

    // ========================================
    // Program End
    // ========================================
    cout << "\n=============================================" << endl;
    cout << "      ROUTER SIMULATION COMPLETED           " << endl;
    cout << "=============================================" << endl;

    return 0;
}