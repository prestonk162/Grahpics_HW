#include "Wheel.h"

Wheel::Wheel(const GLfloat& thickness, const GLfloat& innerRadius, const GLfloat& width)
{
    tireThickness = thickness;
    frameRadius = innerRadius;
    wheelWidth = width;
    speed = 0;
    absAngle = 0;
    qobj = gluNewQuadric();
}

Wheel::~Wheel()
{
    gluDeleteQuadric(qobj);
}

void Wheel::update(void)
{
    absAngle += speed;
}

void Wheel::draw(void)
{
    glPushMatrix();
    glRotatef(absAngle, 0.0, 0.0, 1.0);

    glColor3f(BLUE_RGB);
    gluQuadricDrawStyle(qobj, GLU_FILL);
    gluDisk(qobj, frameRadius, frameRadius + tireThickness, 10, 10);

    gluCylinder(qobj, frameRadius + tireThickness, frameRadius + tireThickness, wheelWidth, 10, 10);

    glTranslatef(0, 0, wheelWidth);
    gluDisk(qobj, frameRadius, frameRadius + tireThickness, 10, 10);
    glColor3f(WHITE_RGB);
    gluDisk(qobj, 0, frameRadius, 10, 10);

    glColor3f(BLACK_RGB);
    gluQuadricDrawStyle(qobj, GLU_LINE);
    gluDisk(qobj, 0, frameRadius + tireThickness, 10, 10);

    glTranslatef(0, 0, -wheelWidth);

    glColor3f(WHITE_RGB);
    gluQuadricDrawStyle(qobj, GLU_FILL);
    gluDisk(qobj, 0, frameRadius, 10, 10);

    glColor3f(BLACK_RGB);
    gluQuadricDrawStyle(qobj, GLU_LINE);
    gluDisk(qobj, 0, frameRadius + tireThickness, 10, 10);

    glPopMatrix();
}

void Wheel::changeSpeed(const GLfloat& newSpeed)
{
    speed = newSpeed;
}