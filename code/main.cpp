#include <iostream>
#include <thread>
#include "SFML/System.hpp"

#include "Vector.h"
#include "Window.h"
#include "Car.h"
#include "Network_Manager.h"

void recieve_data(Network_Manager& network_manager)
{
    while (true)
    {
        network_manager.listen();
    }
}

int main()
{
    Network_Manager network_manager(9958);
    std::thread test(recieve_data, std::ref(network_manager));

    int pixels_per_meter = 10;

    Car car(15, 62, 30, 1.5, 20, Vector(300, 300), pixels_per_meter);

    Window window(Vector(1000, 700), 10, pixels_per_meter);
    int car_id = window.create_car(car);

    bool window_open = true;
    while (window_open == true)
    {
        network_manager.send_data();

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
        window.update_car(car, car_id);

        window_open = window.update();
    }
}