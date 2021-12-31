#pragma once
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/spline.hpp>
#include <iostream>
#include <vector>
#include <tuple>
#include "Spline.h"

class Section
{
private:
    /* data */
    size_t num_of_cps;
    glm::vec3* control_points;
    
    size_t num_of_spline_points;
    glm::vec3 * section_spline_curve;

    float scale;
    glm::quat orientation;
    glm::vec3 center_point;
    glm::mat4 modelMTX;

    size_t resolution;

    bool catmullrom;

public:
    Section();
    Section& operator=(const Section& inputSection);
    Section(const Section& inputSection);

    void updateControlPointsSize(const size_t& cp_num);
    void updateScaleFactor(const float& scale_factor);
    void updateOrientation(const float& angle, const glm::vec3& axis);
    void updateOrientation(const glm::quat& new_orientaion);
    void updateCenterPoint(const glm::vec3& center);
    void updateSplineName(const bool& isCatmullrom);
    void updateSplineCurve(void);
    void updateModelMTX(void);
    void updateResolution(const size_t& resol);

    glm::vec3& operator[](const size_t& point_idx);
    const glm::vec3 operator[](const size_t& point_idx) const;

    friend std::ostream& operator<<(std::ostream& os, const Section& section);
    
    const bool isCPEmpty(void) const;
    const bool& isCatmullRomSpline(void) const;

    const size_t& getNumOfCPs(void) const;
    const size_t& getNumOfSplinePoints(void) const;
    
    const float& getScale(void) const;
    const glm::quat& getOrientation(void) const;
    const glm::vec3& getCenterPoint(void) const;
    const size_t& getResolution(void) const;
    
    const glm::vec3* getSectionSplineCurve(void) const;
    
    const glm::mat4& getModelMTX(void) const;
    ~Section();
};