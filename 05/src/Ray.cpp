#include "Ray.h"

const glm::dvec3& Ray::getRayDirection(void) const
{
    return dir;
}

const glm::dvec3& Ray::getRayOrigin(void) const
{
    return startPoint;
}

const GLdouble& Ray::getMaximumStep(void) const
{
    return maximum_step;
}

void Ray::changeMaximumStep(const GLdouble& newMax)
{
    maximum_step = newMax;
}

Ray::Ray(const Ray& r1)
{
    dir = r1.getRayDirection();
    startPoint = r1.getRayOrigin();
    maximum_step = r1.getMaximumStep();
}

Ray::Ray(const glm::dvec3& rayDir, const glm::dvec3& rayOrigin)
{
    dir = rayDir;
    startPoint = rayOrigin;
    maximum_step = STD_MAXIMUM_STEP;
}

void Ray::changeStartPoint(const glm::dvec3& newStart)
{
    startPoint = newStart;
}

void Ray::changeDir(const glm::dvec3& newDir)
{
    dir = newDir;
}

void Ray::changeDirbyNewEndPoint(const glm::dvec3& newEnd)
{
    dir = glm::normalize(newEnd - startPoint);
}

IntersectionInfo Ray::intersectWithSphere(const glm::dvec3& center, const GLdouble& radius) const
{
    glm::dvec3 deltaP = center - startPoint;
    GLdouble b = glm::dot(dir, deltaP);
    GLdouble ac = glm::dot(deltaP, deltaP) - (radius) * (radius);
    GLdouble deterMineValue = b*b - ac;
    
    if (deterMineValue > 0)
    {
        GLdouble t1 = b + sqrtf(deterMineValue);
        GLdouble t2 = b - sqrtf(deterMineValue);

        if (t1 <= glm::epsilon<GLdouble>() && t2 <= glm::epsilon<GLdouble>())
        {
            return IntersectionInfo(false, maximum_step);
        }
        else if(t1 * t2 <= glm::epsilon<GLdouble>())
        {
            return IntersectionInfo(true, t1 > glm::epsilon<GLdouble>() ? t1 : t2);
        }
        else
        {
            return IntersectionInfo(true, t1 < t2 ? t1 : t2);
        }
    }
    else
    {
        return IntersectionInfo(false, maximum_step);
    }
}

IntersectionPolygonInfo Ray::intersectWithTextureBox(const Box& box) const
{
    GLdouble tmin, tmax, txmin, txmax, tymin, tymax, tzmin, tzmax;
    glm::dvec3 min = box.position - glm::dvec3(box.scale, box.scale, box.scale)*0.5 - glm::epsilon<GLdouble>();
    glm::dvec3 max = box.position + glm::dvec3(box.scale, box.scale, box.scale)*0.5 + glm::epsilon<GLdouble>();
    glm::dvec3 normal = glm::dvec3(0,0,0);
    if (dir.x >= glm::epsilon<GLdouble>()) {
        txmin = (min.x - startPoint.x) / dir.x;
        txmax = (max.x - startPoint.x) / dir.x;
    }
    else {
        txmin = (max.x - startPoint.x) / dir.x;
        txmax = (min.x - startPoint.x) / dir.x;
    }
    if (dir.y >= glm::epsilon<GLdouble>()) {
        tymin = (min.y - startPoint.y) / dir.y;
        tymax = (max.y - startPoint.y) / dir.y;
    } else {
        tymin = (max.y - startPoint.y) / dir.y;
        tymax = (min.y - startPoint.y) / dir.y;
    }
    if ((txmin > tymax) || (tymin > txmax)) {
        return IntersectionPolygonInfo(false, maximum_step, glm::dvec3());
    }
    if (tymin > txmin) {
        tmin = tymin;
    }
    else tmin = txmin;
    if (tymax < txmax) {
        tmax = tymax;
    }
    else tmax = txmax;
    if (dir.z >= glm::epsilon<GLdouble>()) {
        tzmin = (min.z - startPoint.z) / dir.z;
        tzmax = (max.z - startPoint.z) / dir.z;
    } else {
        tzmin = (max.z - startPoint.z) / dir.z;
        tzmax = (min.z - startPoint.z) / dir.z;
    }
    if ((tmin > tzmax) || (tzmin > tmax)) {
        return IntersectionPolygonInfo(false, maximum_step, glm::dvec3());
    }
    if (tzmin > tmin) {
        tmin = tzmin;
    }
    if (tzmax < tmax) {
        tmax = tzmax;
    }

    if(tmin == txmin)
    {
        if (dir.x >= glm::epsilon<GLdouble>()) normal.x = -1;
        else normal.x = 1;
    }
    else if(tmin == tymin)
    {
        if (dir.y >= glm::epsilon<GLdouble>()) normal.y = -1;
        else normal.y = 1;
    }
    else
    {
        if (dir.z >= glm::epsilon<GLdouble>()) normal.z = -1;
        else normal.z = 1;
    }

    if(tmin < 0.0) return IntersectionPolygonInfo(false, maximum_step, glm::dvec3(0,0,0));
    else return IntersectionPolygonInfo(true, tmin, normal);
}

IntersectionPolygonInfo Ray::intersectWithPolygon(const Polygon3D& polygon) const
{
    if (polygon.getPolygonType() == POLYGON_TYPE::CUBE)
    {
        GLdouble tmin, tmax, txmin, txmax, tymin, tymax, tzmin, tzmax;
        glm::dvec3 min = polygon.position - glm::dvec3(polygon.slices, polygon.stacks, polygon.height)*0.5 - glm::epsilon<GLdouble>();
        glm::dvec3 max = polygon.position + glm::dvec3(polygon.slices, polygon.stacks, polygon.height)*0.5 + glm::epsilon<GLdouble>();
        glm::dvec3 normal = glm::dvec3(0,0,0);
        if (dir.x >= glm::epsilon<GLdouble>()) {
            txmin = (min.x - startPoint.x) / dir.x;
            txmax = (max.x - startPoint.x) / dir.x;
        }
        else {
            txmin = (max.x - startPoint.x) / dir.x;
            txmax = (min.x - startPoint.x) / dir.x;
        }
        if (dir.y >= glm::epsilon<GLdouble>()) {
            tymin = (min.y - startPoint.y) / dir.y;
            tymax = (max.y - startPoint.y) / dir.y;
        } else {
            tymin = (max.y - startPoint.y) / dir.y;
            tymax = (min.y - startPoint.y) / dir.y;
        }
        if ((txmin > tymax) || (tymin > txmax)) {
            return IntersectionPolygonInfo(false, maximum_step, glm::dvec3());
        }
        if (tymin > txmin) {
            tmin = tymin;
        }
        else tmin = txmin;
        if (tymax < txmax) {
            tmax = tymax;
        }
        else tmax = txmax;
        if (dir.z >= glm::epsilon<GLdouble>()) {
            tzmin = (min.z - startPoint.z) / dir.z;
            tzmax = (max.z - startPoint.z) / dir.z;
        } else {
            tzmin = (max.z - startPoint.z) / dir.z;
            tzmax = (min.z - startPoint.z) / dir.z;
        }
        if ((tmin > tzmax) || (tzmin > tmax)) {
            return IntersectionPolygonInfo(false, maximum_step, glm::dvec3());
        }
        if (tzmin > tmin) {
            tmin = tzmin;
        }
        if (tzmax < tmax) {
            tmax = tzmax;
        }

        if(tmin == txmin)
        {
            if (dir.x >= glm::epsilon<GLdouble>()) normal.x = -1;
            else normal.x = 1;
        }
        else if(tmin == tymin)
        {
            if (dir.y >= glm::epsilon<GLdouble>()) normal.y = -1;
            else normal.y = 1;
        }
        else
        {
            if (dir.z >= glm::epsilon<GLdouble>()) normal.z = -1;
            else normal.z = 1;
        }

        if(tmin < 0) return IntersectionPolygonInfo(false, maximum_step, glm::dvec3(0,0,0));
        else return IntersectionPolygonInfo(true, tmin, normal);
        // glm::dvec3 center = polygon.position;
        // GLdouble x_length = polygon.slices;
        // GLdouble y_length = polygon.stacks;
        // GLdouble z_length = polygon.height;

        // GLdouble s[6];
        // glm::dvec3 vertices[8];
        
        // vertices[0] = center + glm::dvec3(x_length, y_length, z_length) * 0.5;
        // vertices[1] = center + glm::dvec3(-x_length, y_length, z_length) * 0.5;
        // vertices[2] = center + glm::dvec3(-x_length, y_length, -z_length) * 0.5;
        // vertices[3] = center + glm::dvec3(x_length, y_length, -z_length) * 0.5;
        // vertices[4] = center + glm::dvec3(x_length, -y_length, z_length) * 0.5;
        // vertices[5] = center + glm::dvec3(-x_length, -y_length, z_length) * 0.5;
        // vertices[6] = center + glm::dvec3(-x_length, -y_length, -z_length) * 0.5;
        // vertices[7] = center + glm::dvec3(x_length, -y_length, -z_length) * 0.5;

        // glm::dvec3 n[6];
        // size_t faces[6][4];
        // // +y
        // faces[0][0] = 0; faces[0][1] = 3; faces[0][2] = 2; faces[0][3] = 1; 
        // // -y
        // faces[1][0] = 4; faces[1][1] = 5; faces[1][2] = 6; faces[1][3] = 7;
        // // +x
        // faces[2][0] = 0; faces[2][1] = 4; faces[2][2] = 7; faces[2][3] = 3;
        // // -x
        // faces[3][0] = 1; faces[3][1] = 2; faces[3][2] = 6; faces[3][3] = 5;
        // // +z
        // faces[4][0] = 0; faces[4][1] = 1; faces[4][2] = 5; faces[4][3] = 4;
        // // -z
        // faces[5][0] = 3; faces[5][1] = 7; faces[5][2] = 6; faces[5][3] = 3;

        // n[0] = glm::dvec3(0, 1, 0);
        // n[1] = glm::dvec3(0, -1, 0);
        // n[2] = glm::dvec3(1, 0, 0);
        // n[3] = glm::dvec3(-1, 0, 0);
        // n[4] = glm::dvec3(0, 0, 1);
        // n[5] = glm::dvec3(0, 0, -1);

        // s[0] = -(-center.y - y_length*0.5 + glm::dot(n[0], startPoint)) / glm::dot(n[0], dir);
        // s[1] = -(-center.y + y_length*0.5 + glm::dot(n[1], startPoint)) / glm::dot(n[1], dir);
        // s[2] = -(-center.x - x_length*0.5 + glm::dot(n[2], startPoint)) / glm::dot(n[2], dir);
        // s[3] = -(-center.x + x_length*0.5 + glm::dot(n[3], startPoint)) / glm::dot(n[3], dir);
        // s[4] = -(-center.z - z_length*0.5 + glm::dot(n[4], startPoint)) / glm::dot(n[4], dir);
        // s[5] = -(-center.z + z_length*0.5 + glm::dot(n[5], startPoint)) / glm::dot(n[5], dir);

        // for (size_t i = 0; i < 6; i++)
        // {
        //     if (glm::abs(s[i]-tmin) < 1e-3)
        //     {
        //         return IntersectionPolygonInfo(true, tmin, n[i]);
        //     }
            
        // }
        

        // GLdouble s_value = maximum_step;
        // size_t idx;

        // for (size_t i = 0; i < 6; i++)
        // {
        //     if(s[i] < s_value && s[i] > 0)
        //     {
        //         GLboolean insideCheck1 = true;
        //         GLboolean insideCheck2 = true;
        //         for (size_t v = 0; v < 3; v++)
        //         {
        //             /* code */
        //             glm::dvec3 b_a = vertices[(faces[i][v == 2 ? 0 : v+1])]-vertices[faces[i][v]];
        //             glm::dvec3 x_a = startPoint + dir*s[i]-vertices[faces[i][v]];
        //             if (glm::dot(glm::cross(b_a, x_a), n[i]) < 0)
        //             {
        //                 insideCheck1 = false;
        //                 break;
        //             } 
        //         }
        //         for (size_t v = 2; v < 5; v++)
        //         {
        //             /* code */
        //             glm::dvec3 b_a = glm::dvec3();
        //             glm::dvec3 x_a = glm::dvec3();
        //             if(v == 4) 
        //             {
        //                 b_a = vertices[(faces[i][2])]-vertices[faces[i][0]];
        //                 x_a = startPoint + dir*s[i]-vertices[faces[i][0]];
        //             }
        //             else if(v == 3) 
        //             {
        //                 b_a = vertices[(faces[i][0])]-vertices[faces[i][v]];
        //                 x_a = startPoint + dir*s[i]-vertices[faces[i][v]];
        //             }
        //             else 
        //             {
        //                 b_a = vertices[(faces[i][v+1])]-vertices[faces[i][v]];
        //                 x_a = startPoint + dir*s[i]-vertices[faces[i][v]];
        //             }
                    
        //             if (glm::dot(glm::cross(b_a, x_a), n[i]) > 0)
        //             {
        //                 insideCheck2 = false;
        //                 break;
        //             } 
        //         }
        //         if(insideCheck1 || insideCheck2)
        //         {
        //             s_value = s[i]; 
        //             idx = i;
        //         }
        //     }
        // }

        // if (s_value < maximum_step)
        // {
        //     return IntersectionPolygonInfo(true, s_value, n[idx]);
        // }
        // else
        // {
        //     return IntersectionPolygonInfo(false, maximum_step, glm::dvec3());
        // }
    }
    else
    {
        return IntersectionPolygonInfo(false, maximum_step, glm::dvec3());
    }
}

Ray::Ray()
{
    startPoint = glm::dvec3();
    dir = glm::dvec3();
    maximum_step = STD_MAXIMUM_STEP;
}

Ray::~Ray()
{
}