#pragma once
#include "Turret.h"

#define FRONT_LEFT_WHEEL 0
#define FRONT_RIGHT_WHEEL 1
#define REAR_LEFT_WHEEL 2
#define REAR_RIGHT_WHEEL 3

class Model
{
private:
    Turret* turret;
    GLfloat bodyWidth;  // z
    GLfloat bodyHeight; //  x
    GLfloat bodyThickness;   // y
     
public:
    Model(const GLfloat& width, const GLfloat& thickness, const GLfloat& height);
    void draw(void);
    void control(const GLboolean& turretRotate, const GLboolean& turretRotateCW, const GLboolean& gunRotate, const GLboolean& gunUp, const GLboolean& gunFire);
    GLfloat getTurretCurrentAngle(void);
    GLfloat getMinigunCurrentAngle(void);
    ~Model();
};