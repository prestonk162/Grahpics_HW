#pragma once
#include "Cylinder.h"

#define DESERT_CAMO_COLOR_RGB 237/MAX_VALUE_RGB,201/MAX_VALUE_RGB,175/MAX_VALUE_RGB

class Minigun
{
private:
    /* data */
    GLboolean firing;
    GLUquadric* qobj;
    GLfloat caliberSize;
    GLfloat barrelThickness;
    GLfloat barrelLength;
    GLfloat rotateSpeed;
    GLfloat absAngle;
public:
    Minigun(const GLfloat& caliber, const GLfloat& length, const GLfloat& speed);
    void draw(void);
    void fire(void);
    void ceaseFire(void);
    void update(void);
    ~Minigun();
};
