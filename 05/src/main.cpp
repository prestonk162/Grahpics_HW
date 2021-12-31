#include <iostream>
#include <math.h>
#include <string>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Model.h"
#include "Material.h"
#include "Light.h"
#include "OffModel.h"
#include "Polygon3D.h"
#include "Scene.h"
#include "Object.h"
#include "ScreenCapture.h"
#include "RayTracing.h"
#include "Box.h"
#include "ConcreteSphere.h"

#define ROTATE_ANGLE 0.5f*3.141592f/180.0f
#define BUF_SIZE 512
#define TRUMBONE_MODEL "model/trombone.txt"
#define COKE_BOTTLE_MODEL "model/coke_bottle.txt"
#define CUP_MODEL "model/cup.txt"
#define HDODEC_OFF_MODEL "model/hdodec.off"
#define MUSHROOM_OFF_MODEL "model/mushroom.off"
#define SOCKET_OFF_MODEL "model/socket.off"
#define MCTET_OFF_MODEL "model/mctet.off"
#define BOX_TEXTURE_FILE "texture/box.jpg"
#define SPHERE_TEXTURE_FILE "texture/Concrete.jpg"

const static std::string modeUI[2] = {std::string("Operation Mode: Normal"), std::string("Operation Mode: Seeking")};
const static std::string screenUI[2] = {std::string("Screen Mode: Full Screen On"), std::string("Screen Mode: Full Screen Off")};
static GLint width = 1000;
static GLint height = 1000;

static GLboolean mouseRotatePressed = false;
static GLboolean mouseMovePressed   = false;
static GLboolean mouseZoomPressed   = false;
static GLboolean mouseScrollUp = false;
static GLboolean seekingMode = false;

static glm::vec2 lastWindowXY = glm::vec2(0, 0);
static glm::vec2 curWindowXY = glm::vec2(0, 0);
static GLfloat theta=0.0;
static GLfloat fovAngle = 90.0;

static GLboolean fullScreen = false;

static GLboolean dollyCameraIn = false;

static GLboolean rayTracingOn = false;

const static GLfloat dNear = 0.1f;
const static GLfloat dFar = 1000.0f;

static GLdouble * objX;
static GLdouble * objY;
static GLdouble * objZ;


static glm::vec3 lookAtPoint = glm::vec3();
static glm::vec3 cameraOriginPoint = glm::vec3(19.0, 0.0, 0.0);

static glm::vec3 cameraAxisZUnit = glm::normalize(cameraOriginPoint - lookAtPoint);
static glm::vec3 viewUpVector = glm::vec3(0.0, 1.0, 0.0);
static glm::vec3 cameraAxisXUnit = glm::normalize(cross(viewUpVector, cameraAxisZUnit));
static glm::vec3 cameraAxisYUnit = glm::normalize(cross(cameraAxisZUnit, cameraAxisXUnit));

static Scene* scene;
// static OffModel* tempmodel;
// static Light* light;
static RayTracing * rayTracer;

GLenum errorCheck(void)
{
    GLenum code;
    const GLubyte *string;

    code = glGetError();
    if (code != GL_NO_ERROR) {
        string = gluErrorString(code);
        fprintf(stderr, "OpenGL Error: %s\n", string);
    }
    else {
        printf("No Error\n");
    }

    return code;
}

void updateCameraAxis(void)
{
    cameraAxisZUnit = glm::normalize(cameraOriginPoint - lookAtPoint);
    cameraAxisXUnit = glm::normalize(cross(viewUpVector, cameraAxisZUnit));
    cameraAxisYUnit = glm::normalize(cross(cameraAxisZUnit, cameraAxisXUnit));
}

GLfloat depthZTransform(const GLfloat& zWindowValue)
{
    GLfloat result_inv = (1 - zWindowValue) / dNear + zWindowValue / dFar;
    return (GLfloat) 1 / result_inv;
}

glm::vec2 projectionWin(const GLint& x, const GLint& y)
{
    return glm::vec2(2.0f*x/width - 1.0f, 1.0f - 2.0f*y/height);
}

void seekNearestHitPoint(const GLint& xWindowValue, const GLint& yWindowValue, const GLfloat& zWindowValue)
{
    GLdouble modelMatrix[16];
    GLdouble projMatrix[16];
    GLint viewport [4];

    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
    glGetIntegerv(GL_VIEWPORT, viewport);

    gluUnProject((GLdouble)xWindowValue, (GLdouble) viewport[3] - yWindowValue, (GLdouble)zWindowValue, modelMatrix, projMatrix, viewport, objX, objY, objZ);

    glm::vec3 lookCameraVec = cameraOriginPoint - lookAtPoint;

    lookAtPoint = glm::vec3((GLfloat)(*objX), (GLfloat)(*objY), (GLfloat)(*objZ));
    cameraOriginPoint = lookAtPoint + lookCameraVec;

    updateCameraAxis();
}

glm::vec3 transformTrackballVector(const GLint& x, const GLint& y)
{
    glm::vec3 result = glm::vec3(projectionWin(x, y), 0);
    GLfloat xyLengthSquare = result.x * result.x + result.y * result.y;
    
    if(xyLengthSquare <= 1*1)
    {
        result.z = sqrtf(1*1 - xyLengthSquare);
    }
    else
    {
        result = glm::normalize(result);
    }
    return result;
}

void cameraDolly(void)
{
    dollyCameraIn ? cameraOriginPoint += (-1.0f*cameraAxisZUnit) : cameraOriginPoint += (1.0f*cameraAxisZUnit);
    updateCameraAxis();
}

void rotateRandomAxis(void)
{

    glm::vec3 afterDirection = transformTrackballVector(curWindowXY.x, curWindowXY.y);
    glm::vec3 beforeDirection = transformTrackballVector(lastWindowXY.x, lastWindowXY.y);

    glm::vec3 afterDirectionWorld = afterDirection.x * cameraAxisXUnit + afterDirection.y * cameraAxisYUnit + afterDirection.z * cameraAxisZUnit;
    glm::vec3 beforeDirectionWorld = beforeDirection.x * cameraAxisXUnit + beforeDirection.y * cameraAxisYUnit + beforeDirection.z * cameraAxisZUnit;

    afterDirectionWorld = glm::normalize(afterDirectionWorld);
    beforeDirectionWorld = glm::normalize(beforeDirectionWorld);

    glm::vec3 rotateAxis = cross(beforeDirectionWorld, afterDirectionWorld);

    GLfloat cos_theta = dot(beforeDirectionWorld, afterDirectionWorld);
    GLfloat sin_theta = rotateAxis.length();   

    theta = atan2(sin_theta, cos_theta);

    rotateAxis = glm::normalize(rotateAxis);
    
    glm::quat p = glm::quat(0.0f, cameraOriginPoint - lookAtPoint);
    glm::quat v = glm::quat(0.0f, viewUpVector);

    theta *= 0.05f;

    glm::quat q = glm::quat(cos(theta/2), (GLfloat)sin(theta/2)*rotateAxis);
    glm::quat q_inv = glm::quat(cos(theta/2), -(GLfloat)sin(theta/2)*rotateAxis);

    glm::quat p_after = q_inv * p * q;
    glm::quat v_after = q_inv * v * q;

    cameraOriginPoint = lookAtPoint + glm::vec3(p_after.x, p_after.y, p_after.z);
    viewUpVector = glm::normalize(glm::vec3(v_after.x, v_after.y, v_after.z));
    updateCameraAxis();
}

void cameraTranslate(void)
{
    glm::vec2 distance = curWindowXY - lastWindowXY;

    distance = 0.01f * distance;

    distance.y *= -1;

    cameraOriginPoint += (-distance.x * cameraAxisXUnit - distance.y * cameraAxisYUnit);
    lookAtPoint += (-distance.x * cameraAxisXUnit - distance.y * cameraAxisYUnit);

    updateCameraAxis();
}

void uiDraw(void)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity();
        
        gluOrtho2D(0, width, 0, height);
        
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
            glLoadIdentity();
            glColor3f(1.0f, 0.0f, 0.0f);
            
            glRasterPos2f(width*0.01, height*0.96);
            if(seekingMode)
            {
                for (size_t i = 0; i < modeUI[1].size(); i++)
                {
                    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, modeUI[1][i]);
                }
            }
            else
            {
                for (size_t i = 0; i < modeUI[0].size(); i++)
                {
                    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, modeUI[0][i]);
                }
            }

            glRasterPos2f(width*0.01, height*0.96 - 24);
            if(fullScreen)
            {
                for (size_t i = 0; i < screenUI[0].size(); i++)
                {
                    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, screenUI[0][i]);
                }
            }
            else
            {
                for (size_t i = 0; i < screenUI[1].size(); i++)
                {
                    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, screenUI[1][i]);
                }
            }

            glPopMatrix();

        glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}


void drawSomething(void)
{
// Draw something Here
    uiDraw();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    scene -> draw();
        
    glPopMatrix();
}

void drawRayTracedScene(void)
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glLoadIdentity();
    rayTracer -> draw();
    glPopMatrix();
}

void reshape(GLint w, GLint h)
{
    width = glutGet(GLUT_WINDOW_WIDTH);
    height = glutGet(GLUT_WINDOW_HEIGHT);

    glViewport(0, 0, w, h );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	GLfloat aspectRatio = (GLfloat)w/(GLfloat)h;
    
    gluPerspective( fovAngle /*field of view angle*/,
					aspectRatio,
					dNear /*near clipping plane*/,
					dFar /* far clipping plane */ );
	
    gluLookAt(  cameraOriginPoint.x, 
                cameraOriginPoint.y, 
                cameraOriginPoint.z, 
                lookAtPoint.x, 
                lookAtPoint.y, 
                lookAtPoint.z, 
                viewUpVector.x, 
                viewUpVector.y, 
                viewUpVector.z);
	glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

void display()
{
    if(!rayTracingOn)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawSomething();
        // errorCheck();
        glFlush();
        glutSwapBuffers();
    }
    else
    {
        drawRayTracedScene();
        // errorCheck();
        glutSwapBuffers();
    }
}

void adjustToFitEntireModel(void)
{
    
    GLint hits;
    GLuint selectBuffer[BUF_SIZE];
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    GLdouble pickMatrixDimension[2][2] = {{(GLdouble)width, 5.0}, {5.0, (GLdouble)height}};
    GLdouble pickMatrixCenter[4][2] = {{(GLdouble)width/2.0, 2.5}, {2.5, (GLdouble)height/2.0}, {(GLdouble)width/2.0, (GLdouble)height - 2.5}, {(GLdouble)width - 2.5, (GLdouble)height/2.0}};

    GLboolean hitresult [4] = {false, false, false, false};
    do
    {   
        for (size_t i = 0; i < 4; i++)
        {
            glPushMatrix();
            glSelectBuffer(BUF_SIZE, selectBuffer);
            glRenderMode(GL_SELECT);

            glInitNames();
            glPushName(0);

            glMatrixMode(GL_PROJECTION);
            glPushMatrix();

            glLoadIdentity();
            gluPickMatrix(pickMatrixCenter[i][0], pickMatrixCenter[i][1], pickMatrixDimension[i%2][0], pickMatrixDimension[i%2][1], viewport);

            gluPerspective( fovAngle /*field of view angle*/,
                        (GLfloat)width/(GLfloat)height,
                        dNear /*near clipping plane*/,
                        dFar /* far clipping plane */ );

            gluLookAt(  cameraOriginPoint.x, 
                        cameraOriginPoint.y, 
                        cameraOriginPoint.z, 
                        lookAtPoint.x, 
                        lookAtPoint.y, 
                        lookAtPoint.z, 
                        viewUpVector.x, 
                        viewUpVector.y, 
                        viewUpVector.z);

            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();
            
            glLoadName(0);
            
            glPushName(1);
            scene -> draw();
            // offModel -> draw();
            // glColor3f(YELLOW_RGB);
            // glutSolidCube(1);
            // glutSolidCone(0.7, 1.2, 10, 1);
            glPopName();
            
            glPopMatrix();

            glMatrixMode(GL_PROJECTION);

            glPopMatrix();
            // glMatrixMode(GL_MODELVIEW);
            glPopMatrix();
            glFlush();

            hits = glRenderMode(GL_RENDER);

            if (hits > 0)
            {
                hitresult[i] = true;
            }
            else
            {
                hitresult[i] = false;
            }

            if (hitresult[0] || hitresult[1] || hitresult[2] || hitresult[3])
            {
                cameraOriginPoint += cameraAxisZUnit;
                reshape(width, height);
            }   
        }

    } while (hitresult[0] || hitresult[1] || hitresult[2] || hitresult[3]);
    
    glutPostRedisplay();
}

void seekModel(GLint x, GLint y)
{
    glPushMatrix();

    GLuint selectBuffer[BUF_SIZE];
    GLint hits;
    GLint viewport[4];

    glGetIntegerv(GL_VIEWPORT, viewport);
    glSelectBuffer(BUF_SIZE, selectBuffer);

    glRenderMode(GL_SELECT);

    glInitNames();
    glPushName(0);

    glMatrixMode(GL_PROJECTION);
    
    glPushMatrix();
    
    glLoadIdentity();
    gluPickMatrix(x, viewport[3] - y, 5.0, 5.0, viewport);

    gluPerspective( fovAngle /*field of view angle*/,
					(GLfloat)width/(GLfloat)height,
					dNear /*near clipping plane*/,
					dFar /* far clipping plane */ );

    gluLookAt(  cameraOriginPoint.x, 
                cameraOriginPoint.y, 
                cameraOriginPoint.z, 
                lookAtPoint.x, 
                lookAtPoint.y, 
                lookAtPoint.z, 
                viewUpVector.x, 
                viewUpVector.y, 
                viewUpVector.z);
	
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glLoadName(0);
    
    glPushName(1);
    scene -> draw();
    // offModel -> draw();
    // glColor3f(YELLOW_RGB);
    // glutSolidCube(1);
    // glutSolidCone(0.7, 1.2, 10, 1);
    glPopName();
    
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);

    glPopMatrix();
    // glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glFlush();

    hits = glRenderMode(GL_RENDER);
        
    if(hits > 0)
    {
        GLuint * ptr = (GLuint *)selectBuffer;
        ptr++;
        seekNearestHitPoint(x, y, (GLfloat) *ptr/0xffffffff );
        reshape(width, height);
        seekingMode = false;
    }
    
    glutPostRedisplay();
}

void findAll(void)
{
    glm::vec3 lookCameraVec = cameraOriginPoint - lookAtPoint;
    lookAtPoint = glm::vec3();
    cameraOriginPoint = lookAtPoint + lookCameraVec;
    updateCameraAxis();

    adjustToFitEntireModel();
    updateCameraAxis();
}


void keyboardSpecialCB(GLint key, GLint x, GLint y)
{
    switch(key) 
    {
        case GLUT_KEY_UP:   // dolly in
            dollyCameraIn = true;
            if(!seekingMode && !rayTracingOn)
            { 
                cameraDolly();
                reshape(width, height);
            }
            break;
        case GLUT_KEY_DOWN: // dolly out
            dollyCameraIn = false;
            if (!seekingMode && !rayTracingOn)
            {                
                cameraDolly();
                reshape(width, height);
            }
            break;
        case GLUT_KEY_END: // Raytracing
            rayTracingOn = !rayTracingOn;
            if (rayTracingOn)
            {
                glDisable(GL_DEPTH_TEST);
                captureScreen((size_t)width, (size_t)height);
                rayTracer->cameraPos = cameraOriginPoint;
                rayTracer->calculatePixelCoord(width, height);
                rayTracer->rayTrace(*scene);
                printRayTracedImage(width, height, rayTracer->pixelColorBuffer);
            }
            else glEnable(GL_DEPTH_TEST);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void keyboardCB(unsigned char keyPressed, int x, int y)
{
    if(!rayTracingOn) {
        switch (keyPressed) {
        case 'v':
            if (fullScreen == true) {
                glutReshapeWindow(width,height);
                fullScreen = false;
            } else {
                glutFullScreen();
                fullScreen = true;
            }
            break;
        case 's':   // toggle Seeking Mode
            seekingMode = !seekingMode;
            break;
        case 'f':
            if(!seekingMode)
            {
                findAll();
                reshape(width, height);
            }
            break;
        case 27:
            exit(0);
            break;
        }
        glutPostRedisplay();
    } else {
        return;
    }
}

void mouseWheelCB(GLint button, GLint dir, GLint x, GLint y)
{
    if(!rayTracingOn) {
        if(dir > 0)
        {
            fovAngle <= 0.0 ? fovAngle = 0.0f : fovAngle -= 1.0;
            reshape(width, height);
        }
        else if (dir < 0)
        {
            fovAngle >= 180.0 ? fovAngle = 180.0f : fovAngle += 1.0;
            reshape(width, height);
        }
        glutPostRedisplay();
    } else {
        return;
    }
}

// Left Up: (0,0) Right Down: (Width, height)
void mouseButtonCB(GLint button, GLint state, GLint x, GLint y) {
    if(!rayTracingOn) {
        if (state == GLUT_UP) {
            mouseMovePressed   = false;
            mouseRotatePressed = false;
            mouseZoomPressed   = false;
        } else {
            if (button==GLUT_LEFT_BUTTON && GLUT_ACTIVE_SHIFT==glutGetModifiers())
            {
                // do something here
                lastWindowXY = curWindowXY = glm::vec2(x, y);
                mouseMovePressed   = true;
                mouseRotatePressed = false;
                mouseZoomPressed   = false;
            }
            else if ((button==3 || button ==4) && !seekingMode)
            {
                mouseZoomPressed = true;
                mouseMovePressed   = false;
                mouseRotatePressed = false;
                mouseScrollUp = (button==3);

                if(mouseScrollUp)
                {
                    fovAngle <= 0.0 ? fovAngle = 0.0f : fovAngle -= 1.0;
                }
                else
                {
                    fovAngle >= 180.0 ? fovAngle = 180.0f : fovAngle += 1.0;
                    
                }
                reshape(width, height);
            }
            else if (button==GLUT_LEFT_BUTTON)
            {
                // do something here
                lastWindowXY = curWindowXY = glm::vec2(x, y);

                mouseMovePressed   = false;
                mouseRotatePressed = seekingMode ? false : true;
                mouseZoomPressed   = false;

                if(seekingMode)
                {
                    seekModel(x, y);
                }
            }
        }
        glutPostRedisplay();
    }
    else {
        return;
    }
}

void motionCB(GLint x, GLint y) {
    if(~rayTracingOn) {
        if (mouseRotatePressed && !seekingMode)
        {
            // do something here

            curWindowXY = glm::vec2(x, y);

            glm::vec2 delta = curWindowXY - lastWindowXY;
            if(abs(delta.x) || abs(delta.y))
            {
                rotateRandomAxis();
                lastWindowXY = curWindowXY;
            }
            reshape(width, height);
        }
        else if (mouseMovePressed && !seekingMode)
        {
            // do something here

            curWindowXY = glm::vec2(x, y);
            cameraTranslate();
            lastWindowXY = curWindowXY;
            reshape(width, height);
        }
        glutPostRedisplay();
    } else {
        return;
    }
}

void idle() 
{
    glutPostRedisplay(); 
}

void init(void)
{
    RayTracing * tempRayTracing = new RayTracing(width, height, cameraOriginPoint);

    std::cout << "Camera Pos: (" << cameraOriginPoint.x << ", " << cameraOriginPoint.y << ", " << cameraOriginPoint.z << ")" << std::endl;

    delete tempRayTracing;

    scene = new Scene();

    // Polygon3D* wallBottom = new Polygon3D(0, 0, 1, 1, 1);
    Box* box = new Box();
    box -> scale = 4;
    // Polygon3D* wallBottom = new Polygon3D(0, 0, 2, 2, 2);
    box -> mat.setAmbient(0.05f, 0.05f, 0.05f, 1.0f);
    box -> mat.setDiffuse(0.05f, 0.05f, 0.05f, 1.0f);
    box -> mat.setSpecular(0.0f, 0.0f, 0.0f, 1.0f);
    box -> mat.setEmission();
    box -> mat.setShiness(0.0);
    // wallBottom -> position = glm::vec3(0.0f, 0.0f, 0.0f);
    box -> position = glm::dvec3(0.0, 1.0, 0.0);
    box -> setFilename(BOX_TEXTURE_FILE);
    box -> setTextureID(1);
    scene -> addModel(box);

    ConcreteSphere* concreteSphere = new ConcreteSphere();
    concreteSphere -> mat.setAmbient(0.05f, 0.05f, 0.05f, 1.0f);
    concreteSphere -> mat.setDiffuse(0.05f, 0.05f, 0.05f, 1.0f);
    concreteSphere -> mat.setSpecular(0.0f, 0.0f, 0.0f, 1.0f);
    concreteSphere -> mat.setEmission();
    concreteSphere -> mat.setShiness(0.0);
    concreteSphere -> position = glm::dvec3(0, 0, -8);
    concreteSphere -> setFilename(SPHERE_TEXTURE_FILE);
    concreteSphere -> setTextureID(2);
    scene -> addModel(concreteSphere);

    // OffModel* socketModel = new OffModel();
    // socketModel -> setObjType(OBJECT_TYPE::OFF_IMPORT);
    // // OFF Imported Model: Socket
    // socketModel -> loadModel(SOCKET_OFF_MODEL);
    // // Chrome
    // socketModel -> mat.setAmbient(0.25f, 0.25f, 0.25f, 1.0f);
    // socketModel -> mat.setDiffuse(0.4f, 0.4f, 0.4f, 1.0f);
    // socketModel -> mat.setSpecular(0.774597f, 0.774597f, 0.774597f, 1.0f);
    // socketModel -> mat.setEmission(0.0f, 0.0f, 0.0f, 1.0f);
    // socketModel -> mat.setShiness(76.8f);
    // socketModel -> scale = 0.02f;
    // socketModel -> position = glm::vec3(-8.0f, 1.5f, 8.0f);
    // scene->addModel(socketModel);

    // // Own Swept Surface
    // Model* cupModel = new Model();
    // cupModel -> setObjType(OBJECT_TYPE::SPLINE_SURFACE_IMPORT);
    // cupModel -> setResolution(5);
    // cupModel -> loadData(CUP_MODEL);
    // // Red Plastic
    // cupModel -> mat.setAmbient(0.0f, 0.0f, 0.0f, 1.0f);
    // cupModel -> mat.setDiffuse(0.5f, 0.0f, 0.0f, 1.0f);
    // cupModel -> mat.setSpecular(0.7f, 0.0f, 0.0f, 1.0f);
    // cupModel -> mat.setEmission(0.0f, 0.0f, 0.0f, 1.0f);
    // cupModel -> mat.setShiness(32.0f);
    // cupModel -> position = glm::vec3(0.0f, 1.5f, 0.0f);
    // cupModel -> scale = 0.1f;
    
    // scene -> addModel(cupModel);

    // // // Swept Surface
    // Model* cockBottleModel = new Model();
    // cockBottleModel -> setObjType(OBJECT_TYPE::SPLINE_SURFACE_IMPORT);
    // cockBottleModel -> setResolution(5);
    // cockBottleModel -> loadData(COKE_BOTTLE_MODEL);
    // // Polished Gold
    // cockBottleModel -> mat.setAmbient(0.024752f, 0.02245f, 0.00645f, 1.0f);
    // cockBottleModel -> mat.setDiffuse(0.34615f, 0.3143f, 0.0903f, 1.0f);
    // cockBottleModel -> mat.setSpecular(0.797357f, 0.723991f, 0.208006f, 1.0f);
    // cockBottleModel -> mat.setEmission(0.0f, 0.0f, 0.0f, 1.0f);
    // cockBottleModel -> mat.setShiness(83.2f);
    // cockBottleModel -> scale = 0.1f;
    // cockBottleModel -> position = glm::vec3(8.0f, 2.0f, 8.0f);
    // scene->addModel(cockBottleModel);

    // Implicit Surface: Sphere
    Polygon3D* sphereChrome = new Polygon3D(1, 0, 20, 20, 0);
    // Brass
    sphereChrome -> setType(POLYGON_TYPE::SPHERE);
    sphereChrome -> mat.setAmbient(0.25f, 0.25f, 0.25f, 1.0f);
    sphereChrome -> mat.setDiffuse(0.4f, 0.4f, 0.4f, 1.0f);
    sphereChrome -> mat.setSpecular(0.774597f, 0.774597f, 0.774597f, 1.0f);
    sphereChrome -> mat.setEmission(0.0f, 0.0f, 0.0f, 1.0f);
    sphereChrome -> mat.setShiness(76.8f);
    sphereChrome -> position = glm::dvec3(8.0f, 0.0f, 0.0f);
    sphereChrome -> mat.setReflectioness(0.0f);
    sphereChrome -> mat.setOpaqueness(false);
    sphereChrome -> mat.setIndexOfRefraction(1.5f);
    scene->addModel(sphereChrome);

    // Implicit Surface: Sphere
    Polygon3D* sphereCopper = new Polygon3D(1, 0, 20, 20, 0);
    // Copper
    sphereCopper -> setType(POLYGON_TYPE::SPHERE);
    sphereCopper -> mat.setAmbient(0.19125f, 0.0735f, 0.0225f, 1.0f);
    sphereCopper -> mat.setDiffuse(0.7038f, 0.27048f, 0.0828f, 1.0f);
    sphereCopper -> mat.setSpecular(0.256777f, 0.137622f, 0.086014f, 1.0f);
    sphereCopper -> mat.setEmission(0.0f, 0.0f, 0.0f, 1.0f);
    sphereCopper -> mat.setShiness(12.8f);
    sphereCopper -> position = glm::dvec3(-4.0f, 0.0f, -4.0f);
    // sphereCopper -> mat.setReflectioness(0.7f);
    scene->addModel(sphereCopper);
    
    // // Polygon: Icosahedron
    // Polygon3D* icosahedronEmerald = new Polygon3D();
    // icosahedronEmerald -> setType(POLYGON_TYPE::ICOSAHEDRON);
    // // Emerald
    // icosahedronEmerald -> mat.setAmbient(0.0215f, 0.1745f, 0.0215f, 1.0f);
    // icosahedronEmerald -> mat.setDiffuse(0.07568f, 0.61424f, 0.07568f, 1.0f);
    // icosahedronEmerald -> mat.setSpecular(0.633f, 0.727811f, 0.633f, 1.0f);
    // icosahedronEmerald -> mat.setEmission(0.0f, 0.0f, 0.0f, 1.0f);
    // icosahedronEmerald -> mat.setShiness(76.8f);
    // icosahedronEmerald -> position = glm::vec3(8.0f, 0.0f, 0.0f);
    // scene->addModel(icosahedronEmerald);

    // // Polygon: Teapot 
    // Polygon3D* teapotPlastic = new Polygon3D();
    // teapotPlastic -> setType(POLYGON_TYPE::TEAPOT);
    // // Yello Plastic
    // teapotPlastic -> mat.setAmbient(0.0f, 0.0f, 0.0f, 1.0f);
    // teapotPlastic -> mat.setDiffuse(0.5f, 0.5f, 0.0f, 1.0f);
    // teapotPlastic -> mat.setSpecular(0.6f, 0.6f, 0.5f, 1.0f);
    // teapotPlastic -> mat.setEmission(0.0f, 0.0f, 0.0f, 1.0f);
    // teapotPlastic -> mat.setShiness(32.0f);
    // teapotPlastic -> position = glm::vec3(-3.0f, 0.0f, 4.0f);
    // scene->addModel(teapotPlastic);

    // // Polygon: Cone
    // Polygon3D* coneLamp = new Polygon3D(1, 0, 20, 20, 2);
    // coneLamp -> setType(POLYGON_TYPE::CONE);
    // coneLamp -> mat.setAmbient(0.1f, 0.1f, 0.1f, 1.0f);
    // coneLamp -> mat.setDiffuse(0.3f, 0.3f, 0.3f, 1.0f);
    // coneLamp -> mat.setSpecular(0.2f, 0.2f, 0.2f, 1.0f);
    // coneLamp -> mat.setEmission(0.4f, 0.0f, 0.0f, 1.0f);
    // coneLamp -> mat.setShiness(50.0f);
    // coneLamp -> position = glm::vec3(0.0f, 10.5f, -20.0f);
    // coneLamp -> scale = 2.0f;
    // scene -> addModel(coneLamp);

    // Polygon3D* sun = new Polygon3D(4, 0, 40, 40, 0);
    // sun -> setType(POLYGON_TYPE::SPHERE);
    // sun -> mat.setAmbient(0.0f, 0.0f, 0.0f, 1.0f);
    // sun -> mat.setDiffuse(0.0f, 0.0f, 0.0f, 1.0f);
    // sun -> mat.setSpecular(0.0f, 0.0f, 0.0f, 1.0f);
    // sun -> mat.setEmission(1.0f, 1.0f, 1.0f, 1.0f);
    // sun -> mat.setShiness(128.0f);
    // sun -> position = glm::vec3(25.0f, 75.0f, 25.0f);
    // scene -> addModel(sun);

    Polygon3D* blueSun = new Polygon3D(1, 0, 20, 20, 0);
    blueSun -> setType(POLYGON_TYPE::SPHERE);
    blueSun -> mat.setAmbient(0.0f, 0.0f, 0.0f, 1.0f);
    blueSun -> mat.setDiffuse(0.5f, 0.5f, 0.0f, 1.0f);
    blueSun -> mat.setSpecular(0.6f, 0.6f, 0.5f, 1.0f);
    blueSun -> mat.setEmission(0.0f, 0.0f, 0.0f, 1.0f);
    blueSun -> mat.setShiness(32.0f);
    blueSun -> position = glm::vec3(4.0f, 0.0f, -4.0f);
    // blueSun -> mat.setReflectioness(0.6f);
    scene -> addModel(blueSun);

    // Implicit Surface: Sphere
    Polygon3D* sphereLamp = new Polygon3D(1, 0, 20, 20, 0);
    // Sphere Silver
    sphereLamp -> setType(POLYGON_TYPE::SPHERE);
    sphereLamp -> mat.setAmbient(0.03125f, 0.03125f, 0.03125f, 1.0f);
    sphereLamp -> mat.setDiffuse(0.1775f, 0.1775f, 0.1775f, 1.0f);
    sphereLamp -> mat.setSpecular(0.773911f, 0.773911f, 0.773911f, 1.0f);
    sphereLamp -> mat.setEmission(0.0f, 0.0f, 0.0f, 1.0f);
    sphereLamp -> mat.setShiness(89.6f);
    sphereLamp -> position = glm::dvec3(-4.0, 0.0, 4.0);
    // sphereLamp -> mat.setReflectioness(0.0f);
    scene->addModel(sphereLamp);

    // Polygon3D* rubberDonut = new Polygon3D(4, 1, 20, 20, 0);
    // // Rubber Cyan
    // rubberDonut -> setType(POLYGON_TYPE::TORUS);
    // rubberDonut -> mat.setAmbient(0.0f, 0.05f, 0.05f, 1.0f);
    // rubberDonut -> mat.setDiffuse(0.4f, 0.5f, 0.5f, 1.0f);
    // rubberDonut -> mat.setSpecular(0.04f, 0.7f, 0.7f, 1.0f);
    // rubberDonut -> mat.setEmission(0.0f, 0.0f, 0.0f, 1.0f);
    // rubberDonut -> mat.setShiness(10.0f);
    // rubberDonut -> position = glm::vec3(0.0f, 10.5f, -19.0f);
    // scene->addModel(rubberDonut);

    

    // Polygon3D* wallBottom = new Polygon3D(0, 0, 1, 1, 1);
    Polygon3D* wallBottom = new Polygon3D(0, 0, 40, 1, 40);
    // Polygon3D* wallBottom = new Polygon3D(0, 0, 2, 2, 2);
    wallBottom -> setType(POLYGON_TYPE::CUBE);
    wallBottom -> mat.setAmbient(0.135f, 0.2225f, 0.1575f, 0.95f);
    wallBottom -> mat.setDiffuse(0.54f, 0.89f, 0.63f, 0.95f);
    wallBottom -> mat.setSpecular(0.316228f, 0.316228f, 0.316228f, 0.95f);
    wallBottom -> mat.setEmission();
    wallBottom -> mat.setShiness(12.8);
    // wallBottom -> position = glm::vec3(0.0f, 0.0f, 0.0f);
    wallBottom -> position = glm::vec3(0.0f, -1.5f, 0.0f);
    wallBottom -> mat.setReflectioness(0.1);
    scene -> addModel(wallBottom);

    Polygon3D* wallFront = new Polygon3D(0, 0, 40, 40, 1);
    wallFront -> setType(POLYGON_TYPE::CUBE);
    wallFront -> mat.setAmbient(0.135f, 0.2225f, 0.1575f, 0.95f);
    wallFront -> mat.setDiffuse(0.54f, 0.89f, 0.63f, 0.95f);
    wallFront -> mat.setSpecular(0.316228f, 0.316228f, 0.316228f, 0.95f);
    wallFront -> mat.setEmission();
    wallFront -> mat.setShiness(12.8);
    wallFront -> position = glm::vec3(0.0f, 18.5f, 20.5f);
    wallFront -> mat.setReflectioness(0.1);
    wallFront -> mat.setOpaqueness(false);
    wallFront -> mat.setIndexOfRefraction(1.5);
    scene -> addModel(wallFront);

    Polygon3D* wallLeft = new Polygon3D(0, 0, 1, 40, 40);
    wallLeft -> setType(POLYGON_TYPE::CUBE);
    wallLeft -> mat.setAmbient(0.135f, 0.2225f, 0.1575f, 0.95f);
    wallLeft -> mat.setDiffuse(0.54f, 0.89f, 0.63f, 0.95f);
    wallLeft -> mat.setSpecular(0.316228f, 0.316228f, 0.316228f, 0.95f);
    wallLeft -> mat.setEmission();
    wallLeft -> mat.setShiness(12.8);
    wallLeft -> position = glm::vec3(-20.5f, 18.5f, 0.0f);
    wallLeft -> mat.setReflectioness(0.1);
    wallLeft -> mat.setOpaqueness(false);
    wallLeft -> mat.setIndexOfRefraction(1.5);
    scene -> addModel(wallLeft);

    Polygon3D* wallRight = new Polygon3D(0, 0, 1, 40, 40);
    wallRight -> setType(POLYGON_TYPE::CUBE);
    wallRight -> mat.setAmbient(0.135f, 0.2225f, 0.1575f, 0.95f);
    wallRight -> mat.setDiffuse(0.54f, 0.89f, 0.63f, 0.95f);
    wallRight -> mat.setSpecular(0.316228f, 0.316228f, 0.316228f, 0.95f);
    wallRight -> mat.setEmission();
    wallRight -> mat.setShiness(12.8);
    wallRight -> position = glm::vec3(20.5f, 18.5f, 0.0f);
    wallRight -> mat.setReflectioness(0.1);
    wallRight -> mat.setOpaqueness(false);
    wallRight -> mat.setIndexOfRefraction(1.5);
    scene -> addModel(wallRight);

    Polygon3D* wallBack = new Polygon3D(0, 0, 40, 40, 1);
    wallBack -> setType(POLYGON_TYPE::CUBE);
    wallBack -> mat.setAmbient(0.135f, 0.2225f, 0.1575f, 0.95f);
    wallBack -> mat.setDiffuse(0.54f, 0.89f, 0.63f, 0.95f);
    wallBack -> mat.setSpecular(0.316228f, 0.316228f, 0.316228f, 0.95f);
    wallBack -> mat.setEmission();
    wallBack -> mat.setShiness(12.8);
    wallBack -> position = glm::vec3(0.0f, 18.5f, -20.5f);
    wallBack -> mat.setReflectioness(0.1);
    wallBack -> mat.setOpaqueness(false);
    wallBack -> mat.setIndexOfRefraction(1.5);
    scene -> addModel(wallBack);

    Light* lightSun = new Light();
    lightSun -> setLightID(GL_LIGHT0);
    lightSun -> setAmbient(0.1f, 0.1f, 0.1f, 1.0f);
    lightSun -> setDiffuse(0.5f, 0.5f, 0.5f, 1.0f);
    lightSun -> setSpecular(0.4f, 0.4f, 0.4f, 1.0f);
    lightSun -> setPosition(25.0f, 75.0f, 25.0f);
    lightSun -> setLightType(true);
    scene->addLight(lightSun);

    Light* lightCup = new Light();
    lightCup -> setLightID(GL_LIGHT1);
    lightCup -> setAmbient(0.3f, 0.0f, 0.0f, 1.0f);
    lightCup -> setDiffuse(0.4f, 0.0f, 0.0f, 1.0f);
    lightCup -> setSpecular(0.2f, 0.0f, 0.0f, 1.0f);
    lightCup -> setPosition(0.0f, 18.5f, -19.0f);
    lightCup -> setLightType(true);
    scene->addLight(lightCup);
 
    Light* lightConeLamp = new Light();
    lightConeLamp -> setLightID(GL_LIGHT2);
    lightConeLamp -> setAmbient(0.0f, 0.0f, 0.2f, 1.0f);
    lightConeLamp -> setDiffuse(0.0f, 0.0f, 0.4f, 1.0f);
    lightConeLamp -> setSpecular(0.0f, 0.0f, 0.4f, 1.0f);
    lightConeLamp -> setPosition(-25.0f, 75.0f, -25.0f);
    lightConeLamp -> setLightType(true);
    lightConeLamp -> setDirection(0.0f , 50.0f, 0.0f);
    lightConeLamp -> setExponent(128.0f);
    lightConeLamp -> setCutOffAngle(90.0f);
    scene->addLight(lightConeLamp);

    Light* lightSphereLamp = new Light();
    lightSphereLamp -> setLightID(GL_LIGHT3);
    lightSphereLamp -> setAmbient(0.03125f, 0.03125f, 0.03125f, 1.0f);
    lightSphereLamp -> setDiffuse(0.1775f, 0.1775f, 0.1775f, 1.0f);
    lightSphereLamp -> setSpecular(0.773911f, 0.773911f, 0.773911f, 1.0f);
    lightSphereLamp -> setPosition(75.0f, 25.0f, 25.0f);
    lightSphereLamp -> setLightType(true);
    scene->addLight(lightSphereLamp);

    rayTracer = new RayTracing(width, height, cameraOriginPoint);

    glEnable(GL_DEPTH_TEST);
    scene -> lightOn();
    scene -> lightSetting();
    glEnable(GL_TEXTURE_2D);
    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    box -> initTexture();
    concreteSphere -> initTexture();
}


int main(int argc, char** argv) {    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(width, height);
    glutCreateWindow("3D viewer");

    reshape(width, height);    

    objX = new GLdouble;
    objY = new GLdouble;
    objZ = new GLdouble;

    
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClearDepth(1.0f);
    glDepthFunc(GL_LESS);
    init();

    glutIdleFunc(idle);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardCB);
    glutSpecialFunc(keyboardSpecialCB);
    glutReshapeFunc(reshape);
    glutMotionFunc(motionCB);
    glutMouseFunc(mouseButtonCB);

    glutMainLoop();
    delete objX;
    delete objY;
    delete objZ;

    delete scene;
    delete rayTracer;

    return 0;
}
