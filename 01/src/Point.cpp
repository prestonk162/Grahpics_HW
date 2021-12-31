#include "Point.h"

Point3f::Point3f()
{
    position[0] = 0;
    position[1] = 0;
    position[2] = 0;
}

Point3f::Point3f(const GLfloat& x, const GLfloat& y, const GLfloat& z)
{
    position[0] = x;
    position[1] = y;
    position[2] = z;
}

Point3f::~Point3f()
{
}

void Point3f::setPosition(const GLfloat& x, const GLfloat& y, const GLfloat& z)
{
    position[0] = x;
    position[1] = y;
    position[2] = z;
}

Point3f::Point3f(const Point3f& p)
{
    position[0] = p[0];
    position[1] = p[1];
    position[2] = p[2];
}

GLfloat& Point3f::operator [] (const int i)
{
    return position[i];
}

GLfloat Point3f::operator [] (const int i) const
{
    return position[i];
}

Point3f& Point3f::operator += (const Vector3f& v)
{
    position[0] += v[0];
    position[1] += v[1];
    position[2] += v[2];
    return (*this);
}

Point3f& Point3f::operator = (const Point3f& p)
{
    setPosition(p[0], p[1], p[2]);
    return (*this);
}

Point3f operator+(const Point3f& p, const Vector3f& v)
{
    return Point3f(p[0] + v[0], p[1] + v[1], p[2] + v[2]);
}

Vector3f operator-(const Point3f& p1, const Point3f& p2)
{
    return Vector3f(p1[0] - p2[0], p1[1] - p2[1], p1[2] - p2[2]);
}