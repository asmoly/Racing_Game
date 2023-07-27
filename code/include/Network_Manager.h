#pragma once

#include "SFML/Network.hpp"

class Network_Manager
{
public:
    Network_Manager() = default;
    Network_Manager(const int& port);

    void send_data(const float& x, const float& y, const float& rotation, const std::string& address);
    void connect(const std::string& address);
    void send_client_info(const int& num_of_clients, std::string* clients, const std::string& address, const std::string& map_name);
    
    sf::Packet listen();

private:
    sf::UdpSocket socket;
    int port;
};