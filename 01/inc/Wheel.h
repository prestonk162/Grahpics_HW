#pragma once
#include "Cylinder.h"

class Wheel
{
private:
    /* data */
    GLfloat speed;
    GLfloat absAngle;
    GLfloat tireThickness;
    GLfloat frameRadius;
    GLfloat wheelWidth;
    GLUquadric* qobj;
public:
    Wheel(const GLfloat& thickness, const GLfloat& innerRadius, const GLfloat& width);
    void draw(void);
    void update(void);
    void changeSpeed(const GLfloat& speed);
    ~Wheel();
};
