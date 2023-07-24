#pragma once

#include "SFML/Network.hpp"

class Network_Manager
{
public:
    Network_Manager() = default;
    Network_Manager(const int& port);

    void send_data(const char*& data);
    
    void listen();

private:
    sf::UdpSocket socket;
};