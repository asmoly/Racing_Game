#include <iostream>

#include "Network_Manager.h"
#include "Vector.h"

Network_Manager::Network_Manager(const int& port)
{
    if (this->socket.bind(port) != sf::Socket::Done)
    {
        std::cout << "Could not connect to socket" << std::endl;
    }

    this->port = port;
}

sf::Packet Network_Manager::listen()
{
    sf::Packet packet;

    unsigned short port;
    sf::IpAddress sender;

    if (socket.receive(packet, sender, port) != sf::Socket::Done)
    {
        std::printf("Error recieving data");
    }

    packet << sender.toString();

    return packet;
}

void Network_Manager::send_data(const float& x, const float& y, const float& rotation, const std::string& address)
{
    sf::Packet packet;
    packet << 0 << x << y << rotation;

    sf::IpAddress recipient = address;
    unsigned short port = this->port;
    
    if (socket.send(packet, recipient, port) != sf::Socket::Done)
    {
        std::cout << "Error sending data" << std::endl;
    }
}

void Network_Manager::connect(const std::string& address)
{
    sf::Packet packet;
    packet << 1;

    sf::IpAddress recipient = address;
    unsigned short port = this->port;
    
    if (socket.send(packet, recipient, port) != sf::Socket::Done)
    {
        std::cout << "Error sending data" << std::endl;
    }
}

void Network_Manager::send_client_info(const int& num_of_clients,std::string* clients, const std::string& address)
{
    sf::Packet packet;
    packet << 2 << num_of_clients;
    for (int i = 0; i < num_of_clients; i++)
    {
        packet << clients[i];
    }

    sf::IpAddress recipient = address;
    unsigned short port = this->port;
    
    if (socket.send(packet, recipient, port) != sf::Socket::Done)
    {
        std::cout << "Error sending data" << std::endl;
    }
}