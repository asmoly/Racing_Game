#include <math.h>

#include "Matrix.h"
#include "Vector.h"

Matrix::Matrix(const Vector& vector_a, const Vector& vector_b)
{
    this->vector_a = vector_a;
    this->vector_b = vector_b;
}

Matrix Matrix::create_rotation_matrix(const float& angle)
{
    float angle_in_rad = (angle*3.1415f)/180.0f;
    return Matrix(Vector(cosf(angle_in_rad), sinf(angle_in_rad)), Vector(-sinf(angle_in_rad), cosf(angle_in_rad)));
}

Vector Matrix::multiply(const Matrix& matrix, const Vector& vector)
{
    Vector vector_to_return;
    vector_to_return.x = vector.x*matrix.vector_a.x + vector.y*matrix.vector_b.x;
    vector_to_return.y = vector.x*matrix.vector_a.y + vector.y*matrix.vector_b.y;
    return vector_to_return;
}