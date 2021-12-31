#include "Object.h"

Object::Object(/* args */)
{
    position = glm::vec3(0, 0, 0);
    scale = 1.0f;
    orientation = glm::quat(1, 0, 0, 0);
    mat = Material();
}

Object::~Object()
{
}
