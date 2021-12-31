#pragma once
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glu.h>
#include <GL/glut.h>
#include <glm/gtx/quaternion.hpp>
#include "Section.h"

#define DEFAULT_MODEL_DIR std::string("model")

class Model
{
private:
    /* data */
    Section* control_sections;
    size_t num_of_control_sections;
    size_t num_of_cps_per_section;

    std::string spline;

    Section* spline_sections;
    size_t num_of_spline_sections;
    size_t resolution;
public:
    Model();

    void updateSplineName(const std::string& spline_name);
    void setResolution(const size_t& resol);
    
    void loadData(const std::string& filename);
    Section& operator[](const size_t& section_idx);

    void setNumOfSections(const size_t& control_section_num);
    void setNumOfCPsPerSection(const size_t& cp_num);
    
    void updateControlSections(void);
    void updateSplineSections();

    friend std::ostream& operator<<(std::ostream& os, const Model& model);
    void drawModel(void) const;

    ~Model();
};

Section * catmullRomDecasteliju(Section* const &control, const size_t& num_of_control, const size_t& num_of_cp_per_section, const size_t& resolution);