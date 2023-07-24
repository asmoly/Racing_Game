#pragma once

#include <iostream>

class Vector
{
public:
    Vector() = default;
    Vector(const float& x, const float& y);

    static float dot_product(const Vector& vector_a, const Vector& vector_b);
    static Vector normalize(const Vector& vector_to_normalize);
    static Vector scale(const Vector& vector_to_scale, const float& scalar);
    static Vector add(const Vector& vector_a, const Vector& vector_b);
    std::string to_string();

public:
    float x;
    float y;
};