#include "Section.h"

Section::Section()
{
    num_of_cps = 0;
    num_of_spline_points = 0;
    control_points = nullptr;
    scale = 0.0;
    orientation = glm::quat(1, 0, 0, 0);
    center_point = glm::vec3();
    section_spline_curve = nullptr;
    catmullrom = true;
    resolution = 0;
    updateModelMTX();
}

Section& Section::operator=(const Section& inputSection)
{
    num_of_cps = inputSection.getNumOfCPs();
    scale = inputSection.getScale();
    orientation = inputSection.getOrientation();
    center_point = inputSection.getCenterPoint();
    modelMTX = inputSection.getModelMTX();
    catmullrom = inputSection.isCatmullRomSpline();
    resolution = inputSection.getResolution();

    control_points = nullptr;
    section_spline_curve = nullptr;


    if (!inputSection.isCPEmpty() && num_of_cps > 0)
    {
        control_points = new glm::vec3 [num_of_cps];
        for (size_t i = 0; i < num_of_cps; i++)
        {
            control_points[i] = inputSection[i];
        }
    }
    
    if (inputSection.getSectionSplineCurve() != nullptr)
    {
        num_of_spline_points = inputSection.getNumOfSplinePoints();
        section_spline_curve = new glm::vec3 [num_of_spline_points];

        for (size_t i = 0; i < num_of_spline_points ; i++)
        {
            section_spline_curve[i] = inputSection.getSectionSplineCurve()[i];
        }
    }
    return *this;
}

Section::Section(const Section& inputSection)
{
    num_of_cps = inputSection.getNumOfCPs();
    scale = inputSection.getScale();
    orientation = inputSection.getOrientation();
    center_point = inputSection.getCenterPoint();
    modelMTX = inputSection.getModelMTX();
    catmullrom = inputSection.isCatmullRomSpline();
    resolution = inputSection.getResolution();
    
    control_points = nullptr;
    section_spline_curve = nullptr;

    if (!inputSection.isCPEmpty() && num_of_cps > 0)
    {
        control_points = new glm::vec3 [num_of_cps];
        for (size_t i = 0; i < num_of_cps; i++)
        {
            control_points[i] = inputSection[i];
        }
    }
    
    if (inputSection.getSectionSplineCurve() != nullptr)
    {
        num_of_spline_points = inputSection.getNumOfSplinePoints();
        section_spline_curve = new glm::vec3 [num_of_spline_points];

        for (size_t i = 0; i < num_of_spline_points ; i++)
        {
            section_spline_curve[i] = inputSection.getSectionSplineCurve()[i];
        }
    }
}

void Section::updateControlPointsSize(const size_t& cp_num)
{
    num_of_cps = cp_num;

    if (num_of_cps == 0)
    {
        return;
    }

    if (control_points != nullptr)
    {
        delete [] control_points;
    }
    control_points = new glm::vec3 [num_of_cps];
}

void Section::updateScaleFactor(const float& scale_factor)
{
    scale = scale_factor;
    updateModelMTX();
}

void Section::updateOrientation(const float& angle, const glm::vec3& axis)
{
    orientation = glm::angleAxis(angle, axis);
    // orientation = glm::quat(cosf(glm::degrees(angle)/2), sinf(glm::degrees(angle)/2) * axis);
    updateModelMTX();
}

void Section::updateOrientation(const glm::quat& new_orientaion)
{
    orientation = new_orientaion;
    updateModelMTX();
}

void Section::updateCenterPoint(const glm::vec3& center)
{
    center_point = center;
    updateModelMTX();
}

void Section::updateSplineName(const bool& isCatmullrom)
{
    catmullrom = isCatmullrom;
}


void Section::updateSplineCurve(void)
{
    if (num_of_cps < 4 || control_points == nullptr)
    {
        return;
    }
    
    if (section_spline_curve != nullptr)
    {
        delete [] section_spline_curve;
    }

    std::tuple<glm::vec3*, size_t> result;
    if (catmullrom) // Catmull-Rom
    {
        // section_spline_curve = calculateCatmullRomClosedSpline(num_of_cps, control_points);
        result = closedCalculateCatmullRom(num_of_cps, control_points, resolution);
    }
    else    // B-Spline
    {
        result = calculateBSpline(num_of_cps, control_points, resolution);
    }
    section_spline_curve = std::get<0>(result);
    num_of_spline_points = std::get<1>(result);

    for (size_t i = 0; i < num_of_spline_points; i++)
    {
        glm::vec4 transform_result = modelMTX * glm::vec4(section_spline_curve[i].x, section_spline_curve[i].y, section_spline_curve[i].z, 1);
        section_spline_curve[i] = glm::vec3(transform_result.x, transform_result.y, transform_result.z);
    }
}

void Section::updateResolution(const size_t& resol)
{
    resolution = resol;
}

void Section::updateModelMTX(void)
{
    glm::mat4 scaleMTX = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
    glm::mat4 rotMTX = glm::toMat4(orientation);
    glm::mat4 translationMTX = glm::translate(glm::mat4(1.0f), glm::vec3(center_point.x, center_point.y, center_point.z));
    modelMTX = translationMTX * rotMTX * scaleMTX * glm::toMat4(glm::angleAxis(3.141592f * 0.5f, glm::vec3(1, 0, 0)));
}

glm::vec3& Section::operator[](const size_t& point_idx)
{
    return control_points[point_idx];
}

const glm::vec3 Section::operator[](const size_t& point_idx) const
{
    return control_points[point_idx];
}

const glm::mat4& Section::getModelMTX(void) const
{
    return modelMTX;
}

const bool Section::isCPEmpty(void) const
{
    return (control_points == nullptr || num_of_cps == 0);
}

const bool& Section::isCatmullRomSpline(void) const
{
    return catmullrom;
}


const size_t& Section::getNumOfCPs(void) const
{
    return num_of_cps;
}

const size_t& Section::getNumOfSplinePoints(void) const
{
    return num_of_spline_points;
}

const float& Section::getScale(void) const
{
    return scale;
}

const glm::quat& Section::getOrientation(void) const
{
    return orientation;
}

const glm::vec3& Section::getCenterPoint(void) const
{
    return center_point;
}

const size_t& Section::getResolution(void) const
{
    return resolution;
}

const glm::vec3* Section::getSectionSplineCurve(void) const
{
    return section_spline_curve;
}


Section::~Section()
{
    if (control_points != nullptr)
    {
        delete [] control_points;
    }

    if (section_spline_curve != nullptr)
    {
        delete [] section_spline_curve;
    }
}

std::ostream& operator<<(std::ostream& os, const Section& section)
{
    os << "Section Information\n";
    os << "Number of Total Control Points: " << section.num_of_cps << "\n";
    os << "Scale Factor: " << section.scale << "\n";
    os << "Orientation: " << section.orientation.w << ", " << section.orientation.x << ", " << section.orientation.y << "," << section.orientation.z << "\n" ;
    os << "Center Point: " << section.center_point.x  << ", " << section.center_point.y  << ", " << section.center_point.z << "\n";
    return os;
}

