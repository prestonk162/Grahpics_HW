#include "SolidShape3D.h"

SolidShape3D::SolidShape3D(const GLfloat& x, const GLfloat& y, const GLfloat& z)
{
    center = Point3f(x, y, z);
    rotation = ZERO_VECTOR_3D;
    velocity = ZERO_VECTOR_3D;
    acceleration = ZERO_VECTOR_3D;
    colorRGB = Point3f(WHITE_RGB);
}

SolidShape3D::SolidShape3D(const Point3f& c)
{
    center = c;
    rotation = ZERO_VECTOR_3D;
    velocity = ZERO_VECTOR_3D;
    acceleration = ZERO_VECTOR_3D;
    colorRGB = Point3f(WHITE_RGB);
}

SolidShape3D::~SolidShape3D()
{
}

void SolidShape3D::translate(const Vector3f& displacement)
{
    center += displacement;
}

void SolidShape3D::setVelocity(const Vector3f& vel)
{
    velocity = vel;
}

void SolidShape3D::setAcceleration(const Vector3f& acc)
{
    acceleration = acc;
}

void SolidShape3D::setRotation(const Vector3f& rot)
{
    rotation = rot;
}

void SolidShape3D::rotate(const GLfloat& angle, const Vector3f& rotator)
{
    rotation += (angle * rotator);
}

void SolidShape3D::changeColorRGB(const GLfloat& r, const GLfloat& g, const GLfloat& b)
{
    colorRGB = Point3f(r,g,b);
}

void SolidShape3D::changeColorRGB(const Point3f color)
{
    colorRGB = color;
}