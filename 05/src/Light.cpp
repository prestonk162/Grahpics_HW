#include "Light.h"

Light::Light()
{
    setAmbient(0.0f, 0.0f, 0.0f, 1.0f);
    setDiffuse(0.0f, 0.0f, 0.0f, 1.0f);
    setSpecular(0.0f, 0.0f, 0.0f, 1.0f);
    setPosition(0.0f, 0.0f, 0.0f);
    setDirection(0.0f, 0.0, -1.0f);
    setLightType(true);
    setExponent(30.0f);
    setCutOffAngle(45.0f);
    light_id = GL_LIGHT0;
}

Light::~Light()
{
}

void Light::setDirection(const GLfloat& x, const GLfloat& y, const GLfloat& z)
{
    direction[0] = x;
    direction[1] = y;
    direction[2] = z;
}
void Light::setExponent(const float& exp)
{
    exponent[0] = exp < 128.0f ? exp : 128.0f;
}

void Light::setAmbient(const GLfloat& r, const GLfloat& g, const GLfloat& b, const GLfloat& a)
{
    ambient[0] = r;
    ambient[1] = g;
    ambient[2] = b;
    ambient[3] = a;
}
void Light::setDiffuse(const GLfloat& r, const GLfloat& g, const GLfloat& b, const GLfloat& a)
{
    diffuse[0] = r;
    diffuse[1] = g;
    diffuse[2] = b;
    diffuse[3] = a;
}

void Light::setSpecular(const GLfloat& r, const GLfloat& g, const GLfloat& b, const GLfloat& a)
{
    specular[0] = r;
    specular[1] = g;
    specular[2] = b;
    specular[3] = a;
}

void Light::setPosition(const GLfloat& x, const GLfloat& y, const GLfloat& z)
{
    position[0] = x;
    position[1] = y;
    position[2] = z;
}

void Light::setLightID(const size_t& id)
{
    light_id = id;
}

void Light::setLightType(const GLboolean& isPointLight)
{
    position[3] = isPointLight ? 1.0f : 0.0f;
}

void Light::setLighting(void) const
{
    glLightfv(light_id, GL_AMBIENT, ambient);
    glLightfv(light_id, GL_DIFFUSE, diffuse);
    glLightfv(light_id, GL_SPECULAR, specular);
    glLightfv(light_id, GL_POSITION, position);
    if (position[3] == 0.0f)
    {
        glLightfv(light_id, GL_SPOT_DIRECTION, direction);
        glLightfv(light_id, GL_SPOT_EXPONENT, exponent);
        glLightf(light_id, GL_SPOT_CUTOFF, cutOffAngle);
        glLightf(light_id, GL_QUADRATIC_ATTENUATION, 1.0f);
    }
}

void Light::setCutOffAngle(const GLfloat& angle)
{
    if (angle >= 0.0f && angle <= 180.0f)
    {
        cutOffAngle = angle;
    }
    else if (angle < 0.0f)
    {
        cutOffAngle = 0.0f;
    }
    else
    {
        cutOffAngle = 180.0f;
    }
    
}

const GLfloat* Light::getAmbient(void) const
{
    return ambient;
}

const GLfloat* Light::getDiffuse(void) const
{
    return diffuse;
}

const GLfloat* Light::getSpecular(void) const
{
    return specular;
}

const GLfloat* Light::getPosition(void) const
{
    return position;
}

const size_t& Light::getLightId(void) const
{
    return light_id;
}