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
    std::cout << "test" << std::endl;
}