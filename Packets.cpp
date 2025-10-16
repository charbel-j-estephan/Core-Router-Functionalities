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
packets::packets(int id, string source, string destination, int port, int TTL) {
	this->id = id;
	this->source = source;
	this->destination = destination;
	this->port = port;
	this->TTL = TTL;
	priority = "";
}

int packets::getId() {
	return id;
}
string packets::getSource() {
	return source;
}

string packets::getDestination() {
	return destination;
}

int packets::getPort() {
	return port;
}

int packets::getTTL() {
	return TTL;
}


void packets::setPriority(string priority) {
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

void packets::display() {
	cout << "Packet ID: " << id
		<< "\nSource: " << source
		<< "\nDestination: " << destination
		<< "\nPort: " << port
		<< "\nTTL: " << TTL
		<< "\nPriority: " << ((priority == "") ? "Not_Assigned" : priority) << "\n";
}