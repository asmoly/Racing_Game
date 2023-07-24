#pragma once

#include "Vector.h"

class Matrix
{
public:
    Matrix() = default;
    Matrix(const Vector& vector_a, const Vector& vector_b);

    static Matrix create_rotation_matrix(const float& angle);
    static Vector multiply(const Matrix& matrix, const Vector& vector);

public:
    Vector vector_a;
    Vector vector_b;
};