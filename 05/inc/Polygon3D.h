#pragma once
#include "Object.h"

enum POLYGON_TYPE {
    SPHERE,
    TORUS,
    TETRAHEDRONE,
    TEAPOT,
    CONE,
    DEDECAHEDRON,
    ICOSAHEDRON,
    OCTAHEDRON,
    CUBE
};

class Polygon3D : public Object
{
private:
    /* data */
    POLYGON_TYPE polygonType;

public:
    Polygon3D(/* args */);
    Polygon3D(const size_t& o_r, const size_t& i_r, const size_t& sl, const size_t& st, const size_t& h);
    ~Polygon3D();
    const POLYGON_TYPE& getPolygonType(void) const;
    size_t o_radius;
    size_t i_radius;
    size_t slices;
    size_t stacks;
    size_t height;
    void draw(void) const;
    void setType(const POLYGON_TYPE& type);
};
