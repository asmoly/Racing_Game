#include <iostream>
#include <thread>
#include "SFML/System.hpp"
#include "SFML/Network.hpp"

#include "Vector.h"
#include "Window.h"
#include "Car.h"
#include "Network_Manager.h"
#include "Map.h"

void recieve_and_send_data(Network_Manager& network_manager, Window& window, Car default_car, std::string* clients, int& clients_count, Car& player_car)
{
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
            network_manager.send_client_info(clients_count, clients, address);

            window.create_car(default_car);

            clients[clients_count] = address;
            clients_count ++;
        }
        else if (status == 2)
        {
            data >> clients_count;
            for (int i = 0; i < clients_count; i++)
            {
                data >> clients[i];
                window.create_car(default_car);
            }

            data >> address;

            clients[clients_count] = address;
            window.create_car(default_car);
            clients_count ++;
        }
    }
}

int main()
{
    Map map(std::string("maps/moscow/"));

    Car car(15, 62, 30, 1.5, 20, Vector(map.starting_pos.x, map.starting_pos.y), map.pixels_per_meter);

    Window window(Vector(1200, 800), 10, map.pixels_per_meter, map.path_to_background);

    Network_Manager network_manager(9958);
    network_manager.connect("192.168.1.61");

    std::string* clients = new std::string[10];
    int clients_count = 0;
    std::thread listen_thread(recieve_and_send_data, std::ref(network_manager), std::ref(window), car, std::ref(clients), std::ref(clients_count), std::ref(car));

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

        car.update_pos(acceleration, steering, window.delta_time);
        window.update_player_car(car.position, car.rotation);

        window_open = window.update();
    }
}