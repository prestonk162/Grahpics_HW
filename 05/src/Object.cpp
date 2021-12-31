#include "Object.h"

Object::Object(/* args */)
{
    position = glm::dvec3(0, 0, 0);
    scale = 1.0f;
    orientation = glm::quat(1, 0, 0, 0);
    mat = Material();
    setObjType(OBJECT_TYPE::POLYGON3D_OBJECT);
}

Object::Object(const OBJECT_TYPE& type)
{
    position = glm::dvec3(0, 0, 0);
    scale = 1.0f;
    orientation = glm::quat(1, 0, 0, 0);
    mat = Material();
    setObjType(type);
}

void Object::setObjType(const OBJECT_TYPE& type)
{
    objType = type;
}

const OBJECT_TYPE& Object::getObjType(void) const
{
    return objType;
}

Object::~Object()
{
}
