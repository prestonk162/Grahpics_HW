#pragma once
#include "Cylinder.h"

#define DESERT_CAMO_COLOR_RGB 237/MAX_VALUE_RGB,201/MAX_VALUE_RGB,175/MAX_VALUE_RGB

class Cannon
{
private:
    /* data */
    GLboolean firing;
    GLUquadric* qobj;
    GLfloat caliberSize;
    GLfloat barrelThickness;
    GLfloat retreatDistance;
    GLfloat barrelLength;
    GLfloat recoilSpeed;
    const GLfloat maxRetreatDistance;
    GLboolean retreat;
public:
    Cannon(const GLfloat& caliber, const GLfloat& length, const GLfloat& recoil);
    void draw(void);
    void fire(void);
    void update(void);
    ~Cannon();
};
