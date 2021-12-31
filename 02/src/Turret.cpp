#include "Turret.h"

Turret::Turret(const GLfloat& radius, const GLfloat& height) : maxCannonRotateAngle(60), minCannonRotateAngle(-10), turretRotateSpeed(1), cannonRotateSpeed(0.5), turretAngle(0), cannonAngle(0), turretRadius(radius), turretHeight(height)
{
    turretBody = new Cylinder(turretRadius, turretHeight, 10, 10, Point3f());
    // turretCannon = new Cannon(turretRadius * 0.2, turretRadius * 3, turretRadius * 0.5 / 120);
    turretMinigun = new Minigun(turretRadius * 0.2, turretRadius * 3, turretRadius * 10);
    turretBody -> changeColorRGB(BLUE_RGB);
    qobj = gluNewQuadric();
}

Turret::~Turret()
{
    gluDeleteQuadric(qobj);
    delete turretBody;
    delete turretMinigun;
    // delete turretCannon;
}

void Turret::fire(void)
{
    turretMinigun -> fire();
}

void Turret::ceaseFire(void)
{
    turretMinigun -> ceaseFire();
}

void Turret::rotateGun(const GLboolean& up)
{
    if(up)
    {
        cannonAngle >= maxCannonRotateAngle ? cannonAngle = maxCannonRotateAngle : cannonAngle += cannonRotateSpeed;
    }
    else
    {
        cannonAngle <= minCannonRotateAngle ? cannonAngle = minCannonRotateAngle : cannonAngle -= cannonRotateSpeed;
    }
}

void Turret::rotateTurret(const GLboolean& cw)
{
    if (cw)
    {
        if (turretAngle >= 360)
        {
            turretAngle = 0;
        }
        turretAngle += turretRotateSpeed;
    }
    else
    {
        if (turretAngle <= 0)
        {
            turretAngle = 360;
        }        
        turretAngle -= turretRotateSpeed;
    }
}

GLfloat Turret::getTurretCurrentAngle(void)
{
    return turretAngle;
}

GLfloat Turret::getMinigunCurrentAngle(void)
{
    return cannonAngle;
}

void Turret::draw(void)
{
    glPushMatrix();
    glTranslatef(0, turretHeight, 0);
    glRotatef(90.0, 1, 0, 0);
    glRotatef(turretAngle, 0, 0, 1);
    glColor3f(0, 0, 1);
    gluQuadricDrawStyle(qobj, GLU_FILL);
    gluSphere(qobj, turretRadius, 10, 10);
    glColor3f(0, 0, 0);
    gluQuadricDrawStyle(qobj, GLU_LINE);
    gluSphere(qobj, turretRadius, 10, 10);
    turretBody -> draw();
    glRotatef(-90, 1, 0, 0);
    glTranslatef(0, 0.25, 0);
    glRotatef(cannonAngle, -1, 0, 0);
    turretMinigun -> draw();
    glPopMatrix();
}

void Turret::update(void)
{
    turretMinigun -> update();
    //turretCannon -> update();
}