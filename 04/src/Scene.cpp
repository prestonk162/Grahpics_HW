#include "Scene.h"

void Scene::draw(void) const
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, wallMaterial.getAmbient());
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, wallMaterial.getDiffuse());
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, wallMaterial.getSpecular());
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, wallMaterial.getEmission());
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, wallMaterial.getShiness());

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0, -1.5f, 0.0f);
    glScalef(40.0f, 1.0f, 40.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0, 18.5f, 20.5f);
    glScalef(40.0f, 40.0f, 1.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(-20.5, 18.5f, 0.0f);
    glScalef(1.0f, 40.0f, 40.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(20.5, 18.5f, 0.0f);
    glScalef(1.0f, 40.0f, 40.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0, 18.5f, -20.5f);
    glScalef(40.0f, 40.0f, 1.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    for (std::vector<Object*>::const_iterator iter = models.begin(); iter != models.end(); iter++)
    {
        (*iter)->draw();
    }
    // for (std::vector<Polygon3D*>::const_iterator iter = polygons.begin(); iter != polygons.end(); iter++)
    // {
    //     (*iter)->draw();
    // }
}

void Scene::lightSetting(void) const
{
    for (std::vector<Light *>::const_iterator iter = lights.begin(); iter != lights.end(); iter++)
    {
        (*iter)->setLighting();
    }
}

void Scene::lightOn(void) const
{
    glEnable(GL_LIGHTING);
    for (std::vector<Light *>::const_iterator iter = lights.begin(); iter != lights.end(); iter++)
    {

        glEnable((*iter)->getLightId());
    }
}

// void Scene::addPolygon(Polygon3D* polygon)
// {
//     models.push_back(polygon);
//     polygons.push_back(polygon);
// }

void Scene::addLight(Light* lightSource)
{
    lights.push_back(lightSource);
}

void Scene::addModel(Object* model)
{
    models.push_back(model);
}

Scene::Scene(/* args */)
{
    models = std::vector<Object *>();
    lights = std::vector<Light *>();
    // polygons = std::vector<Polygon3D *>();
    wallMaterial = Material();
    wallMaterial.setAmbient(0.135f, 0.2225f, 0.1575f, 0.95f);
    wallMaterial.setDiffuse(0.54f, 0.89f, 0.63f, 0.95f);
    wallMaterial.setSpecular(0.316228f, 0.316228f, 0.316228f, 0.95f);
    wallMaterial.setEmission();
    wallMaterial.setShiness(12.8);
}

Scene::~Scene()
{
    for (std::vector<Object *>::const_iterator iter = models.begin(); iter != models.end(); iter++)
    {
        if (*iter != nullptr)
        {
            delete *iter;
        }     
    }
    
    // for (std::vector<Polygon3D *>::const_iterator iter = polygons.begin(); iter != polygons.end(); iter++)
    // {
    //     if (*iter != nullptr)
    //     {
    //         delete *iter;
    //     }
    // }

    for (std::vector<Light *>::const_iterator iter = lights.begin(); iter != lights.end(); iter++)
    {
        if (*iter != nullptr)
        {
            delete *iter;
        }
    }
}
