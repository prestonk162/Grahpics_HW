#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <GL/glut.h>
#include "Material.h"

class Object
{
private:
    /* data */
public:
    Object(/* args */);
    glm::vec3 position;
    GLfloat scale;
    glm::quat orientation;
    Material mat;
    virtual void draw(void) const = 0;    

    ~Object();
};