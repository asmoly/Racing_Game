#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"

#include "Vector.h"
#include "Window.h"

Window::Window(const Vector& window_dimensions, const int& max_num_of_cars, const int& pixels_per_meter, const std::string& path_to_map_background)
{
    this->window.create(sf::VideoMode(window_dimensions.x, window_dimensions.y), "Racing Game");

    this->cars = new sf::Sprite[max_num_of_cars];

    this->car_counter = 0;
    this->max_num_of_cars = max_num_of_cars;
    this->pixels_per_meter = pixels_per_meter;

    this->car_texture.loadFromFile("cars/race_car.png");
    this->car_scale = 3.0f*pixels_per_meter/this->car_texture.getSize().x;

    this->map_texture.loadFromFile(path_to_map_background);
    this->map_sprite.setTexture(this->map_texture);

    this->delta_time = this->clock.restart().asSeconds();

    this->player_car.setTexture(this->car_texture);
    this->player_car.setOrigin(sf::Vector2f(this->car_texture.getSize().x/2.0f, this->car_texture.getSize().y/2.0f));
    this->player_car.setScale(sf::Vector2f(this->car_scale, this->car_scale));
}

bool Window::update()
{
    this->delta_time = this->clock.restart().asSeconds();

    sf::Event event;
    while (this->window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            this->window.close();
    }

    this->window.clear(sf::Color::White);

    this->draw();

    this->window.display();

    return window.isOpen();
}

int Window::create_car(const Car& car)
{
    sf::Sprite car_sprite;
    car_sprite.setTexture(this->car_texture);
    car_sprite.setOrigin(sf::Vector2f(this->car_texture.getSize().x/2.0f, this->car_texture.getSize().y/2.0f));
    car_sprite.setScale(sf::Vector2f(this->car_scale, this->car_scale));

    car_sprite.setPosition(sf::Vector2f(car.position.x, -car.position.y));
    car_sprite.setRotation(car.rotation);

    this->cars[this->car_counter] = car_sprite;

    this->car_counter ++;
    return this->car_counter - 1;
}

void Window::update_car(const Vector& pos, const float& rotation, const int& car_id, const Vector& player_car_pos)
{
    //this->cars[car_id].setPosition(sf::Vector2f(pos.x, pos.y));
    Vector difference_in_pos;
    difference_in_pos.x = pos.x - player_car_pos.x;
    difference_in_pos.y = pos.y - player_car_pos.y;

    this->cars[car_id].setPosition(sf::Vector2f(window.getSize().x/2 + difference_in_pos.x, window.getSize().y/2 + difference_in_pos.y));
    this->cars[car_id].setRotation(rotation);
}

void Window::update_player_car(const Vector& pos, const float& rotation)
{
    this->player_car.setPosition(sf::Vector2f(window.getSize().x/2.0f, window.getSize().y/2.0f));
    //this->player_car.setPosition(sf::Vector2f(pos.x, pos.y));
    this->player_car.setRotation(rotation);

    float x = pos.x - window.getSize().x/2;
    float y = pos.y - window.getSize().y/2;
    float width = window.getSize().x;
    float height = window.getSize().y;

    this->map_sprite.setTextureRect(sf::IntRect(x, y, width, height));
}

void Window::draw()
{
    this->window.draw(this->map_sprite);
    this->window.draw(this->player_car);

    for (int i = 0; i < this->car_counter; i++)
    {
        this->window.draw(this->cars[i]);
    }
}