#include "Minigun.h"


Minigun::Minigun(const GLfloat& caliber, const GLfloat& length, const GLfloat& speed) : caliberSize(caliber), barrelLength(length), rotateSpeed(speed), absAngle(0), firing(false)
{
    qobj = gluNewQuadric();
    barrelThickness = (0.2) * caliberSize;
}

Minigun::~Minigun()
{
    gluDeleteQuadric(qobj);
}

void Minigun::draw(void)
{   
    glPushMatrix();

    glColor3f(DESERT_CAMO_COLOR_RGB);
    gluQuadricDrawStyle(qobj, GLU_FILL);
    gluDisk(qobj, caliberSize, caliberSize + barrelThickness, 10, 10);

    gluCylinder(qobj, caliberSize + barrelThickness, caliberSize + barrelThickness, barrelLength, 10, 10);

    glTranslatef(0, 0, barrelLength);
    gluDisk(qobj, caliberSize, caliberSize + barrelThickness, 10, 10);
        
        glPushMatrix();
        glTranslatef(0, caliberSize * 1.25 , -caliberSize * 0.2);
        glutSolidCube(caliberSize * 0.4);
        glPopMatrix();

        glPushMatrix();
            glColor3f(1, 1, 1);
            glRotatef(absAngle, 0, 0, 1);
            for (size_t i = 0; i < 6; i++)
            {
                glPushMatrix();
                glRotatef(60*i, 0, 0, 1);
                glTranslatef(0, caliberSize * 0.7, 0);
                gluCylinder(qobj, caliberSize * 0.1, caliberSize * 0.1, barrelLength * 0.05, 5, 5);
                gluDisk(qobj, caliberSize * 0.02, caliberSize * 0.1, 5, 5);
                glPopMatrix();
            }
        glPopMatrix();

    glColor3f(BLACK_RGB);
    gluDisk(qobj, 0, caliberSize, 10, 10);
    
    gluQuadricDrawStyle(qobj, GLU_LINE);
    gluDisk(qobj, caliberSize, caliberSize + barrelThickness, 10, 10);

    glTranslatef(0, 0, -barrelLength);
    gluCylinder(qobj, caliberSize + barrelThickness, caliberSize + barrelThickness, barrelLength, 10, 10);

    glPopMatrix();
}

void Minigun::fire(void)
{
    if(!firing)
    {
        firing = true;
    }
}

void Minigun::ceaseFire(void)
{
    if(firing)
    {
        firing = false;
    }
}

void Minigun::update(void)
{
    if (firing)
    {
        if(absAngle >= 360)
        {
            absAngle = 0;
        }
        absAngle += rotateSpeed;
    }    
}