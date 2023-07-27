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
    Window(const Vector& window_dimensions, const int& max_num_of_cars);

    bool update();
    std::string start_screen();
    void load_map(const std::string& path_to_map_background, const float& pixels_per_meter);

    int create_car(const Car& car);
    void update_car(const Vector& pos, const float& rotation, const int& car_id, const Vector& player_car_pos);

    void update_player_car(const Vector& pos, const float& rotation);

public:
    float delta_time;
    sf::RenderWindow window;

private:
    void draw();

private:
    sf::Texture car_texture;
    
    sf::Texture map_texture;
    sf::Sprite map_sprite;
    
    sf::Clock clock;

    sf::Sprite* cars;
    sf::Sprite player_car;

    sf::Font font;
    sf::Text fps_text;

    int car_counter;
    int max_num_of_cars;
    int pixels_per_meter;
    float car_scale;
};