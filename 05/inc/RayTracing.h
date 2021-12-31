#pragma once
#include <GL/glut.h>
#include <iostream>
#include <glm/glm.hpp>
#include "Ray.h"
#include "Scene.h"
#include "Pixel.h"
#include "Box.h"
#include "Color.h"

class RayTracing
{
private:
    /* data */
    glm::dvec3 ** pixelWorldCoords; // [height][width]
    
    GLint width;
    GLint height;
    Ray ** primaryRays;
public:
    glm::dvec3 cameraPos;
    GLfloat * pixelColorBuffer;
    GLubyte * pixelColorByteBuffer;
    RayTracing(/* args */);
    RayTracing(const GLint& w, const GLint& h, const glm::dvec3& cameraOrigin);
    void calculatePixelCoord(const GLint& w, const GLint& h);
    void rayTrace(const Scene& scene);
    void draw(void) const;
    Color reflectHandling(const Ray& inputRay, const glm::dvec3& normal, const glm::dvec3& intersectPoint, const GLuint& depth, const Scene& scene, const Material& objMat, Object *obj) const;
    Color refractionHandling(const Ray& inputRay, const glm::dvec3& normal, const glm::dvec3& intersectPoint, const GLuint& depth, const Scene& scene, const Material& objMat, Object* obj) const;
    Color localColor(const Ray& inputRay, const glm::dvec3& normal, const glm::dvec3& intersectPoint, const Scene& scene, const Material& objMat, Object* obj) const;
    ~RayTracing();
};
