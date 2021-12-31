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

void SolidShape3D::translate(const glm::vec3& displacement)
{
    center += displacement;
}

void SolidShape3D::setVelocity(const glm::vec3& vel)
{
    velocity = vel;
}

void SolidShape3D::setAcceleration(const glm::vec3& acc)
{
    acceleration = acc;
}

void SolidShape3D::setRotation(const glm::vec3& rot)
{
    rotation = rot;
}

void SolidShape3D::rotate(const GLfloat& angle, const glm::vec3& rotator)
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