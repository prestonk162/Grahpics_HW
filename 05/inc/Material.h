#pragma once
#include <GL/glut.h>

class Material
{
private:
    /* data */
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat emission[4];
    GLfloat shiness[1];
    GLfloat reflectioness;
    GLboolean opaque;
    GLfloat index_of_refraction;
public:
    Material();
    Material(const Material& mat);
    void setAmbient(const GLfloat& r = 0.0f, const GLfloat& g = 0.0f, const GLfloat& b = 0.0f, const GLfloat& a = 1.0f);
    void setDiffuse(const GLfloat& r = 0.0f, const GLfloat& g = 0.0f, const GLfloat& b = 0.0f, const GLfloat& a = 1.0f);
    void setSpecular(const GLfloat& r = 0.0f, const GLfloat& g = 0.0f, const GLfloat& b = 0.0f, const GLfloat& a = 1.0f);
    void setEmission(const GLfloat& r = 0.0f, const GLfloat& g = 0.0f, const GLfloat& b = 0.0f, const GLfloat& a = 1.0f);
    void setShiness(const GLfloat& s = 50.0f);
    void setReflectioness(const GLfloat& r = 0.0f);
    void setOpaqueness(const GLboolean& isOpaque);
    void setIndexOfRefraction(const GLfloat& n = 0.0f);
    Material& operator=(const Material& mat);

    const GLfloat* getAmbient(void) const;
    const GLfloat* getDiffuse(void) const;
    const GLfloat* getSpecular(void) const;
    const GLfloat* getEmission(void) const;
    const GLfloat* getShiness(void) const;
    const GLfloat getReflectioness(void) const;
    const GLfloat getIndexOfRefraction(void) const;
    const GLboolean isOpaque(void) const;
    ~Material();
};
