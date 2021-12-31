#include "Model.h"

Model::Model(const GLfloat& width, const GLfloat& thickness, const GLfloat& height) : bodyWidth(width), bodyHeight(height), bodyThickness(thickness)
{
    turret = new Turret(bodyWidth * 0.3, bodyThickness * 0.2);
}

void Model::draw(void)
{
    glPushMatrix();
    glColor3f(1, 0, 0);
    glScalef(bodyHeight, bodyThickness, bodyWidth);
    glutSolidCube(1.0);
    glColor3f(0, 0, 0);
    glutWireCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, bodyThickness*0.5, 0);
    turret->draw();
    glPopMatrix();
}

GLfloat Model::getTurretCurrentAngle(void)
{
    return turret -> getTurretCurrentAngle();
}
GLfloat Model::getMinigunCurrentAngle(void)
{
    return turret -> getMinigunCurrentAngle();
}

void Model::control(const GLboolean& turretRotate, const GLboolean& turretRotateCW, const GLboolean& gunRotate, const GLboolean& gunUp, const GLboolean& gunFire)
{
    if(turretRotate)
    {
        turret -> rotateTurret(turretRotateCW);
    }

    if(gunRotate)
    {
        turret -> rotateGun(gunUp);
    }

    gunFire ? turret -> fire() : turret -> ceaseFire();
    turret -> update();
}

Model::~Model()
{
    delete turret;
}