#pragma once

#include <GL/glut.h>

#define ZERO_VECTOR_3D Vector3f(0.0, 0.0, 0.0)

class Vector3f
{
private:
    /* data */
    GLfloat direction [3];
public:
    Vector3f();
    Vector3f(const GLfloat& x, const GLfloat& y, const GLfloat& z);
    void setDirection(const GLfloat& x, const GLfloat& y, const GLfloat& z);
    Vector3f(const Vector3f& v);
    GLfloat& operator [] (const int i);
    GLfloat operator [] (const int i) const;

    Vector3f& operator+=(const Vector3f& v);
    Vector3f& operator-=(const Vector3f& v);
    Vector3f& operator*=(const GLfloat& s);
    Vector3f& operator/=(const GLfloat& s);
    Vector3f& operator=(const Vector3f& v);
    ~Vector3f();
};

Vector3f operator+(const Vector3f& v1, const Vector3f& v2);
Vector3f operator-(const Vector3f& v1, const Vector3f& v2);
Vector3f operator*(const float s, const Vector3f& vec3);
Vector3f operator/(const Vector3f& vec3, const float s);
GLfloat dotProduct(const Vector3f& v1, const Vector3f& v2);