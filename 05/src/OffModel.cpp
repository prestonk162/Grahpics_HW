#include "OffModel.h"

OffModel::OffModel(/* args */)
{
    num_of_vertices = 0;
    num_of_faces = 0;
    vertices = nullptr;
    faces = nullptr;
    vertex_normals_list = nullptr;
    vertices_normals = nullptr;
}

void OffModel::generateFaceNormals(void)
{
    if (num_of_vertices < 3 || num_of_faces == 0)
    {
        return;
    }

    vertex_normals_list = new std::list<glm::dvec3>[num_of_vertices];
    vertices_normals = new glm::dvec3 [num_of_vertices];
    
    for (size_t face_idx = 0; face_idx < num_of_faces; face_idx++)
    {
        size_t num_of_vertices_per_face = std::get<0>(faces[face_idx]);
        const size_t* vertices_per_face = std::get<1>(faces[face_idx]);
        glm::dvec3 face_normal = glm::dvec3(0.0f, 0.0f, 0.0f);
        for (size_t i = 0; i < num_of_vertices_per_face; i++)
        {
            glm::dvec3 cur_vertex = vertices[vertices_per_face[i]];
            glm::dvec3 next_vertex = vertices[vertices_per_face[(i+1) % num_of_vertices_per_face]];
            glm::dvec3 next_next_vertex = vertices[vertices_per_face[(i+2) % num_of_vertices_per_face]];

            glm::dvec3 v1 = next_vertex - cur_vertex;
            glm::dvec3 v2 = next_next_vertex - next_vertex;

            face_normal.x += ((v1.y * v2.z) - (v1.z * v2.y));
            face_normal.y += ((v1.z * v2.x) - (v1.x * v2.z));
            face_normal.z += ((v1.x * v2.y) - (v1.y * v2.x));

            // face_normal.x += ((cur_vertex.y * next_vertex.z) - (cur_vertex.z * next_vertex.y));
            // face_normal.y += ((cur_vertex.z * next_vertex.x) - (cur_vertex.x * next_vertex.z));
            // face_normal.z += ((cur_vertex.x * next_vertex.y) - (cur_vertex.y * next_vertex.x));
            
        }
        std::get<2>(faces[face_idx]) = glm::normalize(face_normal);
        for (size_t i = 0; i < num_of_vertices_per_face; i++)
        {
            vertex_normals_list[vertices_per_face[i]].push_back(glm::normalize(face_normal));
        }
        
        // glm::dvec3 v0 = vertices[std::get<1>(faces[face_idx])[0]];
        // glm::dvec3 v1 = vertices[std::get<1>(faces[face_idx])[1]];
        // glm::dvec3 v2 = vertices[std::get<1>(faces[face_idx])[2]];
        // std::get<2>(faces[face_idx]) = -glm::normalize(glm::cross(v0-v1, v2-v1));
    }
    for (size_t i = 0; i < num_of_vertices; i++)
    {
        glm::dvec3 sum = glm::dvec3(0.0f, 0.0f, 0.0f);
        for (std::list<glm::dvec3>::const_iterator iter = vertex_normals_list[i].begin() ; iter != vertex_normals_list[i].end(); iter++)
        {
            sum += *iter;
        }
        vertices_normals[i] = glm::normalize(sum);
    }
    
    
}

void OffModel::loadModel(const std::string& name)
{
    filename = name;

    std::fstream fp;
    fp.open(filename, std::fstream::in);

    if(fp.is_open()) {
        const int linesize = 128;
        char line[linesize];

        char * token = NULL;
        char * next_token = NULL;

        // float cp_x;
        // float cp_y;

        size_t cur_line = 1;
        size_t section_idx = 0;
        size_t vertex_idx = 0;
        size_t face_idx;

        while(fp.getline(line, linesize)) {
            if(std::string(line).length() > 1) {
                token = NULL;
                next_token = NULL;
                
                token = strtok_r(line, " ", &next_token);
                if (token[0] == '#')
                {
                    continue;
                }
                

                if (cur_line == 1)
                {
                    if (std::string(token) != std::string("OFF"))
                    {
                        fp.close();
                        std::cout << "Not OFF Format" << std::endl;
                        return;
                    }
                    // else
                    // {
                    //     std::cout << "OFF Format Checked" << std::endl;
                    // }
                }
                else if (cur_line == 2)
                {
                    char * endpos = NULL;
                    num_of_vertices = strtoul(token, &endpos, 10);
                    token = strtok_r(NULL, " ", &next_token);
                    num_of_faces = strtoul(token, &endpos, 10);
                    
                    vertices = new vertex [num_of_vertices];
                    faces = new std::tuple<size_t, size_t*, faceNormal> [num_of_faces];
                }
                else
                {
                    if(vertex_idx < num_of_vertices)
                    {
                        vertices[vertex_idx].x = strtof(token, NULL);
                        token = strtok_r(NULL, " ", &next_token);

                        if (token == NULL || token[0] == '#') break;
                        vertices[vertex_idx].y = strtof(token, NULL);
                        token = strtok_r(NULL, " ", &next_token);

                        if (token == NULL || token[0] == '#') break;
                        vertices[vertex_idx].z = strtof(token, NULL);

                        vertex_idx++;
                    }
                    else if (face_idx < num_of_faces)
                    {
                        size_t vertices_num_per_face = strtoul(token, NULL, 10);
                        std::get<0>(faces[face_idx]) = vertices_num_per_face;
                        std::get<1>(faces[face_idx]) = new size_t [vertices_num_per_face];

                        for (size_t i = 0; i < vertices_num_per_face; i++)
                        {
                            token = strtok_r(NULL, " ", &next_token);
                            if (token == NULL || token[0] == '#') break;
                            else std::get<1>(faces[face_idx])[i] = strtoul(token, NULL, 10);
                        }

                        face_idx++;
                    }
                }
                cur_line++;
            }
            else
            {
                if (cur_line > 2 && vertex_idx == num_of_vertices && face_idx == num_of_faces)
                {
                    break;
                }
            }
        }
        generateFaceNormals();
        fp.close();
    }
    else {
        std::cout << "File Opening Failed" << std::endl;
    }
}

void OffModel::draw(void) const
{
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(position.x, position.y, position.z);
    glScalef(scale, scale, scale);   
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat.getAmbient());
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat.getDiffuse());
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat.getSpecular());
    glMaterialfv(GL_FRONT, GL_EMISSION, mat.getEmission());
    glMaterialfv(GL_FRONT, GL_SHININESS, mat.getShiness());

    // glShadeModel(GL_SMOOTH);
    // glEnable(GL_POLYGON_SMOOTH);
    // glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    for (size_t face_idx = 0; face_idx < num_of_faces; face_idx++)
    {
        std::tuple<size_t, size_t*, faceNormal> face = faces[face_idx];
        faceNormal f_normal = std::get<2>(face);
        size_t num_of_vertex_per_face = std::get<0>(face);
        const size_t * vertices_array_per_face = std::get<1>(face);

        num_of_vertex_per_face == 3 ? glBegin(GL_TRIANGLES) : num_of_vertex_per_face == 4 ? glBegin(GL_QUADS) : glBegin(GL_POLYGON);
        glNormal3f(f_normal.x, f_normal.y, f_normal.z);
        for (size_t vertex_idx = 0; vertex_idx < num_of_vertex_per_face; vertex_idx++)
        {
            vertex vertex_per_face = vertices[vertices_array_per_face[vertex_idx]];
            glm::dvec3 vertex_normal = vertices_normals[vertices_array_per_face[vertex_idx]];
            glNormal3d(vertex_normal.x, vertex_normal.y, vertex_normal.z);
            glVertex3d(vertex_per_face.x, vertex_per_face.y, vertex_per_face.z);
        }
        glEnd();
    }
    glPopMatrix();

    // glDisable(GL_POLYGON_SMOOTH);
    // glHint(GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);
}

OffModel::~OffModel()
{
    if (vertices != nullptr)
    {
        delete [] vertices;
    }
    
    if (faces != nullptr)
    {
        for (size_t face_idx = 0; face_idx < num_of_faces; face_idx++)
        {
            delete [] std::get<1>(faces[face_idx]);
        }
        delete [] faces;
    }
    if (vertex_normals_list != nullptr)
    {
        for (size_t i = 0; i < num_of_vertices; i++)
        {
            vertex_normals_list[i].clear();
        }
        
        delete [] vertex_normals_list;
    }
    if (vertices_normals != nullptr)
    {
        delete [] vertices_normals;
    }
    
    
}