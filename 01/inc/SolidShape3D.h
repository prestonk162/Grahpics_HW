#pragma once

#include <GL/glut.h>

#include "Vector.h"
#include "Point.h"

#define BLACK_RGB 0.0,0.0,0.0
#define YELLOW_RGB 1.0,1.0,0.0
#define BLUE_RGB 0.0,0.0,1.0
#define WHITE_RGB 1.0,1.0,1.0
#define RED_RGB 1.0,0.0,0.0
#define GREEN_RGB 0.0,1.0,0.0

#define MAX_VALUE_RGB 256.0

#define IDX_R 0
#define IDX_G 1
#define IDX_B 2

class SolidShape3D
{
private:
    /* data */
    Point3f center;
    Vector3f rotation;
    Vector3f velocity;
    Vector3f acceleration;
public:
    SolidShape3D(const GLfloat& x, const GLfloat& y, const GLfloat& z);
    SolidShape3D(const Point3f& c);
    void setVelocity(const Vector3f& vel);
    void setAcceleration(const Vector3f& acc);
    void setRotation(const Vector3f& rot);
    void translate(const Vector3f& displacement);
    void rotate(const GLfloat& angle, const Vector3f& rotator);
    void changeColorRGB(const GLfloat& r, const GLfloat& g, const GLfloat& b);
    void changeColorRGB(const Point3f color);
    virtual void draw(void) = 0;
    ~SolidShape3D();
protected:
    Point3f colorRGB;
};
