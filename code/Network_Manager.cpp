#include <iostream>

#include "Network_Manager.h"

Network_Manager::Network_Manager(const int& port)
{
    if (this->socket.bind(port) != sf::Socket::Done)
    {
        std::cout << "Could not connect to socket" << std::endl;
    }
}

void Network_Manager::listen()
{
    int data[5];
    std::size_t received;

    // UDP socket:
    sf::IpAddress sender;
    unsigned short port;
    if (socket.receive(data, 5, received, sender, port) != sf::Socket::Done)
    {
        std::cout << "Error recieving data" << std::endl;
    }
    std::cout << "Received " << received << " bytes from " << sender << " on port " << port << std::endl;
}

void Network_Manager::send_data()
{
    int data[5] = {4, 1, 5, 2, 3};
    sf::IpAddress recipient = "192.168.1.61";
    unsigned short port = 9958;
    
    if (this->socket.send(data, 5, recipient, port) != sf::Socket::Done)
    {
        // error...
    }
}