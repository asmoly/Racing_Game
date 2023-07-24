#pragma once

#include "Vector.h"
#include "Matrix.h"

class Car
{
public:
    Car() = default;
    Car(const float& acceleration, const float& top_speed, const float& brake_force, const float& friction, const float& max_steering_angle, const Vector& starting_pos, const int& pixels_per_meter);

    void update_pos(const float& acceleration, const float& steering, const float& delta_time);

public:
    Vector position;
    float rotation;
    float speed;

    float acceleration;
    float top_speed;
    float brake_force;
    float friction;
    float max_steering_angle;
    int pixels_per_meter;
};