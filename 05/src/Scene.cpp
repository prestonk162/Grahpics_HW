#include "Scene.h"

void Scene::draw(void) const
{

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

void Scene::lightOff(void) const
{
    glDisable(GL_LIGHTING);
    for (std::vector<Light *>::const_iterator iter = lights.begin(); iter != lights.end(); iter++)
    {

        glDisable((*iter)->getLightId());
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
}

const std::vector<Light *>& Scene::getLightSources(void) const
{
    return lights;
}
const std::vector<Object *>& Scene::getSceneObjects(void) const
{
    return models;
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
