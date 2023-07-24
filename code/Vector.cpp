#include <string>
#include "math.h"

#include "vector.h"

Vector::Vector(const float& x, const float& y)
{
    this->x = x;
    this->y = y;
}

float Vector::dot_product(const Vector& vector_a, const Vector& vector_b)
{
    return (vector_a.x*vector_b.x + vector_a.y*vector_b.y);
}

Vector Vector::normalize(const Vector& vector_to_normalize)
{
    float vector_length = sqrtf(vector_to_normalize.x*vector_to_normalize.x + vector_to_normalize.y*vector_to_normalize.y);
    
    if (vector_length != 0)
    {
        return Vector(vector_to_normalize.x/vector_length, vector_to_normalize.y/vector_length);
    }

    return Vector(0.0f, 0.0f);
}

Vector Vector::scale(const Vector& vector_to_scale, const float& scalar)
{
	return Vector(vector_to_scale.x*scalar, vector_to_scale.y*scalar);
}

Vector Vector::add(const Vector& vector_a, const Vector& vector_b)
{
	return Vector(vector_a.x + vector_b.x, vector_a.y + vector_b.y);
}

std::string Vector::to_string()
{
	return (std::to_string(this->x) + ", " + std::to_string(this->y));
}