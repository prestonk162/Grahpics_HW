#pragma once

#include <GL/glut.h>

#include "Vector.h"

#define ZERO_POINT_3D Point3f(0.0, 0.0, 0.0)

class Point3f
{
private:
    /* data */
    GLfloat position [3];
public:
    Point3f();
    Point3f(const GLfloat& x, const GLfloat& y, const GLfloat& z);
    void setPosition(const GLfloat& x, const GLfloat& y, const GLfloat& z);
    Point3f(const Point3f& p);

    GLfloat& operator [] (const int i);
    GLfloat operator [] (const int i) const;

    Point3f& operator+=(const Vector3f& v);
    Point3f& operator=(const Point3f& p);
    ~Point3f();
};

Point3f operator+(const Point3f& p, const Vector3f& v);
Vector3f operator-(const Point3f& p1, const Point3f& p2);