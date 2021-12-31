#pragma once
#include <GL/glut.h>
#include <GL/gl.h>
#include <FreeImage.h>
#include <iostream>
#include "Object.h"
#include <string>
#include "ScreenCapture.h"

class Box : public Object
{
private:
    /* data */
    GLboolean textureLoadEnable;
    GLint imageHeight;
    GLint imageWidth;
    GLuint textureID;
    std::string filename;
    GLubyte * textureData;
public:
    Box(/* args */);
    
    void generateTexture(void);
    void initTexture(void);
    void draw(void) const;
    void setTextureID(const GLuint& id);
    void setFilename(const std::string& name);
    ~Box();
};
