#pragma once

#include <GL/glut.h>
#include <glm/glm.hpp>

#define ZERO_VECTOR_3D glm::vec3(0.0, 0.0, 0.0)

class Point3f
{
private:
    /* data */
    static glm::vec3 originPos;
public:
    GLfloat x;
    GLfloat y;
    GLfloat z;
    Point3f();
    Point3f(const GLfloat& x, const GLfloat& y, const GLfloat& z);
    Point3f(const glm::vec3& v);
    Point3f(const Point3f& p);

    Point3f& operator+=(const glm::vec3& v);
    Point3f& operator=(const Point3f& p);
    ~Point3f();
};

Point3f operator+(const Point3f& p, const glm::vec3& v);
glm::vec3 operator-(const Point3f& p1, const Point3f& p2);