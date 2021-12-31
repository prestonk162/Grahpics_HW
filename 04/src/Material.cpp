#include "Material.h"

Material::Material()
{
    setAmbient(0.0f, 0.0f, 0.0f, 1.0f);
    setDiffuse(0.0f, 0.0f, 0.0f, 1.0f);
    setSpecular(0.0f, 0.0f, 0.0f, 1.0f);
    setEmission(0.0f, 0.0f, 0.0f, 1.0f);
    setShiness(50.0f);
}

Material::Material(const Material& mat)
{
    setAmbient((mat.getAmbient())[0], (mat.getAmbient())[1], (mat.getAmbient())[2], (mat.getAmbient())[3]);
    setDiffuse((mat.getDiffuse())[0], (mat.getDiffuse())[1], (mat.getDiffuse())[2], (mat.getDiffuse())[3]);
    setSpecular((mat.getSpecular())[0], (mat.getSpecular())[1], (mat.getSpecular())[2], (mat.getSpecular())[3]);
    setEmission((mat.getEmission())[0], (mat.getEmission())[1], (mat.getEmission())[2], (mat.getEmission())[3]);
    setShiness((mat.getShiness())[0]);
}

Material::~Material()
{
}

Material& Material::operator=(const Material& mat)
{
    setAmbient((mat.getAmbient())[0], (mat.getAmbient())[1], (mat.getAmbient())[2], (mat.getAmbient())[3]);
    setDiffuse((mat.getDiffuse())[0], (mat.getDiffuse())[1], (mat.getDiffuse())[2], (mat.getDiffuse())[3]);
    setSpecular((mat.getSpecular())[0], (mat.getSpecular())[1], (mat.getSpecular())[2], (mat.getSpecular())[3]);
    setEmission((mat.getEmission())[0], (mat.getEmission())[1], (mat.getEmission())[2], (mat.getEmission())[3]);
    setShiness((mat.getShiness())[0]);

    return *this;
}

void Material::setAmbient(const GLfloat& r, const GLfloat& g, const GLfloat& b, const GLfloat& a)
{
    ambient[0] = r;
    ambient[1] = g;
    ambient[2] = b;
    ambient[3] = a;
}
void Material::setDiffuse(const GLfloat& r, const GLfloat& g, const GLfloat& b, const GLfloat& a)
{
    diffuse[0] = r;
    diffuse[1] = g;
    diffuse[2] = b;
    diffuse[3] = a;
}

void Material::setSpecular(const GLfloat& r, const GLfloat& g, const GLfloat& b, const GLfloat& a)
{
    specular[0] = r;
    specular[1] = g;
    specular[2] = b;
    specular[3] = a;
}

void Material::setEmission(const GLfloat& r, const GLfloat& g, const GLfloat& b, const GLfloat& a)
{
    emission[0] = r;
    emission[1] = g;
    emission[2] = b;
    emission[3] = a;
}

void Material::setShiness(const GLfloat& s)
{
    shiness[0] = s;
}

const GLfloat* Material::getAmbient(void) const
{
    return ambient;
}

const GLfloat* Material::getDiffuse(void) const
{
    return diffuse;
}

const GLfloat* Material::getSpecular(void) const
{
    return specular;
}

const GLfloat* Material::getEmission(void) const
{
    return emission;
}

const GLfloat* Material::getShiness(void) const
{
    return shiness;
}