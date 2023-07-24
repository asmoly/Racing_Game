#include <iostream>

#include "Network_Manager.h"

Network_Manager::Network_Manager(const int& port)
{
    if (this->socket.bind(port) != sf::Socket::Done)
    {
        std::cout << "Could not connect to socket" << std::endl;
    }

    this->port = port;
}

float* Network_Manager::listen()
{
    float* data = new float[3];
    sf::Packet packet;

    unsigned short port;
    sf::IpAddress sender;

    if (socket.receive(packet, sender, port) != sf::Socket::Done)
    {
        std::printf("Error recieving data");
    }

    for (int i = 0; i < 3; i++)
    {
        packet >> data[i];
    }

    return data;
}

void Network_Manager::send_data(const float& x, const float& y, const float& rotation, const std::string& address)
{
    float data[3] = {x, y, rotation};
    sf::Packet packet;
    packet << data;

    sf::IpAddress recipient = address;
    unsigned short port = this->port;
    
    if (socket.send(packet, recipient, port) != sf::Socket::Done)
    {
        std::cout << "Error sending data" << std::endl;
    }
}