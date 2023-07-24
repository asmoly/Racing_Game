#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"

#include "Vector.h"
#include "Car.h"

class Window
{
public:
    Window() = default;
    Window(const Vector& window_dimensions, const int& max_num_of_cars, const int& pixels_per_meter);

    bool update();

    int create_car(const Car& car);
    void update_car(const Car& car, const int& id);

public:
    float delta_time;

private:
    void draw();

private:
    sf::RenderWindow window;
    sf::Texture car_texture;
    
    sf::Clock clock;

    sf::Sprite* cars;
    int car_counter;
    int max_num_of_cars;
    int pixels_per_meter;
    float car_scale;
};