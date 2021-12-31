#pragma once
#include <GL/glut.h>

class Light
{
private:
    /* data */
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat position[4];
    GLfloat direction[3];
    GLfloat exponent[1];
    GLfloat cutOffAngle;
    size_t light_id;
public:
    Light();
    void setAmbient(const GLfloat& r, const GLfloat& g, const GLfloat& b, const GLfloat& a);
    void setDiffuse(const GLfloat& r, const GLfloat& g, const GLfloat& b, const GLfloat& a);
    void setSpecular(const GLfloat& r, const GLfloat& g, const GLfloat& b, const GLfloat& a);
    void setPosition(const GLfloat& x, const GLfloat& y, const GLfloat& z);
    void setDirection(const GLfloat& x, const GLfloat& y, const GLfloat& z);
    void setExponent(const float& exp);
    void setLightType(const GLboolean& isPointLight);
    void setLightID(const size_t& id);
    void setLighting(void) const;
    void setCutOffAngle(const GLfloat& angle);

    const GLfloat* getAmbient(void) const;
    const GLfloat* getDiffuse(void) const;
    const GLfloat* getSpecular(void) const;
    const GLfloat* getPosition(void) const;
    const size_t& getLightId(void) const;

    ~Light();
};
