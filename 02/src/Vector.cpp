#include "Vector.h"

Vector3f::Vector3f()
{
    direction[0] = 0;
    direction[1] = 0;
    direction[2] = 0;
}


Vector3f::Vector3f(const GLfloat& x, const GLfloat& y, const GLfloat& z)
{
    direction[0] = x;
    direction[1] = y;
    direction[2] = z;
}

Vector3f::~Vector3f()
{
}

void Vector3f::setDirection(const GLfloat& x, const GLfloat& y, const GLfloat& z)
{
    direction[0] = x;
    direction[1] = y;
    direction[2] = z;
}

Vector3f::Vector3f(const Vector3f& v)
{
    direction[0] = v[0];
    direction[1] = v[1];
    direction[2] = v[2];
}

GLfloat& Vector3f::operator [] (const int i)
{
    return direction[i];
}

GLfloat Vector3f::operator [] (const int i) const
{
    return direction[i];
}

Vector3f& Vector3f::operator += (const Vector3f& v)
{
    direction[0] += v[0];
    direction[1] += v[1];
    direction[2] += v[2];
    return (*this);
}

Vector3f& Vector3f::operator -= (const Vector3f& v)
{
    direction[0] -= v[0];
    direction[1] -= v[1];
    direction[2] -= v[2];
    return (*this);
}

Vector3f& Vector3f::operator *= (const GLfloat& s)
{
    direction[0] *= s;
    direction[1] *= s;
    direction[2] *= s;
    return (*this);
}
Vector3f& Vector3f::operator /= (const GLfloat& s)
{
    direction[0] /= s;
    direction[1] /= s;
    direction[2] /= s;
    return (*this);
}

Vector3f& Vector3f::operator = (const Vector3f& v)
{
    setDirection(v[0], v[1], v[2]);
    return (*this);
}

Vector3f operator+(const Vector3f& v1, const Vector3f& v2)
{
    return Vector3f(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
}

Vector3f operator-(const Vector3f& v1, const Vector3f& v2)
{
    return Vector3f(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
}

Vector3f operator*(const float s, const Vector3f& vec3)
{
    return Vector3f(vec3[0] * s, vec3[1] * s, vec3[2] * s);
}

Vector3f operator/(const Vector3f& vec3, const float s)
{
    return Vector3f(vec3[0] / s, vec3[1] / s, vec3[2] / s);
}

GLfloat dotProduct(const Vector3f& v1, const Vector3f& v2)
{
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}