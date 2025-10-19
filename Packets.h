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
    packets(int id, const std::string& source, const std::string& destination, int port, int TTL);

    int getId() const;
    std::string getSource() const;
    std::string getDestination() const;
    int getPort() const;
    int getTTL() const;

    void setPriority(const std::string& priority);
    void decrementTTL();
    void setTTL(int TTL);
    void display() const;
};

#endif