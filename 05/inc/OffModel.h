#pragma once
#include <string>
#include <cstring>
#include <iostream>
#include <glm/glm.hpp>
#include <fstream>
#include "Material.h"
#include <tuple>
#include "Object.h"
#include <list>

typedef glm::dvec3 faceNormal;
typedef glm::dvec3 vertex;

class OffModel : public Object
{
private:
    /* data */
    std::string filename;
    
    size_t num_of_vertices;
    glm::dvec3* vertices;

    size_t num_of_faces;
    std::tuple<size_t, size_t*, faceNormal>* faces;
    std::list<glm::dvec3> * vertex_normals_list;
    glm::dvec3* vertices_normals;
public:
    // Material mat;
    OffModel(/* args */);
    ~OffModel();
    void draw(void) const;
    void loadModel(const std::string& name);
    void generateFaceNormals(void);
};

