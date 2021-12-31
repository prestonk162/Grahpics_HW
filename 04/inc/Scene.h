#include "OffModel.h"
#include "Model.h"
#include "Light.h"
#include "Object.h"
#include "Polygon3D.h"
#include <GL/glut.h>
#include <vector>

class Scene
{
private:
    /* data */
    std::vector<Object *> models;
    std::vector<Light *> lights;
    Material wallMaterial;
    // std::vector<Polygon3D *> polygons;

public:
    Scene(/* args */);
    void draw(void) const;
    void lightSetting(void) const;
    void lightOn(void) const;
    void addLight(Light* lightSource);
    void addModel(Object* model);
    // void addPolygon(Polygon3D* polygon);
    ~Scene();
};