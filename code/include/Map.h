#pragma once

#include <iostream>
#include "Vector.h"

class Map
{
public:
    Map() = default;
    Map(const std::string& path_to_map);

public:
    std::string path_to_background;
    std::string path_to_collision_mask;
    float pixels_per_meter;
    Vector starting_pos;
    std::string name;
};