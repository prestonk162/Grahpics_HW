#pragma once

#include <GL/glu.h>

#include "SolidShape3D.h"

class Cylinder : public SolidShape3D
{
private:
    GLfloat radius;
    GLfloat height;
    GLint stack;
    GLint slice;
    GLUquadric *qobj;
public:
    Cylinder(const GLfloat &r, const GLfloat &h, const GLint& st, const GLint& sl, const Point3f& c);
    ~Cylinder();
    virtual void draw(void);
};
