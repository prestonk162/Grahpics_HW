#pragma once
#include <GL/glut.h>
#include <glm/glm.hpp>
#include "Polygon3D.h"
#include "Box.h"
#include "ConcreteSphere.h"
#include <tuple>
#include <cmath>
#include <iostream>

#define STD_MAXIMUM_STEP 20000.0f

typedef std::tuple<GLboolean, GLdouble> IntersectionInfo;
typedef std::tuple<GLboolean, GLdouble, glm::dvec3> IntersectionPolygonInfo;

class Ray
{
private:
    /* data */
    glm::dvec3 dir;
    glm::dvec3 startPoint;
    GLdouble maximum_step;
public:
    void changeStartPoint(const glm::dvec3& newStart);
    void changeDir(const glm::dvec3& newDir);
    void changeDirbyNewEndPoint(const glm::dvec3& newEnd);
    void changeMaximumStep(const GLdouble& newMax);
    const glm::dvec3& getRayDirection(void) const;
    const glm::dvec3& getRayOrigin(void) const;
    const GLdouble& getMaximumStep(void) const;
    Ray();
    Ray(const glm::dvec3& rayDir, const glm::dvec3& rayOrigin);
    Ray(const Ray& r1);
    IntersectionInfo intersectWithSphere(const glm::dvec3& center, const GLdouble& radius) const;
    IntersectionPolygonInfo intersectWithPolygon(const Polygon3D& polygon) const;
    IntersectionPolygonInfo intersectWithTextureBox(const Box& box) const;
    GLboolean intersectWith3DPolygon(const Polygon3D& polygon) const;
    ~Ray();
};