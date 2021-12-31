#include "Pixel.h"

Pixel::Pixel(/* args */)
{
    pixelColor = Color();
    woordCoords = glm::dvec3();
}

Pixel::Pixel(const glm::dvec3& coord)
{
    pixelColor = Color();
    woordCoords = coord;
}

Pixel::Pixel(const glm::dvec3& coord, const Color& color)
{
    pixelColor = color;
    woordCoords = coord;
}

Pixel::~Pixel()
{
}