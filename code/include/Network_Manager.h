#pragma once

#include "SFML/Network.hpp"

class Network_Manager
{
public:
    Network_Manager() = default;
    Network_Manager(const int& port);

    void send_data(const float& x, const float& y, const float& rotation, const std::string& address);
    float* listen();

private:
    sf::UdpSocket socket;
    int port;
};