#include "Cannon.h"

Cannon::Cannon(const GLfloat& caliber, const GLfloat& length, const GLfloat& recoil) : caliberSize(caliber), barrelLength(length), recoilSpeed(recoil), maxRetreatDistance(-recoil*120)
{
    qobj = gluNewQuadric();
    barrelThickness = (0.2) * caliberSize;
    firing = false;
    retreatDistance = 0;
    retreat = false;
}

Cannon::~Cannon()
{
    gluDeleteQuadric(qobj);
}

void Cannon::draw(void)
{   
    glPushMatrix();

    if (firing)
    {
        glTranslatef(0, 0, retreatDistance);
    }

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

    glColor3f(BLACK_RGB);
    gluDisk(qobj, 0, caliberSize, 10, 10);
    
    gluQuadricDrawStyle(qobj, GLU_LINE);
    gluDisk(qobj, caliberSize, caliberSize + barrelThickness, 10, 10);

    glTranslatef(0, 0, -barrelLength);
    gluCylinder(qobj, caliberSize + barrelThickness, caliberSize + barrelThickness, barrelLength, 10, 10);

    glPopMatrix();
}

void Cannon::fire(void)
{
    if(!firing)
    {
        firing = !firing;
        retreat = true;
    }
}

void Cannon::update(void)
{
    if (firing)
    {
        if(retreat && retreatDistance <= maxRetreatDistance) 
        {
            retreatDistance = maxRetreatDistance;
            retreat = false;
        }

        retreat ? retreatDistance -= recoilSpeed : retreatDistance += recoilSpeed;

        if (!retreat && retreatDistance >= 0)
        {
            retreatDistance = 0;
            firing = false;
        }
    }    
}