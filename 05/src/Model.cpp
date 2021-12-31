#include "Model.h"

const char * delimeters = " ";

Model::Model()
{
    control_sections = nullptr;
    num_of_control_sections = 0;
    num_of_cps_per_section = 0;
    num_of_spline_sections = 0;
    spline_sections = nullptr;
    num_of_points_per_spline_section = 0;
    faces = nullptr;
    mat = Material();
    vertex_normals = nullptr;
}

void Model::updateSplineName(const std::string& spline_name)
{
    spline = spline_name;
}

void Model::setResolution(const size_t& resol)
{
    resolution = resol;
}

void Model::setNumOfSections(const size_t& control_section_num)
{
    num_of_control_sections = control_section_num;
    if (control_sections != nullptr)
    {
        delete [] control_sections;
    }
    control_sections = new Section [num_of_control_sections];
}

void Model::setNumOfCPsPerSection(const size_t& cp_num)
{
    num_of_cps_per_section = cp_num;
    
    if (control_sections == nullptr || num_of_control_sections == 0)
    {
        return;
    }

    for (size_t i = 0; i < num_of_control_sections; i++)
    {
        control_sections[i].updateSplineName(spline == std::string("CATMULL_ROM"));
        control_sections[i].updateControlPointsSize(num_of_cps_per_section);
    }
}

Section& Model::operator[](const size_t& section_idx)
{
    return control_sections[section_idx];
}

void Model::loadData(const std::string& filename)
{
    std::fstream fp;
    fp.open(filename, std::fstream::in);

    bool blankTaken = false;

    if(fp.is_open()) {
        const int linesize = 128;
        char line[linesize];

        char * token = NULL;
        char * next_token = NULL;

        // float cp_x;
        // float cp_y;

        std::vector<float> data_stl_vec;

        size_t cur_line = 1;
        size_t section_idx = 0;
        size_t point_idx = 0;
        while(fp.getline(line, linesize)) {
            if(std::string(line).length() > 1) {
                token = NULL;
                next_token = NULL;
                blankTaken = false;
                
                token = strtok_r(line, delimeters, &next_token);
                if (token[0] == '#')
                {
                    continue;
                }
                

                if (cur_line == 1)
                {                    
                    updateSplineName(std::string(token));
                }
                else if (cur_line == 2 || cur_line == 3)
                {
                    // token = strtok_r(line, delimeters, &next_token);
                    char * endpos = NULL;
                    if (cur_line == 2)
                    {    
                        num_of_control_sections = strtoul(token, &endpos, 10);
                        setNumOfSections(num_of_control_sections);
                    }
                    else
                    {
                        num_of_cps_per_section = strtoul(token, &endpos, 10);
                        setNumOfCPsPerSection(num_of_cps_per_section);
                    }
                }
                else
                {
                    // token = strtok_r(line, delimeters, &next_token);
                    data_stl_vec.clear();
                    while (1)
                    {
                        if (token == NULL || token[0] == '#') break;
                        data_stl_vec.push_back(strtof(token, NULL));
                        
                        // std::cout << std::string(token) << std::endl;
                        token = strtok_r(NULL, delimeters, &next_token);
                    }

                    switch (data_stl_vec.size())
                    {
                    case 1:
                        control_sections[section_idx - 1].updateScaleFactor(data_stl_vec[0]);
                        break;
                    case 2:
                        control_sections[section_idx - 1][point_idx] = glm::vec3(data_stl_vec[0], data_stl_vec[1], 0.0f);
                        point_idx++;
                        break;
                    case 3:
                        control_sections[section_idx - 1].updateCenterPoint(glm::vec3(data_stl_vec[0], data_stl_vec[1], data_stl_vec[2]));
                        break;
                    case 4:
                        control_sections[section_idx - 1].updateOrientation(data_stl_vec[0], glm::vec3(data_stl_vec[1], data_stl_vec[2], data_stl_vec[3]));
                        break;
                    default:
                        break;
                    }
                }
                cur_line++;
            }
            else
            {
                if (section_idx > num_of_control_sections)
                {
                    break;
                }
                else
                {
                    if(!blankTaken) section_idx++;
                    blankTaken = true;
                    point_idx = 0;
                }
            }
        }
        updateControlSections();
        updateSplineSections();
        generateFaces();
        fp.close();
    }
    else {
        std::cout << "File Opening Failed" << std::endl;
    }
}

void Model::updateControlSections(void)
{
    if (num_of_control_sections == 0 || control_sections == nullptr)
    {
        return;
    }

    for (size_t i = 0; i < num_of_control_sections; i++)
    {
        control_sections[i].updateResolution(resolution);
        control_sections[i].updateSplineCurve();
    }
}

void Model::updateSplineSections()
{
    if (num_of_control_sections < 3)
    {
        return;
    }

    if (spline_sections != nullptr)
    {
        delete [] spline_sections;
    }

    spline_sections = catmullRomDecasteliju(control_sections, num_of_control_sections, num_of_cps_per_section, resolution);
    num_of_spline_sections = resolution * (num_of_control_sections -1);
    num_of_points_per_spline_section = spline_sections[0].getNumOfSplinePoints();

    vertex_normals = new glm::vec3 * [num_of_spline_sections];
    for (size_t i = 0; i < num_of_spline_sections; i++)
    {
        vertex_normals[i] = new glm::vec3 [num_of_points_per_spline_section];
    }
    
    return;
}

Section * catmullRomDecasteliju(Section* const &control, const size_t& num_of_control, const size_t& num_of_cp_per_section, const size_t& resolution)
{
    if (num_of_control < 3 || resolution < 1)
    {
        return nullptr;
    }
    
    size_t num_of_spline = resolution * (num_of_control-1);
    Section * splines = new Section [num_of_spline];

    // float * control_scale = new float [num_of_control];
    glm::vec3 * control_scale = new glm::vec3 [num_of_control];
    glm::vec3 * control_center = new glm::vec3 [num_of_control];
    glm::quat * control_orientation = new glm::quat [num_of_control];

    // control points load to each section
    for (size_t i = 0; i < num_of_control; i++)
    {
        control_scale[i] = glm::vec3(control[i].getScale(),control[i].getScale(),control[i].getScale());
        control_center[i] = control[i].getCenterPoint();
        control_orientation[i] = control[i].getOrientation();
    }

    // float * spline_scale;
    glm::vec3 * spline_scale;
    glm::vec3 * spline_center;
    glm::quat * spline_orientation;

    spline_scale = std::get<0>(openedCalculateCatmullRom(num_of_control, control_scale, resolution));
    spline_center = std::get<0>(openedCalculateCatmullRom(num_of_control, control_center, resolution));
    spline_orientation = std::get<0>(openedCalculateCatmullRom(num_of_control, control_orientation, resolution));

    delete [] control_orientation;
    delete [] control_scale;
    delete [] control_center;    

    for (size_t i = 0; i < num_of_spline; i++)
    {
        splines[i].updateControlPointsSize(num_of_cp_per_section);
        splines[i].updateResolution(resolution);
        splines[i].updateSplineName(control[i/resolution].isCatmullRomSpline());
        for (size_t point_idx = 0; point_idx < num_of_cp_per_section; point_idx++)
        {
            splines[i][point_idx] = control[i/resolution][point_idx];   
        }
        
        splines[i].updateScaleFactor(spline_scale[i].x);
        splines[i].updateOrientation(spline_orientation[i]);
        splines[i].updateCenterPoint(spline_center[i]);
        splines[i].updateModelMTX();
        splines[i].updateSplineCurve();
    }

    delete [] spline_center;
    delete [] spline_orientation;
    delete [] spline_scale;

    return splines;
}

std::ostream& operator<<(std::ostream& os, const Model& model)
{
    os << "Model Information" << std::endl;
    os << "Spline Name: " << model.spline << std::endl;
    os << "Number of Cross Sections: " << model.num_of_control_sections << std::endl;
    os << "Number of Control Points per Section: " << model.num_of_cps_per_section << std::endl;
    if (model.control_sections != nullptr)
    {
        for (size_t i = 0; i < model.num_of_control_sections; i++)
        {
            os << "NO." << i + 1 << std::endl;
            os << model.control_sections[i];
            os << "-----------------------------------------" << std::endl;
        }
    }
    return os;
}

Model::~Model()
{
    if (control_sections != nullptr)
    {
        delete [] control_sections;
    }

    if (spline_sections != nullptr)
    {
        delete [] spline_sections;
    }

    if (faces != nullptr)
    {
        for (size_t section_idx = 0; section_idx < num_of_spline_sections-1; section_idx++)
        {
            for (size_t point_idx = 0; point_idx < num_of_points_per_spline_section; point_idx++)
            {
                delete [] faces[section_idx][point_idx];
            }
            delete [] faces[section_idx];
        }
        delete [] faces;
    }
    if (vertex_normals != nullptr)
    {
        for (size_t section_idx = 0; section_idx < num_of_spline_sections; section_idx++)
        {
            delete [] vertex_normals[section_idx];
        }
        delete [] vertex_normals;
    }
       
}

void Model::generateFaces(void) 
{
    if (faces != nullptr)
    {
        for (size_t section_idx = 0; section_idx < num_of_spline_sections-1; section_idx++)
        {
            for (size_t point_idx = 0; point_idx < num_of_points_per_spline_section; point_idx++)
            {
                delete [] faces[section_idx][point_idx];
            }
            delete [] faces[section_idx];
        }
        delete [] faces;
    }

    faces = new glm::vec3**[num_of_spline_sections-1];
    
    for (size_t section_idx = 0; section_idx < num_of_spline_sections-1; section_idx++)
    {
        const glm::vec3* spline_curve = spline_sections[section_idx].getSectionSplineCurve();
        const glm::vec3* next_spline_curve = spline_sections[section_idx+1].getSectionSplineCurve();
        faces[section_idx] = new glm::vec3*[num_of_points_per_spline_section];
        for (size_t point_idx = 0; point_idx < num_of_points_per_spline_section; point_idx++)
        {
            faces[section_idx][point_idx] = new glm::vec3[2];

            glm::vec3 temp[4];
            temp[0] = spline_curve[point_idx];
            temp[1] = next_spline_curve[point_idx];
            temp[2] = next_spline_curve[point_idx < num_of_points_per_spline_section-1 ? point_idx+1 : 0];
            temp[3] = spline_curve[point_idx < num_of_points_per_spline_section-1 ? point_idx+1 : 0];

            faces[section_idx][point_idx][0] = glm::normalize(glm::cross(temp[0]-temp[3], temp[1]-temp[0]));
            faces[section_idx][point_idx][1] = glm::normalize(glm::cross(temp[2]-temp[1], temp[3]-temp[2]));

            // (*faces[section_idx][point_idx]) = glm::vec3(0.0, 0.0, 0.0);
            // for (size_t i = 0; i < 4; i++)
            // {
            //     (faces[section_idx][point_idx])->x += ((temp[i].y * temp[(i+1)%4].z) - (temp[i].z * temp[(i+1)%4].y));
            //     (faces[section_idx][point_idx])->y += ((temp[i].z * temp[(i+1)%4].x) - (temp[i].x * temp[(i+1)%4].z));
            //     (faces[section_idx][point_idx])->z += ((temp[i].x * temp[(i+1)%4].y) - (temp[i].y * temp[(i+1)%4].x));
            // }
            
            // (*faces[section_idx][point_idx]) = -(*faces[section_idx][point_idx]) / glm::length((*faces[section_idx][point_idx]));

            // glm::vec3 point_at_spline = spline_curve[point_idx];
            // glm::vec3 next_point_at_spline = spline_curve[point_idx < num_of_points_per_spline_section-1 ? point_idx+1 : 0];
            // glm::vec3 point_at_next_spline = next_spline_curve[point_idx];
            // *(faces[section_idx][point_idx]) = -glm::normalize(glm::cross(next_point_at_spline-point_at_spline, point_at_next_spline-point_at_spline));
        }
        
    }
    for (size_t section_idx = 0; section_idx < num_of_spline_sections; section_idx++)
    {
        if (section_idx == 0)
        {
            for (size_t point_idx = 0; point_idx < num_of_points_per_spline_section; point_idx++)
            {
                glm::vec3 avg_vertex_normal = faces[section_idx][point_idx > 0 ? point_idx-1 : num_of_points_per_spline_section-1][1];
                avg_vertex_normal += faces[section_idx][point_idx > 0 ? point_idx-1 : num_of_points_per_spline_section-1][0];
                avg_vertex_normal += faces[section_idx][point_idx][0]; 
                vertex_normals[section_idx][point_idx] = glm::normalize(avg_vertex_normal);
            }
        }
        else if(section_idx == num_of_spline_sections-1)
        {
            for (size_t point_idx = 0; point_idx < num_of_points_per_spline_section; point_idx++)
            {
                glm::vec3 avg_vertex_normal = faces[section_idx-1][point_idx > 0 ? point_idx-1 : num_of_points_per_spline_section-1][1];
                avg_vertex_normal += faces[section_idx-1][point_idx][0];
                avg_vertex_normal += faces[section_idx-1][point_idx][1]; 
                vertex_normals[section_idx][point_idx] = glm::normalize(avg_vertex_normal);
            }
        }
        else
        {
            for (size_t point_idx = 0; point_idx < num_of_points_per_spline_section; point_idx++)
            {
                glm::vec3 avg_vertex_normal = faces[section_idx][point_idx > 0 ? point_idx-1 : num_of_points_per_spline_section-1][0];
                avg_vertex_normal += faces[section_idx][point_idx > 0 ? point_idx-1 : num_of_points_per_spline_section-1][1];
                avg_vertex_normal += faces[section_idx][point_idx][0];
                avg_vertex_normal += faces[section_idx-1][point_idx > 0 ? point_idx-1 : num_of_points_per_spline_section-1][1];
                avg_vertex_normal += faces[section_idx-1][point_idx][0];
                avg_vertex_normal += faces[section_idx-1][point_idx][1]; 
                vertex_normals[section_idx][point_idx] = glm::normalize(avg_vertex_normal);
            }
            
        }
    }
}

void Model::draw(void) const
{
    if (control_sections == nullptr || spline_sections == nullptr)
    {
        return;
    }
    
    glLineWidth(0.05f);

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(position.x, position.y, position.z);
    glScalef(scale, scale, scale);

    // glShadeModel(GL_SMOOTH);
    // glEnable(GL_POLYGON_SMOOTH);
    // glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat.getAmbient());
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat.getDiffuse());
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat.getSpecular());
    glMaterialfv(GL_FRONT, GL_EMISSION, mat.getEmission());
    glMaterialfv(GL_FRONT, GL_SHININESS, mat.getShiness());

    for (size_t i = 0; i < num_of_spline_sections-1; i++)
    {
        /* code */
        const glm::vec3* spline_curve = spline_sections[i].getSectionSplineCurve();
        const glm::vec3* next_spline_curve = spline_sections[i+1].getSectionSplineCurve();

        
        for (size_t point_idx = 0; point_idx < num_of_points_per_spline_section; point_idx++)
        {
            glNormal3f(faces[i][point_idx][0].x, faces[i][point_idx][0].y, faces[i][point_idx][0].z);
            glBegin(GL_TRIANGLES);
            glNormal3f(vertex_normals[i][point_idx].x, vertex_normals[i][point_idx].y, vertex_normals[i][point_idx].z);
            glVertex3f(spline_curve[point_idx].x, spline_curve[point_idx].y, spline_curve[point_idx].z);
            glNormal3f(vertex_normals[i+1][point_idx].x, vertex_normals[i+1][point_idx].y, vertex_normals[i+1][point_idx].z);
            glVertex3f(next_spline_curve[point_idx].x, next_spline_curve[point_idx].y, next_spline_curve[point_idx].z);
            glNormal3f(vertex_normals[i][point_idx < num_of_points_per_spline_section-1 ? point_idx + 1 : 0].x, vertex_normals[i][point_idx < num_of_points_per_spline_section-1 ? point_idx + 1 : 0].y, vertex_normals[i][point_idx < num_of_points_per_spline_section-1 ? point_idx + 1 : 0].z);
            glVertex3f(spline_curve[point_idx < num_of_points_per_spline_section-1 ? point_idx + 1 : 0].x, spline_curve[point_idx < num_of_points_per_spline_section-1 ? point_idx + 1 : 0].y, spline_curve[point_idx < num_of_points_per_spline_section-1 ? point_idx + 1 : 0].z);
            glEnd();
            
            glNormal3f(faces[i][point_idx][1].x, faces[i][point_idx][1].y, faces[i][point_idx][1].z);
            glBegin(GL_TRIANGLES);
            glNormal3f(vertex_normals[i+1][point_idx].x, vertex_normals[i+1][point_idx].y, vertex_normals[i+1][point_idx].z);
            glVertex3f(next_spline_curve[point_idx].x, next_spline_curve[point_idx].y, next_spline_curve[point_idx].z);
            glNormal3f(vertex_normals[i+1][point_idx].x, vertex_normals[i+1][point_idx].y, vertex_normals[i+1][point_idx].z);
            glVertex3f(next_spline_curve[point_idx < num_of_points_per_spline_section-1 ? point_idx + 1 : 0].x, next_spline_curve[point_idx < num_of_points_per_spline_section-1 ? point_idx + 1 : 0].y, next_spline_curve[point_idx < num_of_points_per_spline_section-1 ? point_idx + 1 : 0].z);
            glNormal3f(vertex_normals[i][point_idx < num_of_points_per_spline_section-1 ? point_idx + 1 : 0].x, vertex_normals[i][point_idx < num_of_points_per_spline_section-1 ? point_idx + 1 : 0].y, vertex_normals[i][point_idx < num_of_points_per_spline_section-1 ? point_idx + 1 : 0].z);
            glVertex3f(spline_curve[point_idx < num_of_points_per_spline_section-1 ? point_idx + 1 : 0].x, spline_curve[point_idx < num_of_points_per_spline_section-1 ? point_idx + 1 : 0].y, spline_curve[point_idx < num_of_points_per_spline_section-1 ? point_idx + 1 : 0].z);
            glEnd();

            // glBegin(GL_QUADS);
            
            // glNormal3f(vertex_normals[i][point_idx].x, vertex_normals[i][point_idx].y, vertex_normals[i][point_idx].z);
            // glVertex3f(spline_curve[point_idx].x, spline_curve[point_idx].y, spline_curve[point_idx].z);
            // glNormal3f(vertex_normals[i+1][point_idx].x, vertex_normals[i+1][point_idx].y, vertex_normals[i+1][point_idx].z);
            // glVertex3f(next_spline_curve[point_idx].x, next_spline_curve[point_idx].y, next_spline_curve[point_idx].z);
            // glNormal3f(vertex_normals[i+1][point_idx < num_of_points_per_spline_section-1 ? point_idx + 1 : 0].x, vertex_normals[i+1][point_idx < num_of_points_per_spline_section-1 ? point_idx + 1 : 0].y, vertex_normals[i+1][point_idx < num_of_points_per_spline_section-1 ? point_idx + 1 : 0].z);
            // glVertex3f(next_spline_curve[point_idx < num_of_points_per_spline_section-1 ? point_idx + 1 : 0].x, next_spline_curve[point_idx < num_of_points_per_spline_section-1 ? point_idx + 1 : 0].y, next_spline_curve[point_idx < num_of_points_per_spline_section-1 ? point_idx + 1 : 0].z);
            // glNormal3f(vertex_normals[i][point_idx < num_of_points_per_spline_section-1 ? point_idx + 1 : 0].x, vertex_normals[i][point_idx < num_of_points_per_spline_section-1 ? point_idx + 1 : 0].y, vertex_normals[i][point_idx < num_of_points_per_spline_section-1 ? point_idx + 1 : 0].z);
            // glVertex3f(spline_curve[point_idx < num_of_points_per_spline_section-1 ? point_idx + 1 : 0].x, spline_curve[point_idx < num_of_points_per_spline_section-1 ? point_idx + 1 : 0].y, spline_curve[point_idx < num_of_points_per_spline_section-1 ? point_idx + 1 : 0].z);
            // glEnd();    
        }
        glEnd();
        // glVertex3f(spline_curve[0].x, spline_curve[0].y, spline_curve[0].z);
        // glVertex3f(next_spline_curve[0].x, next_spline_curve[0].y, next_spline_curve[0].z);
        
    }

    glPopMatrix();

    // glDisable(GL_POLYGON_SMOOTH);
    // glHint(GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);
    
    return;
}