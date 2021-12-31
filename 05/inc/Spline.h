#pragma once
#include <vector>
#include <tuple>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/spline.hpp>

template <typename T>
std::tuple<T*, size_t> calculateCatmullRom(const size_t& cp_num, T* const &control_points, const bool& closed);

template <typename T>
void knotBSplineCalculate(const T& p0, const T& p1, const T& p2, const T& p3, std::vector<T>& curve, const size_t& resolution);

template <typename T>
std::tuple<T*, size_t> calculateBSpline(const size_t& num_of_cps, T* const &control_points, const size_t& resolution);

template <typename T>
std::tuple<T*, size_t> openedCalculateCatmullRom(const size_t& cp_num, T* const &control_points, const size_t& resolution)
{
    T * spline_points = new T [resolution*(cp_num - 1)];

    for (size_t i = 0; i < resolution; i++)
    {
        spline_points[i] = control_points[0];
    }

    T bp[4];

    for (size_t idx = 2; idx < cp_num - 1; idx++)
    {
        bp[0] = control_points[idx-2];
        bp[1] = control_points[idx-1];
        bp[2] = control_points[idx];
        bp[3] = control_points[idx+1];

        for (size_t i = 0; i < resolution; i++)
        {
            float t = (float)i/(float)resolution;
            spline_points[resolution*(idx-1)+i] = glm::catmullRom(bp[0], bp[1], bp[2], bp[3], t);
        }
    }

    for (size_t i = 0; i < resolution; i++)
    {
        spline_points[resolution*(cp_num-2)+i] = control_points[cp_num-1];
    }

    return std::tuple<T*, size_t>(spline_points, resolution*(cp_num-1));     
}

template <typename T>
std::tuple<T*, size_t> closedCalculateCatmullRom(const size_t& cp_num, T* const &control_points, const size_t& resolution)
{
    T * spline_points = new T [resolution*(cp_num)];

    T bp[4];

    for (size_t idx = 0; idx < cp_num; idx++)
    {
        if (idx > 1 && idx < cp_num - 1)
        {
            bp[0] = control_points[idx-2];
            bp[1] = control_points[idx-1];
            bp[2] = control_points[idx];
            bp[3] = control_points[idx+1];
        }
        else if(idx == 0)
        {
            bp[0] = control_points[cp_num-2];
            bp[1] = control_points[cp_num-1];
            bp[2] = control_points[idx];
            bp[3] = control_points[idx+1];
        }
        else if(idx == 1)
        {
            bp[0] = control_points[cp_num-1];
            bp[1] = control_points[idx-1];
            bp[2] = control_points[idx];
            bp[3] = control_points[idx+1];
        }
        else
        {
            bp[0] = control_points[idx-2];
            bp[1] = control_points[idx-1];
            bp[2] = control_points[idx];
            bp[3] = control_points[0];
        }

        for (size_t i = 0; i < resolution; i++)
        {
            float t = (float)i/(float)resolution;
            spline_points[resolution*(idx-1)+i] = glm::catmullRom(bp[0], bp[1], bp[2], bp[3], t);
        }
    }

    return std::tuple<T*, size_t>(spline_points, resolution*(cp_num-1));     
}

template <typename T>
std::tuple<T*, size_t> calculateBSpline(const size_t& num_of_cps, T* const &control_points, const size_t& resolution)
{

    if (num_of_cps < 4)
    {
        return std::tuple<T*, size_t> (nullptr, 0);
    }

    std::vector<T> curve = std::vector<T>();

    for (size_t i = 0; i < num_of_cps - 3; i++)
    {
        knotBSplineCalculate(control_points[i], 
                            control_points[i+1], 
                            control_points[i+2], 
                            control_points[i+3],
                            curve, 
                            resolution);
    }
    knotBSplineCalculate(control_points[num_of_cps - 3], 
                            control_points[num_of_cps - 2], 
                            control_points[num_of_cps - 1], 
                            control_points[0],
                            curve, 
                            resolution);

    knotBSplineCalculate(control_points[num_of_cps - 2], 
                            control_points[num_of_cps - 1], 
                            control_points[0], 
                            control_points[1],
                            curve, 
                            resolution);

    knotBSplineCalculate(control_points[num_of_cps - 1], 
                            control_points[0], 
                            control_points[1], 
                            control_points[2],
                            curve, 
                            resolution);

    size_t spline_points_num = curve.size();
    T * spline_curve = new T [spline_points_num];
    
    for (size_t i = 0; i < spline_points_num; i++)
    {
        spline_curve[i] = curve.at(i);
    }

    return std::tuple<T*, size_t>(spline_curve, spline_points_num);
}

template <typename T>
void knotBSplineCalculate(const T& p0, const T& p1, const T& p2, const T& p3, std::vector<T>& curve, const size_t& resolution)
{
    float t = 0.0;
    float b0;
    float b1;
    float b2;
    float b3;

    T spline_point;

    for (size_t i = 0; i < resolution; i++)
    {
        t = (float)i * 1.0f / (float) resolution;
        b0 = (1.0-t)*(1.0-t)*(1.0-t)/6.0f;
        b1 = (3.0f*t*t*t - 6.0f*t*t + 4.0f)/6.0f;
        b2 = (-3.0f*t*t*t + 3.0f*t*t + 3.0f*t + 1.0f)/6.0f;
        b3 = t*t*t/6.0f;

        spline_point = p0 * b0 + p1 * b1 + p2 * b2 + p3 * b3;
        curve.push_back(spline_point);
    }
}