#include "Cylinder.h"

Cylinder::Cylinder(const GLfloat &r, const GLfloat &h, const GLint& st, const GLint& sl, const Point3f& c) : radius(r), height(h), stack(st), slice(sl), SolidShape3D(c) 
{
    qobj = gluNewQuadric();
}

void Cylinder::draw(void)
{
    glPushMatrix();
    gluQuadricDrawStyle(qobj, GLU_FILL);
    
    glColor3f(colorRGB[IDX_R], colorRGB[IDX_G], colorRGB[IDX_B]);
    gluCylinder(qobj, radius, radius, height, slice, stack);
    
    glColor3f(0, 0, 0);
    gluQuadricDrawStyle(qobj, GLU_LINE);
    gluCylinder(qobj, radius, radius, height, slice, stack);

    gluDisk(qobj, 0.0, (GLdouble)radius, slice, stack);
    
    glColor3f(colorRGB[IDX_R], colorRGB[IDX_G], colorRGB[IDX_B]);
    gluQuadricDrawStyle(qobj, GLU_FILL);
    gluDisk(qobj, 0.0, (GLdouble)radius, slice, stack);

    glTranslatef(0, 0, height);
    gluDisk(qobj, 0.0, (GLdouble)radius, slice, stack);
    
    glColor3f(0, 0, 0);
    gluQuadricDrawStyle(qobj, GLU_LINE);
    gluDisk(qobj, 0.0, (GLdouble)radius, slice, stack);

    glTranslatef(0, 0, -height);
    glPopMatrix();
}

Cylinder::~Cylinder()
{
    gluDeleteQuadric(qobj);
}