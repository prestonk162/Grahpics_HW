#pragma once
#include <glm/glm.hpp>
#include "Color.h"

class Pixel
{
private:
    /* data */
public:
    Color pixelColor;
    glm::dvec3 woordCoords;
    Pixel(/* args */);
    Pixel(const glm::dvec3& coord);
    Pixel(const glm::dvec3& coord, const Color& color);
    ~Pixel();
};
