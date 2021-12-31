#pragma once
#include "Ray.h"
#include "Object.h"
#include "Polygon3D.h"

class RayElement
{
private:
    Object * obj;
    Ray * ray;
    RayElement* reflectElement;
    RayElement* refractElement;
public:
    RayElement();
    ~RayElement();
};


class RayTree
{
private:
    /* data */
    
public:
    RayTree(/* args */);
    ~RayTree();
};


