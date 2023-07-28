#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>
#include "SFML/System.hpp"
#include "SFML/Network.hpp"

#include "Vector.h"
#include "Window.h"
#include "Car.h"
#include "Network_Manager.h"
#include "Map.h"

std::string wait_for_Connection(Network_Manager& network_manager, std::string* clients, int& clients_count, Window& window, const std::string& ip)
{
    network_manager.connect(ip);
    
    int time_for_connection = 500;
    for (int i = 0; i < time_for_connection; i++)
    {
        sf::Packet data = network_manager.listen();

        int status;
        std::string address;
        std::string map_name;

        data >> status;

        if (status == 2)
        {
            data >> clients_count;
            for (int i = 0; i < clients_count; i++)
            {
                data >> clients[i];
            }

            data >> map_name;
            data >> address;

            clients[clients_count] = address;
            clients_count ++;

            return map_name;
        }
    }

    return std::string("no data");
}

void recieve_and_send_data(Network_Manager& network_manager, Window& window, Car default_car, std::string* clients, int& clients_count, Car& player_car, const std::string& map_name)
{
    for (int i = 0; i < clients_count; i++)
    {
        window.create_car(default_car);
    }

    while (true)
    {
        sf::Packet data = network_manager.listen();

        int status;
        std::string address;

        data >> status;

        if (status == 0) // 0 is just sending data about car
        {
            float x;
            float y;
            float rotation;

            data >> x >> y >> rotation >> address;

            for (int i = 0; i < clients_count; i++)
            {
                if (clients[i] == address)
                {
                    window.update_car(Vector(x, y), rotation, i, player_car.position);
                    break;
                }
            }
        }
        else if (status == 1)
        {
            data >> address;
            network_manager.send_client_info(clients_count, clients, address, map_name);

            window.create_car(default_car);

            clients[clients_count] = address;
            clients_count ++;
        }
    }
}

int main()
{
    Window window(Vector(1200, 800), 10);
    std::string code = window.start_screen();

    // Ip also acts as map name if hosting
    std::string ip;
    std::string port_as_string;
    int split_mark = code.find("/");
    ip = code.substr(0, split_mark);
    port_as_string = code.substr(split_mark + 1, code.length());
    int port = std::stoi(port_as_string);

    Map map;
    std::string map_name;
    std::stringstream path_to_map;

    bool host = false;
    if (ip.find("?") == 0)
    {
        ip.erase(0, 1);
        host = true;
        path_to_map << "maps/" << ip << "/";

        map = Map(path_to_map.str());
        map_name = ip;
    }

    Network_Manager network_manager(port);

    std::string* clients = new std::string[10];
    int clients_count = 0;

    if (host == false)
    {
        std::cout << "connecting" << std::endl;
        map_name = wait_for_Connection(network_manager, clients, clients_count, window, ip);
        map = Map(path_to_map.str());
    }

    window.load_map(map.path_to_background, map.pixels_per_meter);

    Car car(30, 70, 40, 1.5, 20, Vector(map.starting_pos.x, map.starting_pos.y), map.pixels_per_meter);

    std::thread listen_thread(recieve_and_send_data, std::ref(network_manager), std::ref(window), car, std::ref(clients), std::ref(clients_count), std::ref(car), map_name);

    float zoom = 0.0f;

    bool window_open = true;
    while (window_open == true)
    {
        for (int i = 0; i < clients_count; i++)
        {
            network_manager.send_data(car.position.x, car.position.y, car.rotation, std::string(clients[i]));
        }

        float acceleration = 0;
        float steering = 0;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            acceleration = 1.0f;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            acceleration = -1.0f;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            steering = -1;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            steering = 1;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            zoom += 10.0;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            zoom -= 10.0;
        }

        car.update_pos(acceleration, steering, window.delta_time);
        window.update_player_car(car.position, car.rotation, zoom);

        window_open = window.update();
    }
}