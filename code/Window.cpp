#include <iostream>
#include <math.h>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"

#include "Vector.h"
#include "Window.h"

Window::Window(const Vector& window_dimensions, const int& max_num_of_cars)
{
    this->window.create(sf::VideoMode(window_dimensions.x, window_dimensions.y), "Racing Game");

    this->cars = new sf::Sprite[max_num_of_cars];

    this->car_counter = 0;
    this->max_num_of_cars = max_num_of_cars;

    this->car_texture.loadFromFile("cars/race_car.png");

    this->delta_time = this->clock.restart().asSeconds();

    this->player_car.setTexture(this->car_texture);
    this->player_car.setOrigin(sf::Vector2f(this->car_texture.getSize().x/2.0f, this->car_texture.getSize().y/2.0f));

    if (!this->font.loadFromFile("fonts/arial.ttf"))
    {
        std::cout << "Error loading font" << std::endl;
    }

    this->fps_text.setFont(font);
    this->fps_text.setCharacterSize(30);
    this->fps_text.setFillColor(sf::Color::Red);
    this->fps_text.setPosition(sf::Vector2f(10.0f, 10.0f));

    this->map_scale = 0.0f;
}

void Window::load_map(const std::string& path_to_map_background, const float& pixels_per_meter)
{
    this->map_texture.loadFromFile(path_to_map_background);
    this->map_sprite.setTexture(this->map_texture);

    this->pixels_per_meter = pixels_per_meter;
    this->car_scale = 6.0f*pixels_per_meter/this->car_texture.getSize().x;
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

std::string Window::start_screen()
{
    bool start_game = false;

    sf::String code;

    sf::Text code_text;
    code_text.setFont(font);
    code_text.setCharacterSize(30);
    code_text.setFillColor(sf::Color::Blue);
    code_text.setPosition(sf::Vector2f(250.0f, 115.0f));

    sf::Text status_text;
    status_text.setFont(font);
    status_text.setCharacterSize(50);
    status_text.setFillColor(sf::Color::Red);
    status_text.setPosition(sf::Vector2f(500.0f, 115.0f));
    status_text.setString("Loading...");

    sf::Text guiding_text;
    guiding_text.setFont(font);
    guiding_text.setCharacterSize(30);
    guiding_text.setFillColor(sf::Color::Black);
    guiding_text.setPosition(sf::Vector2f(100.0f, 50.0f));
    guiding_text.setString("If hosting type ?MAP_NAME/PORT\nIf joining type HOST_IP/PORT\nEnter code:");

    sf::Text maps_text;
    maps_text.setFont(font);
    maps_text.setCharacterSize(30);
    maps_text.setFillColor(sf::Color::Green);
    maps_text.setPosition(sf::Vector2f(100.0f, 200.0f));
    maps_text.setString("Maps:\nmoscow\nsaint_petersburg");

    while (start_game == false)
    {
        sf::Event event;
        while (this->window.pollEvent(event))
        {
            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode == 8)
                {
                    if (code.getSize() != 0)
                    {
                        code.erase(code.getSize() - 1, 1);
                    }
                }
                else if (event.text.unicode == 13)
                {
                    this->window.draw(status_text);
                    this->window.display();

                    start_game = true;
                    std::string code_as_string = code.toAnsiString();
                    return code_as_string;
                }
                else
                {
                    code += event.text.unicode;
                }
                
                code_text.setString(code);
            }

            if (event.type == sf::Event::Closed)
            {
                start_game = true;
                this->window.close();
            }
        }

        this->window.clear(sf::Color::White);

        this->window.draw(guiding_text);
        this->window.draw(code_text);
        this->window.draw(maps_text);

        this->window.display();
    }

    return (std::string("no data"));
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
    // Updates client car relative to camera and player position
    Vector difference_in_pos;
    difference_in_pos.x = pos.x - player_car_pos.x;
    difference_in_pos.y = pos.y - player_car_pos.y;

    this->cars[car_id].setPosition(sf::Vector2f(window.getSize().x/2 + difference_in_pos.x*this->map_scale, window.getSize().y/2 + difference_in_pos.y*this->map_scale));
    this->cars[car_id].setRotation(rotation);
    this->cars[car_id].setScale(sf::Vector2f(this->car_scale*this->map_scale, this->car_scale*this->map_scale));
}

void Window::update_player_car(const Vector& pos, const float& rotation, const float& map_zoom)
{
    // Updates the position of player car and camera
    this->player_car.setPosition(sf::Vector2f(window.getSize().x/2.0f, window.getSize().y/2.0f));
    this->player_car.setRotation(rotation);

    float x_zoom = map_zoom;
    float y_zoom = map_zoom*(float(this->window.getSize().y)/float(this->window.getSize().x));

    float x = pos.x - window.getSize().x/2 + x_zoom;
    float y = pos.y - window.getSize().y/2 + y_zoom;
    float width = window.getSize().x - x_zoom*2;
    float height = window.getSize().y - y_zoom*2;

    x = fmin(fmax(0.0f, x), this->map_texture.getSize().x - width);
    y = fmin(fmax(0.0f, y), this->map_texture.getSize().y - height);

    if (x == 0 || x == this->map_texture.getSize().x - width)
    {
        this->player_car.setPosition(sf::Vector2f((pos.x - x)*this->map_scale, window.getSize().y/2.0f));
    }
    if (y == 0 || y == this->map_texture.getSize().y - height)
    {
        this->player_car.setPosition(sf::Vector2f(this->player_car.getPosition().x, (pos.y - y)*this->map_scale));
    }

    this->map_sprite.setTextureRect(sf::IntRect(x, y, width, height));

    this->map_scale = this->window.getSize().x/width;

    this->map_sprite.setScale(sf::Vector2f(this->map_scale, this->map_scale));
    this->map_sprite.setPosition(sf::Vector2f(0.0f, 0.0f));
    this->player_car.setScale(sf::Vector2f(this->car_scale*this->map_scale, this->car_scale*this->map_scale));
}

void Window::draw()
{
    this->window.draw(this->map_sprite);
    this->window.draw(this->player_car);

    for (int i = 0; i < this->car_counter; i++)
    {
        this->window.draw(this->cars[i]);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
    {
        this->fps_text.setString(std::to_string(int(1.0f/this->delta_time)));
        this->window.draw(fps_text);
    }
}