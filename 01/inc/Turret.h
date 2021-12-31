#pragma once
#include "Minigun.h"
#include "Cannon.h"
#include "Cylinder.h"

class Turret
{
private:
    /* data */
    Cylinder* turretBody;
    // Cannon* turretCannon;
    Minigun* turretMinigun;
    GLfloat turretAngle;
    GLfloat cannonAngle;
    GLfloat turretRadius;
    GLfloat turretHeight;
    const GLfloat turretRotateSpeed;
    const GLfloat cannonRotateSpeed;
    const GLfloat minCannonRotateAngle;
    const GLfloat maxCannonRotateAngle;
    GLUquadric* qobj;
public:
    Turret(const GLfloat& radius, const GLfloat& height);
    void fire(void);
    void ceaseFire(void);
    void rotateGun(const GLboolean& up);
    void rotateTurret(const GLboolean& cw);
    void draw(void);
    void update(void);
    GLfloat getTurretCurrentAngle(void);
    GLfloat getMinigunCurrentAngle(void);
    ~Turret();
};