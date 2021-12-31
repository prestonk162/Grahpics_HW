#pragma once
#include <GL/glut.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <FreeImage.h>
#include <string>
#include "ScreenCapture.h"
#include "Object.h"

class ConcreteSphere : public Object
{
private:
    /* data */
    GLboolean textureLoadEnable;
    GLint imageHeight;
    GLint imageWidth;
    GLuint textureID;
    std::string filename;
    GLubyte * textureData;
    GLUquadric * sphere;
public:
    ConcreteSphere(/* args */);
    ~ConcreteSphere();
    void generateTexture(void);
    void initTexture(void);
    void draw(void) const;
    void setTextureID(const GLuint& id);
    void setFilename(const std::string& name);
};
