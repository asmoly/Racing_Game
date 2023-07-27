#include <math.h>

#include "Car.h"
#include "Vector.h"
#include "Matrix.h"

Car::Car(const float& acceleration, const float& top_speed, const float& brake_force, const float& friction, const float& max_steering_angle, const Vector& starting_pos, const int& pixels_per_meter)
{
    this->acceleration = acceleration;
    this->top_speed = top_speed;
    this->brake_force = brake_force;
    this->friction = friction;
    this->max_steering_angle = max_steering_angle;
    this->pixels_per_meter = pixels_per_meter;
    
    this->speed = 0.0f;
    this->rotation = 0.0f;
    this->position = starting_pos;
}

void Car::update_pos(const float& acceleration, const float& steering, const float& delta_time)
{
    float rotation_in_rad = (this->rotation*3.1415f)/180.0f;
    Vector direction_vector(cosf(rotation_in_rad), sinf(rotation_in_rad));

    float road_friction = this->friction*this->speed;

    if (acceleration > 0.0f)
    {
        this->speed += (this->acceleration*acceleration)*delta_time;
    }
    else
    {
        this->speed += (this->brake_force*acceleration)*delta_time;
    }

    this->speed = fmin(fmax(0, this->speed), this->top_speed);

    Vector power_vector = Vector::scale(direction_vector, this->speed);

    Matrix steering_rotation_matrix = Matrix::create_rotation_matrix((90.0f*steering + this->max_steering_angle*steering));
    Vector steering_vector = Matrix::multiply(steering_rotation_matrix, direction_vector);
    steering_vector = Vector::scale(steering_vector, road_friction*delta_time);

    Vector total_vector = Vector::add(power_vector, steering_vector);
    total_vector = Vector::scale(total_vector, delta_time*pixels_per_meter);

    if (this->speed != 0.0f)
    {
        rotation_in_rad = atan2(total_vector.y, total_vector.x);
        this->rotation = (rotation_in_rad*180.0f)/3.1415f;
    }

    this->position = Vector::add(this->position, total_vector);
}