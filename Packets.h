#ifndef PACKETS_H
#define PACKETS_H
#include <string>

class packets {

private:
	int id;
	std::string source;
	std::string destination;
	int port;
	int TTL;
	std::string priority;

public:
	packets();
	packets(int id, std::string source, std::string destination, int port, int TTL);

	int getId();
	std::string getSource();
	std::string getDestination();
	int getPort();
	int getTTL();

	void setPriority(std::string priority);
	void decrementTTL();
	void setTTL(int TTL);
	void display();
};
#endif // !PACKETS_H
