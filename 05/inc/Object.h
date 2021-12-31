#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <GL/glut.h>
#include "Material.h"

enum OBJECT_TYPE {
    OFF_IMPORT,
    SPLINE_SURFACE_IMPORT,
    POLYGON3D_OBJECT,
    TEXTURE_BOX,
    TEXTURE_SPHERE
};

class Object
{
private:
    /* data */
    OBJECT_TYPE objType;
public:
    Object(/* args */);
    Object(const OBJECT_TYPE& type);
    glm::dvec3 position;
    GLfloat scale;
    glm::quat orientation;
    Material mat;
    void setObjType(const OBJECT_TYPE& type);
    const OBJECT_TYPE& getObjType(void) const;
    virtual void draw(void) const = 0;    

    ~Object();
};