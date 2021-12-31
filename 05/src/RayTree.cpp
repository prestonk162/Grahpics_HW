#include "RayTree.h"

RayElement::RayElement()
{
    obj = nullptr;
    ray = nullptr;
    reflectElement = nullptr;
    refractElement = nullptr;
}

RayElement::~RayElement()
{
    if(reflectElement != nullptr) delete reflectElement;
    if(refractElement != nullptr) delete refractElement; 
    if (obj != nullptr) delete obj;
    if (ray != nullptr) delete ray;
}

RayTree::RayTree(/* args */)
{
    
}

RayTree::~RayTree()
{
}