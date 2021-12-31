#include "RayTracing.h"

void RayTracing::calculatePixelCoord(const GLint& w, const GLint& h)
{
    if(w <= 0 || h <= 0) return;

    if (pixelWorldCoords != nullptr)
    {
        for (size_t i = 0; i < height; i++)
        {
            delete [] pixelWorldCoords[i];
        }
        delete [] pixelWorldCoords;
    }

    if (pixelColorBuffer != nullptr) delete [] pixelColorBuffer;
    if (pixelColorByteBuffer != nullptr) delete [] pixelColorByteBuffer;
    

    if (primaryRays != nullptr)
    {
        for (size_t i = 0; i < height; i++)
        {
            delete [] primaryRays[i];
        }
        delete [] primaryRays;
    }
    

    width = w;
    height = h;

    pixelWorldCoords = new glm::dvec3 * [height];
    primaryRays = new Ray * [height];
    pixelColorBuffer = new GLfloat [width * height * 3];
    pixelColorByteBuffer = new GLubyte [width * height * 3];

    for (size_t i = 0; i < height; i++)
    {
        pixelWorldCoords[i] = new glm::dvec3 [width];
        primaryRays[i] = new Ray [width];
    }

    GLdouble modelMTX[16];
    GLdouble projMTX[16];
    GLint viewport[4];

    glGetDoublev(GL_MODELVIEW_MATRIX, modelMTX);
    glGetDoublev(GL_PROJECTION_MATRIX, projMTX);
    glGetIntegerv(GL_VIEWPORT, viewport);

    GLdouble * pixel_x = new GLdouble;
    GLdouble * pixel_y = new GLdouble;
    GLdouble * pixel_z = new GLdouble;
    
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            gluUnProject((GLdouble)x+0.5, (GLdouble) height - y - 0.5, 1.0, modelMTX, projMTX, viewport, pixel_x, pixel_y, pixel_z);
            pixelWorldCoords[y][x].x = *pixel_x;
            pixelWorldCoords[y][x].y = *pixel_y;
            pixelWorldCoords[y][x].z = *pixel_z;
            primaryRays[y][x].changeStartPoint(cameraPos);
            primaryRays[y][x].changeDirbyNewEndPoint(pixelWorldCoords[y][x]);
        }
    }

    delete pixel_x;
    delete pixel_y;
    delete pixel_z;

    std::cout << "Calculating Pixel Coordinates Finished!" << std::endl;
}

RayTracing::RayTracing(/* args */)
{
    height = 0;
    width = 0;
    pixelWorldCoords = nullptr;
    primaryRays = nullptr;
    pixelColorBuffer = nullptr;
    pixelColorByteBuffer = nullptr;
    cameraPos = glm::dvec3();
}

RayTracing::RayTracing(const GLint& w, const GLint& h, const glm::dvec3& cameraOrigin)
{
    pixelWorldCoords = nullptr;
    primaryRays = nullptr;
    pixelColorBuffer = nullptr;
    pixelColorByteBuffer = nullptr;
    cameraPos = cameraOrigin;
    calculatePixelCoord(w, h);
}

Color RayTracing::localColor(const Ray& inputRay, const glm::dvec3& normal, const glm::dvec3& intersectPoint, const Scene& scene, const Material& objMat, Object* obj) const
{
    Color localColor = Color();
    std::vector<Object *> objVector = scene.getSceneObjects();
    std::vector<Light *> lightSources = scene.getLightSources();

    if (obj->getObjType() == OBJECT_TYPE::TEXTURE_BOX || obj->getObjType() == OBJECT_TYPE::TEXTURE_SPHERE)
    {
        GLdouble modelMTX[16];
        GLdouble projMTX[16];
        GLint viewport[4];

        glGetDoublev(GL_MODELVIEW_MATRIX, modelMTX);
        glGetDoublev(GL_PROJECTION_MATRIX, projMTX);
        glGetIntegerv(GL_VIEWPORT, viewport);
        GLdouble * pixel_x = new GLdouble;
        GLdouble * pixel_y = new GLdouble;
        GLdouble * pixel_z = new GLdouble;

        gluProject(intersectPoint.x, intersectPoint.y, intersectPoint.z, modelMTX, projMTX, viewport, pixel_x, pixel_y, pixel_z);

        BYTE* pixels = new BYTE[3];
        glReadPixels((GLint)(*pixel_x), (GLint)(*pixel_y), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        localColor.r += (GLfloat)pixels[0] / 255.0f;
        localColor.g += (GLfloat)pixels[1] / 255.0f;
        localColor.b += (GLfloat)pixels[2] / 255.0f;

        delete [] pixels;
        delete pixel_x;
        delete pixel_y;
        delete pixel_z;
        return localColor;
    }

    for (std::vector<Light *>::const_iterator lightIter = lightSources.begin(); lightIter != lightSources.end(); lightIter++)
    {
    
        glm::dvec3 lightSourceCenter = glm::dvec3((*lightIter)->getPosition()[0], (*lightIter)->getPosition()[1], (*lightIter)->getPosition()[2]);
        Ray shadowRay = Ray();
        shadowRay.changeStartPoint(intersectPoint);
        shadowRay.changeDirbyNewEndPoint(lightSourceCenter);
        GLdouble temp_t = (lightSourceCenter - shadowRay.getRayOrigin()).length();                

        GLboolean isShadowed = false;
        for (std::vector<Object *>::const_iterator objectIter = objVector.begin(); objectIter != objVector.end(); objectIter++)
        {
            if ((*objectIter)->getObjType() == OBJECT_TYPE::POLYGON3D_OBJECT)
            {
                Polygon3D * temp = dynamic_cast<Polygon3D *>(*objectIter);
                if (temp->getPolygonType() == POLYGON_TYPE::SPHERE)
                {
                    IntersectionInfo intersectInfo = shadowRay.intersectWithSphere(temp->position, (temp->o_radius)*0.9999);
                    if (std::get<0>(intersectInfo) && std::get<1>(intersectInfo) < temp_t + glm::epsilon<GLdouble>())
                    {
                        isShadowed = true;
                        break;
                    }
                }
                else if (temp->getPolygonType() == POLYGON_TYPE::CUBE)
                {
                    IntersectionPolygonInfo intersectInfo = shadowRay.intersectWithPolygon(*temp);
                    if (std::get<0>(intersectInfo) && std::get<1>(intersectInfo) < temp_t + glm::epsilon<GLdouble>())
                    {
                        isShadowed = true;
                        break;
                    }
                }
                else
                {
                    continue;
                }
            }
            else if ((*objectIter)->getObjType() == OBJECT_TYPE::TEXTURE_BOX)
            {
                Box * temp = dynamic_cast<Box *>(*objectIter);
                IntersectionPolygonInfo intersectInfo = shadowRay.intersectWithTextureBox(*temp);
                if (std::get<0>(intersectInfo) && std::get<1>(intersectInfo) < temp_t + glm::epsilon<GLdouble>())
                {
                    isShadowed = true;
                    break;
                }
            }
            else if ((*objectIter)->getObjType() == OBJECT_TYPE::TEXTURE_SPHERE)
            {
                IntersectionInfo intersectInfo = shadowRay.intersectWithSphere((*objectIter)->position, ((*objectIter)->scale)*0.9999);
                if (std::get<0>(intersectInfo) && std::get<1>(intersectInfo) < temp_t + glm::epsilon<GLdouble>())
                {
                    isShadowed = true;
                    break;
                }
            }
            else
            {
                continue;
            }
        }

        Color lightAmbient = Color((*lightIter)->getAmbient()[0], (*lightIter)->getAmbient()[1], (*lightIter)->getAmbient()[2]);
        Color objAmbient = Color(objMat.getAmbient()[0], objMat.getAmbient()[1],objMat.getAmbient()[2]);

        Color colorFromLightSource = Color(lightAmbient.r*objAmbient.r, lightAmbient.g*objAmbient.g, lightAmbient.b*objAmbient.b);
        if(!isShadowed)
        {

            Color lightDiffuse = Color((*lightIter)->getDiffuse()[0], (*lightIter)->getDiffuse()[1], (*lightIter)->getDiffuse()[2]);
            Color lightSpecular = Color((*lightIter)->getSpecular()[0], (*lightIter)->getSpecular()[1], (*lightIter)->getSpecular()[2]);
            Color objDiffuse = Color(objMat.getDiffuse()[0], objMat.getDiffuse()[1], objMat.getDiffuse()[2]);
            Color objSpecular = Color(objMat.getSpecular()[0], objMat.getSpecular()[1], objMat.getSpecular()[2]);
            GLfloat objShiness = objMat.getShiness()[0];

            glm::dvec3 shadowRayReflectDir = glm::normalize(2.0*(glm::dot(shadowRay.getRayDirection(), normal))*normal-shadowRay.getRayDirection());

            // colorFromLightSource += ((lightDiffuse * objDiffuse) * glm::abs(glm::dot(normalIntersection, shadowRay.getRayDirection())));
            // colorFromLightSource += ((lightSpecular * objSpecular) * glm::abs(glm::pow(glm::dot(primaryRays[y][x].getRayDirection(), shadowRayReflectDir), objShiness)));
            
            colorFromLightSource.r += lightDiffuse.r*objDiffuse.r*glm::abs(glm::dot(normal, shadowRay.getRayDirection()))+lightSpecular.r*objSpecular.r*pow(glm::abs(glm::dot(-inputRay.getRayDirection(), shadowRayReflectDir)), (GLdouble)objShiness);
            colorFromLightSource.g += lightDiffuse.g*objDiffuse.g*glm::abs(glm::dot(normal, shadowRay.getRayDirection()))+lightSpecular.g*objSpecular.g*pow(glm::abs(glm::dot(-inputRay.getRayDirection(), shadowRayReflectDir)), (GLdouble)objShiness);
            colorFromLightSource.b += lightDiffuse.b*objDiffuse.b*glm::abs(glm::dot(normal, shadowRay.getRayDirection()))+lightSpecular.b*objSpecular.b*pow(glm::abs(glm::dot(-inputRay.getRayDirection(), shadowRayReflectDir)), (GLdouble)objShiness);
        }
        localColor.r += colorFromLightSource.r;
        localColor.g += colorFromLightSource.g;
        localColor.b += colorFromLightSource.b;
    }
    return Color(localColor.r * objMat.getReflectioness(), localColor.g * objMat.getReflectioness(), localColor.b * objMat.getReflectioness());
}

Color RayTracing::reflectHandling(const Ray& inputRay, const glm::dvec3& normal, const glm::dvec3& intersectPoint, const GLuint& depth, const Scene& scene, const Material& objMat, Object* obj) const
{
    const std::vector<Object*> objVector = scene.getSceneObjects();
    const std::vector<Light *> lightSources = scene.getLightSources();

    if (depth < 3)
    {
        Ray reflectRay = Ray();
        reflectRay.changeStartPoint(intersectPoint);
        reflectRay.changeDir(glm::normalize(-2.0f*glm::dot(normal, inputRay.getRayDirection())*normal + inputRay.getRayDirection()));
        
        
        Object * temp_obj = nullptr;
        GLdouble temp_t = reflectRay.getMaximumStep();
        
        glm::dvec3 cubeNormal = glm::dvec3(0,0,0);
        // Check Intersection Object with Reflected Ray
        for (std::vector<Object *>::const_iterator iter = objVector.begin(); iter != objVector.end(); iter++)
        {
            if ((*iter)->getObjType() == OBJECT_TYPE::POLYGON3D_OBJECT)
            {
                Polygon3D * temp_polygon = dynamic_cast<Polygon3D *>(*iter);
                if (temp_polygon->getPolygonType() == POLYGON_TYPE::CUBE)
                {
                    IntersectionPolygonInfo intersectionInfo = reflectRay.intersectWithPolygon(*temp_polygon);
                    if (std::get<0>(intersectionInfo) && std::get<1>(intersectionInfo) < temp_t + glm::epsilon<GLdouble>() && std::get<1>(intersectionInfo) > 0)
                    {
                        temp_obj = temp_polygon;
                        temp_t = std::get<1>(intersectionInfo);
                        cubeNormal = std::get<2>(intersectionInfo);
                    }
                    
                }
                else if (temp_polygon->getPolygonType() == POLYGON_TYPE::SPHERE)
                {
                    IntersectionInfo intersectionInfo = reflectRay.intersectWithSphere(temp_polygon->position, (temp_polygon->o_radius)*0.9999);
                    if (std::get<0>(intersectionInfo) && std::get<1>(intersectionInfo) < temp_t + glm::epsilon<GLdouble>() && std::get<1>(intersectionInfo) > 0)
                    {
                        temp_obj = temp_polygon;
                        temp_t = std::get<1>(intersectionInfo);
                        cubeNormal = glm::normalize(reflectRay.getRayOrigin()+temp_t*reflectRay.getRayDirection()-temp_polygon->position);
                    }
                }
                else
                {
                    continue;
                }
            }
            else if((*iter)->getObjType() == OBJECT_TYPE::TEXTURE_BOX)
            {
                Box * temp_polygon = dynamic_cast<Box *>(*iter);
                IntersectionPolygonInfo intersectInfo = reflectRay.intersectWithTextureBox(*temp_polygon);
                if(std::get<0>(intersectInfo) && std::get<1>(intersectInfo) < temp_t + glm::epsilon<GLdouble>() && std::get<1>(intersectInfo) > 0)
                {
                    temp_t = std::get<1>(intersectInfo);
                    temp_obj = temp_polygon;
                    cubeNormal = std::get<2>(intersectInfo);
                }
            }
            else if((*iter)->getObjType() == OBJECT_TYPE::TEXTURE_SPHERE)
            {
                IntersectionInfo intersectionInfo = reflectRay.intersectWithSphere((*iter)->position, ((*iter)->scale)*0.9999);
                if (std::get<0>(intersectionInfo) && std::get<1>(intersectionInfo) < temp_t + glm::epsilon<GLdouble>() && std::get<1>(intersectionInfo) > 0)
                {
                    temp_t = std::get<1>(intersectionInfo);
                    temp_obj = (*iter);
                    cubeNormal = glm::normalize(reflectRay.getRayOrigin()+temp_t*reflectRay.getRayDirection()-(*iter)->position);
                }
            }
            else
            {
                continue;
            }
        }
        // Intersection Object with Reflection Exist
        Color reflectionColor = Color();
        Color lightSourceColor = Color();
        Color refractionColor = Color();
        if(temp_obj != nullptr)
        {
            glm::dvec3 intersectPointWithReflect = intersectPoint + reflectRay.getRayDirection()*temp_t;
            // if(temp_obj->getObjType() == OBJECT_TYPE::TEXTURE_SPHERE || temp_obj->getObjType() == OBJECT_TYPE::TEXTURE_BOX)
            // {
            //     reflectionColor = Color(0,0,0);
            //     refractionColor = Color(0,0,0);
            // }
            // else
            // {
            //     reflectionColor = reflectHandling(reflectRay, cubeNormal, intersectPointWithReflect, depth+1, scene, temp_obj->mat, temp_obj);
            //     refractionColor = refractionHandling(reflectRay, cubeNormal, intersectPointWithReflect, depth+1, scene, temp_obj->mat, temp_obj);
            // }
            reflectionColor = reflectHandling(reflectRay, cubeNormal, intersectPointWithReflect, depth+1, scene, temp_obj->mat, temp_obj);
            refractionColor = refractionHandling(reflectRay, cubeNormal, intersectPointWithReflect, depth+1, scene, temp_obj->mat, temp_obj);
            lightSourceColor = localColor(reflectRay, cubeNormal, intersectPointWithReflect, scene, temp_obj->mat, temp_obj);
        }
        
        return Color(lightSourceColor.r+reflectionColor.r*objMat.getSpecular()[0]+refractionColor.r, lightSourceColor.g+reflectionColor.g*objMat.getSpecular()[1]+refractionColor.g, lightSourceColor.b+reflectionColor.b*objMat.getSpecular()[2]+refractionColor.b);
    }
    else
    {
        return localColor(inputRay, normal, intersectPoint, scene, objMat, obj);
    }
}

Color RayTracing::refractionHandling(const Ray& inputRay, const glm::dvec3& normal, const glm::dvec3& intersectPoint, const GLuint& depth, const Scene& scene, const Material& objMat, Object* obj) const
{
    const std::vector<Object*> objVector = scene.getSceneObjects();
    const std::vector<Light *> lightSources = scene.getLightSources();

    Color transcluentColor = Color();
    Color reflectionColor = Color();
    Color lightSourceColor = Color();

    // Refraction Color Calculation
    if (depth < 3 && obj->mat.isOpaque())
    {
        Ray innerRefraction = Ray();
        GLdouble sin_theta = (-inputRay.getRayDirection()+glm::dot(inputRay.getRayDirection(),normal)*normal).length();
        
        glm::dvec3 innerRefractionDir = inputRay.getRayDirection()*(1.0/(GLdouble)obj->mat.getIndexOfRefraction());
        innerRefractionDir -= (glm::dot(normal, inputRay.getRayDirection())-sqrt(1-pow(sin_theta/obj->mat.getIndexOfRefraction(),2)))*normal;
        innerRefractionDir = glm::normalize(innerRefractionDir);
        innerRefraction.changeDir(innerRefractionDir);

        innerRefraction.changeStartPoint(intersectPoint+glm::epsilon<GLdouble>()*2.0);

        GLdouble inner_t;
        glm::dvec3 outerIntersectionPoint;
        glm::dvec3 outerNormal;
        if (obj->getObjType() == OBJECT_TYPE::TEXTURE_SPHERE)
        {
            inner_t = std::get<1>(innerRefraction.intersectWithSphere(obj->position, obj->scale));
            outerIntersectionPoint = innerRefraction.getRayOrigin() + inner_t * innerRefraction.getRayDirection();
            outerNormal = glm::normalize(outerIntersectionPoint - obj->position);
        }
        else if(obj->getObjType() == OBJECT_TYPE::TEXTURE_BOX)
        {
            Box * temp_polygon = dynamic_cast<Box *>(obj);
            IntersectionPolygonInfo info = innerRefraction.intersectWithTextureBox(*temp_polygon);
            inner_t = std::get<1>(info);
            outerIntersectionPoint = innerRefraction.getRayOrigin() + inner_t * innerRefraction.getRayDirection();
            outerNormal = std::get<2>(info);
        }
        else if(obj->getObjType() == OBJECT_TYPE::POLYGON3D_OBJECT)
        {
            Polygon3D * temp_polygon = dynamic_cast<Polygon3D *>(obj);
            if (temp_polygon -> getPolygonType() == POLYGON_TYPE::SPHERE)
            {
                inner_t = std::get<1>(innerRefraction.intersectWithSphere(obj->position, obj->scale));
                outerIntersectionPoint = innerRefraction.getRayOrigin() + inner_t * innerRefraction.getRayDirection();
                outerNormal = glm::normalize(outerIntersectionPoint - obj->position);
            }
            else if(temp_polygon -> getPolygonType() == POLYGON_TYPE::CUBE)
            {
                IntersectionPolygonInfo info = innerRefraction.intersectWithPolygon(*temp_polygon);
                inner_t = std::get<1>(info);
                outerIntersectionPoint = innerRefraction.getRayOrigin() + inner_t * innerRefraction.getRayDirection();
                outerNormal = std::get<2>(info);
            }
            else
            {
                return localColor(inputRay, normal, intersectPoint, scene, objMat, obj);
            }
        }
        else
        {
            return localColor(inputRay, normal, intersectPoint, scene, objMat, obj);
        }
        Ray refractionRay = Ray();
        GLdouble sin_theta_out = (-innerRefractionDir+glm::dot(innerRefractionDir, outerNormal)*outerNormal).length();
        outerNormal *= -1.0;

        glm::dvec3 refractionDir = innerRefraction.getRayDirection()*((GLdouble)obj->mat.getIndexOfRefraction());
        refractionDir -= (glm::dot(outerNormal, innerRefraction.getRayDirection())-sqrt(1-pow(sin_theta_out*obj->mat.getIndexOfRefraction(),2)))*outerNormal;
        refractionDir = glm::normalize(innerRefractionDir);
        refractionRay.changeDir(refractionDir);
        refractionRay.changeStartPoint(outerIntersectionPoint + 2.0 * glm::epsilon<GLdouble>()*refractionDir);

        GLdouble temp_t = refractionRay.getMaximumStep();
        Object * temp_obj = nullptr;
        glm::dvec3 cubeNormal = glm::dvec3();

        for (std::vector<Object *>::const_iterator objectIter = objVector.begin(); objectIter != objVector.end(); objectIter++)
        {
            if(*objectIter == obj) continue;

            if ((*objectIter)->getObjType() == OBJECT_TYPE::POLYGON3D_OBJECT)
            {
                Polygon3D * temp = dynamic_cast<Polygon3D *>(*objectIter);
                if (temp->getPolygonType() == POLYGON_TYPE::SPHERE)
                {
                    IntersectionInfo intersectInfo = refractionRay.intersectWithSphere(temp->position, temp->o_radius);
                    if (std::get<0>(intersectInfo) && std::get<1>(intersectInfo) < temp_t + glm::epsilon<GLdouble>() && std::get<1>(intersectInfo) > 0)
                    {
                        temp_t = std::get<1>(intersectInfo);
                        temp_obj = temp;
                        cubeNormal = glm::normalize(refractionRay.getRayOrigin()+temp_t*refractionRay.getRayDirection()-(temp)->position);
                    }
                }
                else if (temp->getPolygonType() == POLYGON_TYPE::CUBE)
                {
                    IntersectionPolygonInfo intersectInfo = refractionRay.intersectWithPolygon(*temp);
                    if(std::get<0>(intersectInfo) && std::get<1>(intersectInfo) < temp_t + glm::epsilon<GLdouble>() && std::get<1>(intersectInfo) > 0)
                    {
                        temp_t = std::get<1>(intersectInfo);
                        temp_obj = temp;
                        cubeNormal = std::get<2>(intersectInfo);
                    }
                }
                else
                {
                    continue;
                }
            }
            else if ((*objectIter)->getObjType() == OBJECT_TYPE::TEXTURE_BOX)
            {
                Box * temp = dynamic_cast<Box *>(*objectIter);
                IntersectionPolygonInfo intersectInfo = refractionRay.intersectWithTextureBox(*temp);
                if(std::get<0>(intersectInfo) && std::get<1>(intersectInfo) < temp_t + glm::epsilon<GLdouble>() && std::get<1>(intersectInfo) > 0)
                {
                    temp_t = std::get<1>(intersectInfo);
                    temp_obj = temp;
                    cubeNormal = std::get<2>(intersectInfo);
                }
            }
            else if ((*objectIter)->getObjType() == OBJECT_TYPE::TEXTURE_SPHERE)
            {
                IntersectionInfo intersectionInfo = refractionRay.intersectWithSphere((*objectIter)->position, (*objectIter)->scale);
                if (std::get<0>(intersectionInfo) && std::get<1>(intersectionInfo) < temp_t + glm::epsilon<GLdouble>() && std::get<1>(intersectionInfo) > 0)
                {
                    temp_t = std::get<1>(intersectionInfo);
                    temp_obj = (*objectIter);
                    cubeNormal = glm::normalize(refractionRay.getRayOrigin()+temp_t*refractionRay.getRayDirection()-(*objectIter)->position);
                }
                
            }
            else
            {
                continue;
            }
        }
        transcluentColor = Color(0,0,0);
        if(temp_obj != nullptr)
        {
            glm::dvec3 refractionIntersectPoint = refractionRay.getRayOrigin() + temp_t * refractionRay.getRayDirection();
            // if(temp_obj->getObjType() == OBJECT_TYPE::TEXTURE_SPHERE || temp_obj->getObjType() == OBJECT_TYPE::TEXTURE_BOX)
            // {
            //     transcluentColor = Color(0,0,0);
            //     reflectionColor = Color(0,0,0);
            // }
            // else
            // {
            //     transcluentColor = refractionHandling(refractionRay, cubeNormal, refractionIntersectPoint, depth+1, scene, temp_obj->mat, temp_obj) + reflectHandling(refractionRay, cubeNormal, refractionIntersectPoint, depth+1, scene, temp_obj->mat, temp_obj);
            //     reflectionColor = reflectHandling(refractionRay, cubeNormal, refractionIntersectPoint, depth+1, scene, temp_obj->mat, temp_obj);
            // }
            transcluentColor = refractionHandling(refractionRay, cubeNormal, refractionIntersectPoint, depth+1, scene, temp_obj->mat, temp_obj) + reflectHandling(refractionRay, cubeNormal, refractionIntersectPoint, depth+1, scene, temp_obj->mat, temp_obj);
            reflectionColor = reflectHandling(refractionRay, cubeNormal, refractionIntersectPoint, depth+1, scene, temp_obj->mat, temp_obj);
            lightSourceColor = localColor(refractionRay, cubeNormal, refractionIntersectPoint, scene, temp_obj->mat, temp_obj);
        }

        return Color((lightSourceColor.r+transcluentColor.r+reflectionColor.r), (lightSourceColor.g+transcluentColor.g+reflectionColor.g), (lightSourceColor.b+transcluentColor.b+reflectionColor.b));
    }
    else if (depth >= 3)
    {
        return localColor(inputRay, normal, intersectPoint, scene, objMat, obj);
    }
    else
    {
        return Color(0,0,0);
    }

}

void RayTracing::rayTrace(const Scene& scene)
{
    if (pixelWorldCoords == nullptr || primaryRays == nullptr)
    {
        return;
    }

    GLboolean isIntersectWithObj;
    const std::vector<Light *>& lightSources = scene.getLightSources();
    const std::vector<Object *>& sceneObjects = scene.getSceneObjects();
    
    //scene.lightOff();

    size_t shadowCount = 0;
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            GLint pixelLocation = (height-y-1) * width * 3 + x * 3;
            pixelColorBuffer[pixelLocation + 0] = 0.0f;
            pixelColorBuffer[pixelLocation + 1] = 0.0f;
            pixelColorBuffer[pixelLocation + 2] = 0.0f;

            isIntersectWithObj = false;
            GLdouble temp_t = primaryRays[y][x].getMaximumStep();
            Object * temp_obj = nullptr;

            // Finding Intersect Object with Primary Ray
            glm::dvec3 cubeNormal = glm::dvec3();
            
            for (std::vector<Object *>::const_iterator objectIter = sceneObjects.begin(); objectIter != sceneObjects.end(); objectIter++)
            {
                if ((*objectIter)->getObjType() == OBJECT_TYPE::POLYGON3D_OBJECT)
                {
                    Polygon3D * temp = dynamic_cast<Polygon3D *>(*objectIter);
                    if (temp->getPolygonType() == POLYGON_TYPE::SPHERE)
                    {
                        IntersectionInfo intersectInfo = primaryRays[y][x].intersectWithSphere(temp->position, temp->o_radius);
                        if (std::get<0>(intersectInfo) && std::get<1>(intersectInfo) < temp_t + glm::epsilon<GLdouble>() && std::get<1>(intersectInfo) > 0)
                        {
                            temp_t = std::get<1>(intersectInfo);
                            temp_obj = temp;
                            cubeNormal = glm::normalize(primaryRays[y][x].getRayOrigin()+temp_t*primaryRays[y][x].getRayDirection()-(temp)->position);
                        }
                    }
                    else if (temp->getPolygonType() == POLYGON_TYPE::CUBE)
                    {
                        IntersectionPolygonInfo intersectInfo = primaryRays[y][x].intersectWithPolygon(*temp);
                        if(std::get<0>(intersectInfo) && std::get<1>(intersectInfo) < temp_t + glm::epsilon<GLdouble>() && std::get<1>(intersectInfo) > 0)
                        {
                            temp_t = std::get<1>(intersectInfo);
                            temp_obj = temp;
                            cubeNormal = std::get<2>(intersectInfo);
                        }
                    }
                    else
                    {
                        continue;
                    }
                }
                else if ((*objectIter)->getObjType() == OBJECT_TYPE::TEXTURE_BOX)
                {
                    Box * temp = dynamic_cast<Box *>(*objectIter);
                    IntersectionPolygonInfo intersectInfo = primaryRays[y][x].intersectWithTextureBox(*temp);
                    if(std::get<0>(intersectInfo) && std::get<1>(intersectInfo) < temp_t + glm::epsilon<GLdouble>() && std::get<1>(intersectInfo) > 0)
                    {
                        temp_t = std::get<1>(intersectInfo);
                        temp_obj = temp;
                        cubeNormal = std::get<2>(intersectInfo);
                    }
                }
                else if ((*objectIter)->getObjType() == OBJECT_TYPE::TEXTURE_SPHERE)
                {
                    IntersectionInfo intersectionInfo = primaryRays[y][x].intersectWithSphere((*objectIter)->position, (*objectIter)->scale);
                    if (std::get<0>(intersectionInfo) && std::get<1>(intersectionInfo) < temp_t + glm::epsilon<GLdouble>() && std::get<1>(intersectionInfo) > 0)
                    {
                        temp_t = std::get<1>(intersectionInfo);
                        temp_obj = (*objectIter);
                        cubeNormal = glm::normalize(primaryRays[y][x].getRayOrigin()+temp_t*primaryRays[y][x].getRayDirection()-(*objectIter)->position);
                    }
                    
                }
                
                else
                {
                    continue;
                }
            }

            // Object intersecting with Primary Ray exist
            Color reflectColor = Color();
            Color refractColor = Color();
            Color lightSourceColor = Color();
            if(temp_obj != nullptr)
            {
                glm::dvec3 intersectPoint = primaryRays[y][x].getRayOrigin() + primaryRays[y][x].getRayDirection()*temp_t;
                reflectColor = reflectHandling(primaryRays[y][x], cubeNormal, intersectPoint, 0, scene, temp_obj->mat, temp_obj);
                refractColor = refractionHandling(primaryRays[y][x], cubeNormal, intersectPoint, 0, scene, temp_obj->mat, temp_obj);
                lightSourceColor = localColor(primaryRays[y][x], cubeNormal, intersectPoint, scene, temp_obj->mat, temp_obj);
                Color totalColor = Color(lightSourceColor.r + reflectColor.r + refractColor.r, lightSourceColor.g + reflectColor.g + refractColor.g, lightSourceColor.b + reflectColor.b + refractColor.b);
                
                pixelColorBuffer[pixelLocation+0] += totalColor.r;
                pixelColorBuffer[pixelLocation+1] += totalColor.g;
                pixelColorBuffer[pixelLocation+2] += totalColor.b;
            }

            if(pixelColorBuffer[pixelLocation + 0] > 1.0f)
            {
                pixelColorBuffer[pixelLocation + 0] = 1.0f;
            }
            else if(pixelColorBuffer[pixelLocation + 0] < 0.0f)
            {
                pixelColorBuffer[pixelLocation + 0] = 0.0f;
            }

            if(pixelColorBuffer[pixelLocation + 1] > 1.0f)
            {
                pixelColorBuffer[pixelLocation + 1] = 1.0f;
            }
            else if(pixelColorBuffer[pixelLocation + 1] < 0.0f)
            {
                pixelColorBuffer[pixelLocation + 1] = 0.0f;
            }

            if(pixelColorBuffer[pixelLocation + 2] > 1.0f)
            {
                pixelColorBuffer[pixelLocation + 2] = 1.0f;
            }
            else if(pixelColorBuffer[pixelLocation + 2] < 0.0f)
            {
                pixelColorBuffer[pixelLocation + 2] = 0.0f;
            }
        }
    }

    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            pixelColorByteBuffer[3*y*width + 3*x + 0] = (GLubyte)(pixelColorBuffer[3*y*width + 3*x + 2] * 255);
            pixelColorByteBuffer[3*y*width + 3*x + 1] = (GLubyte)(pixelColorBuffer[3*y*width + 3*x + 1] * 255);
            pixelColorByteBuffer[3*y*width + 3*x + 2] = (GLubyte)(pixelColorBuffer[3*y*width + 3*x + 0] * 255);
        }
    }
}

void RayTracing::draw(void) const
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity();
        
        gluOrtho2D(0, width, 0, height);
        
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
            glLoadIdentity();
            
            glRasterPos2f(0, 0);
            // glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixelColorBuffer);
            glDrawPixels(width, height, GL_BGR, GL_UNSIGNED_BYTE, pixelColorByteBuffer);
            glPopMatrix();

        glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

RayTracing::~RayTracing()
{
    if (pixelWorldCoords != nullptr)
    {
        for (size_t i = 0; i < height; i++)
        {
            delete [] pixelWorldCoords[i];
        }
        delete [] pixelWorldCoords;
    }

    if (pixelColorBuffer != nullptr) delete [] pixelColorBuffer;
    if (pixelColorByteBuffer != nullptr) delete [] pixelColorByteBuffer;

    if (primaryRays != nullptr)
    {
        for (size_t i = 0; i < height; i++)
        {
            delete [] primaryRays[i];
        }
        delete [] primaryRays;
    }
    
}
