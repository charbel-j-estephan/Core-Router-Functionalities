#include "Packets.h"
#include <iostream>
using namespace std;

packets::packets() {
    this->id = 0;
    this->source = "";
    this->destination = "";
    this->port = 0;
    this->TTL = 0;
    priority = "";
}

packets::packets(int id, const string& source, const string& destination, int port, int TTL) {
    this->id = id;
    this->source = source;
    this->destination = destination;
    this->port = port;
    this->TTL = TTL;
    priority = "";
}

int packets::getId() const {
    return id;
}

string packets::getSource() const {
    return source;
}

string packets::getDestination() const {
    return destination;
}

int packets::getPort() const {
    return port;
}

int packets::getTTL() const {
    return TTL;
}

void packets::setPriority(const string& priority) {
    this->priority = priority;
}

void packets::decrementTTL() {
    if (TTL > 0) {
        TTL--;
    }
}

void packets::setTTL(int TTL) {
    this->TTL = TTL;
}

void packets::display() const {
    cout << "ID:" << id << " "
        << source << "->" << destination << " "
        << "Port:" << port << " "
        << "TTL:" << TTL << " "
        << "Priority:" << ((priority == "") ? "None" : priority) << endl;
}