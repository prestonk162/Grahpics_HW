#pragma once

#include <GL/glut.h>
#include <glm/glm.hpp>
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

    glm::vec3 rotation;
    glm::vec3 velocity;
    glm::vec3 acceleration;
public:
    SolidShape3D(const GLfloat& x, const GLfloat& y, const GLfloat& z);
    SolidShape3D(const Point3f& c);
    void setVelocity(const glm::vec3& vel);
    void setAcceleration(const glm::vec3& acc);
    void setRotation(const glm::vec3& rot);
    void translate(const glm::vec3& displacement);
    void rotate(const GLfloat& angle, const glm::vec3& rotator);
    void changeColorRGB(const GLfloat& r, const GLfloat& g, const GLfloat& b);
    void changeColorRGB(const Point3f color);
    virtual void draw(void) = 0;
    ~SolidShape3D();
protected:
    Point3f colorRGB;
};
