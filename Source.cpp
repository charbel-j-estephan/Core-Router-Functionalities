#include <iostream>
#include <vector>
#include "qoservice.h"
#include "packets.h"

using namespace std;

int main() {
    cout << "==================================" << endl;
    cout << "   QoS Packet Management System   " << endl;
    cout << "==================================" << endl;

    // Create QoService with max queue size of 10
    QoService qos(10);

    cout << "\n[Step 1] Reading packets from file..." << endl;
    vector<packets> packetList = qos.readPacketsFromFile("file.txt");

    if (packetList.empty()) {
        cout << "No packets read. Exiting..." << endl;
        return 1;
    }

    // Display all packets before classification
    cout << "\n[Step 2] Displaying all packets before classification:" << endl;
    cout << "------------------------------------------------------" << endl;
    for (size_t i = 0; i < packetList.size(); i++) {
        packetList[i].display();
    }

    // Classify and enqueue packets
    cout << "\n[Step 3] Classifying and enqueueing packets..." << endl;
    qos.classifyPackets(packetList);

    // Display queue status
    cout << "\n[Step 4] Current queue status:" << endl;
    qos.displayQueueStatus();

    // Process all packets
    cout << "\n[Step 5] Processing packets (Forward or Drop)..." << endl;
    qos.forwardOrDrop();

    // Check final queue status
    cout << "\n[Step 6] Final queue status:" << endl;
    qos.displayQueueStatus();

    if (qos.allQueuesEmpty()) {
        cout << "\nAll queues are empty. Processing complete!" << endl;
    }
    else {
        cout << "\nWarning: Some queues still have packets!" << endl;
    }

    cout << "\n==================================" << endl;
    cout << "   Program Terminated Successfully" << endl;
    cout << "==================================" << endl;

    return 0;
}