#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

#include "Map.h"

Map::Map(const std::string& path_to_map)
{
    this->path_to_collision_mask = NAN;
    this->path_to_background = NAN;
    this->pixels_per_meter = 5.0f;
    this->starting_pos = Vector(0, 0);

    std::stringstream path_to_info;
    path_to_info << path_to_map << "info.txt";

    std::ifstream file(path_to_info.str());
    std::string line;
    while (std::getline(file, line))
    {
        if (line == std::string("path_to_background"))
        {
            std::getline(file, line);

            std::stringstream path_to_background;
            path_to_background << path_to_map << line;
            this->path_to_background = path_to_background.str();
        }
        else if (line == std::string("path_to_collision_mask"))
        {
            std::getline(file, line);

            std::stringstream path_to_collision_mask;
            path_to_collision_mask << path_to_map << line;
            this->path_to_collision_mask = path_to_collision_mask.str();
        }
        else if (line == "pixels_per_meter")
        {
            std::getline(file, line);
            this->pixels_per_meter = std::stof(line);
        }
        else if (line == "starting_x")
        {
            std::getline(file, line);
            this->starting_pos.x = std::stof(line);
        }
        else if (line == "starting_y")
        {
            std::getline(file, line);
            this->starting_pos.y = std::stof(line);
        }
    }
}