#include "Point.h"

glm::vec3 Point3f::originPos = ZERO_VECTOR_3D;

Point3f::Point3f() : x(0.0), y(0.0), z(0.0)
{
    x = y = z = 0.0;
}

Point3f::Point3f(const GLfloat& x, const GLfloat& y, const GLfloat& z) : x(x), y(y), z(z)
{
}

Point3f::~Point3f()
{
}

Point3f::Point3f(const glm::vec3& v)
{
    x = (v - originPos).x;
    y = (v - originPos).y;
    z = (v - originPos).z;
}

Point3f::Point3f(const Point3f& p) : x(p.x), y(p.y), z(p.z)
{
}

Point3f& Point3f::operator += (const glm::vec3& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return (*this);
}

Point3f& Point3f::operator = (const Point3f& p)
{
    x = p.x;
    y = p.y;
    z = p.z;
    return (*this);
}

Point3f operator+(const Point3f& p, const glm::vec3& v)
{
    return Point3f(p.x + v.x, p.y+v.y, p.z + v.z);
}

glm::vec3 operator-(const Point3f& p1, const Point3f& p2)
{
    return glm::vec3(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}