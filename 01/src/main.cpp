#include <stdio.h>
#include <string>
#include <GL/glut.h>
#include <GL/glu.h>
#include <time.h>
#include "Wheel.h"
#include "Turret.h"
#include "Model.h"

#define CYLINDER_RADIUS 0.5
#define CYLINDER_HEIGHT 1
#define CYLINDER_STACK 10
#define CYLINDER_SLICE 10

#define SIGNAL_NUMS 5

#define KEY_W_UP 0
#define KEY_S_UP 1
#define KEY_A_UP 2
#define KEY_D_UP 3
#define KEY_SPACE_UP 4
#define KEY_UP 5
#define KEY_DOWN 6

#define FRAME 60.0

/*
GLint ccwRotatedAngle;

GLboolean rightRotate;
GLboolean leftRotate;

Wheel* rearWheel;
Wheel* frontWheel;
*/

GLboolean turretRotate;
GLboolean turretRotateCW;
GLboolean gunRotate;
GLboolean gunUp;
GLboolean gunFire;
GLboolean wheelRun;
GLboolean wheelFront;

GLboolean keyArray[SIGNAL_NUMS];

GLint windowHeight;
GLint windowWidth;

Model* aaTurret;

clock_t start_clock = clock();
clock_t end_clock;

std::string turretUI = std::string("Turret Angle[0:360]: ");
std::string minigunUI = std::string("Minigun Angle [-10:60]: ");
std::string fireUI = std::string("Minigun Firing: ");
std::string turretKeyUI = std::string("Turret Rotation WRT Y Axis - a: CCW, d: CW");
std::string minigunKeyUI = std::string("Minigun Frame Rotation - w: Up, s: Down");
std::string fireKeyUI = std::string("Minigun Fire - space key down: Fire, space key up: Cease Fire");
std::string turretAngle;
std::string minigunAngle;

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

void init(void) 
{
    /*
    rightRotate = false;
    leftRotate = false;

    rearWheel = new Wheel(0.2, 0.8, 0.5);
    frontWheel = new Wheel(0.2, 0.8, 0.5);
    */
    aaTurret = new Model(4, 1, 3);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
}

void frameDraw(void) {
    glPushMatrix();
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
        glVertex3f(4, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 3.5, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 4);
        glVertex3f(0, 0, 0);
    glEnd();
    glPushMatrix();
    glColor3f(1, 1, 0);
    glRasterPos3f(4, -0.15, 0);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'X');
    glRasterPos3f(-0.15, 3.5, 0);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'Y');
    glRasterPos3f(-0.15, -0.15, 4);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'Z');
    glPopMatrix();
    glPopMatrix();
}

void uiDraw(void)
{
    
    turretAngle = std::to_string(aaTurret -> getTurretCurrentAngle());
    minigunAngle = std::to_string(aaTurret -> getMinigunCurrentAngle());
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    glLoadIdentity();
    glColor3f(RED_RGB);
    
    glRasterPos2f(windowWidth*0.01, windowHeight*0.96);
    for (size_t i = 0; i < turretUI.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, turretUI[i]);
    }
    for (size_t i = 0; i < turretAngle.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, turretAngle[i]);
    }

    glRasterPos2f(windowWidth*0.01, windowHeight*0.96-24);

    for (size_t i = 0; i < minigunUI.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, minigunUI[i]);
    }
    for (size_t i = 0; i < minigunAngle.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, minigunAngle[i]);
    }

    glRasterPos2f(windowWidth*0.01, windowHeight*0.96-(24<<1));

    for (size_t i = 0; i < fireUI.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, fireUI[i]);
    }
    gunFire ? glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'Y') : glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'N');

    glRasterPos2f(windowWidth*0.01, windowHeight*0.96-(24*3));

    for (size_t i = 0; i < turretKeyUI.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, turretKeyUI[i]);
    }

    glRasterPos2f(windowWidth*0.01, windowHeight*0.96-(24<<2));

    for (size_t i = 0; i < minigunKeyUI.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, minigunKeyUI[i]);
    }

    glRasterPos2f(windowWidth*0.01, windowHeight*0.96-(24*5));

    for (size_t i = 0; i < fireKeyUI.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, fireKeyUI[i]);
    }

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    //glRotatef(20, 1, 0, 0);
    uiDraw();
    frameDraw();
    /*
    glTranslatef(-1.5, 0.0, 0.0);
    if (rightRotate != leftRotate)
    {
        glPushMatrix();
        leftRotate ? glRotatef(30, 0, 1, 0) : glRotatef(30, 0, -1, 0);
        rearWheel -> draw();
        glPopMatrix();
    }
    else
    {
        rearWheel -> draw();
    }

    glTranslatef(3.0, 0.0, 0.0);
        frontWheel -> draw();
    glTranslatef(-1.5, 0.0, 0.0);
    glPopMatrix();
    */

    aaTurret -> draw();
    glPopMatrix();

    glutSwapBuffers();
    errorCheck();
}

void idle(void)
{
    end_clock = clock();
    if (end_clock - start_clock > 1000 / FRAME)
    {
        turretRotate = keyArray[KEY_D_UP] || keyArray[KEY_A_UP];
        gunRotate = keyArray[KEY_W_UP] || keyArray[KEY_S_UP];
        // wheelRun = keyArray[KEY_UP] ^ keyArray[KEY_DOWN];

        turretRotateCW = keyArray[KEY_D_UP] && !keyArray[KEY_A_UP];
        gunUp = keyArray[KEY_W_UP] && !keyArray[KEY_S_UP];
        gunFire = keyArray[KEY_SPACE_UP];
        // wheelFront = keyArray[KEY_UP] && !keyArray[KEY_DOWN];
        
        /*
        rearWheel -> changeSpeed(ccwRotatedAngle);
        frontWheel -> changeSpeed(ccwRotatedAngle);
        rearWheel -> update();
        frontWheel -> update();
        */
        aaTurret -> control(turretRotate, turretRotateCW, gunRotate, gunUp, gunFire);
    }
    glutPostRedisplay();
}

void resize(GLint w, GLint h)
{
    windowHeight = h;
    windowWidth = w;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, ((GLfloat)w) / h, 1.0, 20.0);
    // glOrtho(-10, 10, -10, 10, 1, 20);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //glTranslatef(0.0, 0.0, -5.0);
    gluLookAt(4, 3, 5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboardDown(GLubyte key, GLint x, GLint y)
{
    switch (key)
    {
    case 'w':
        keyArray[KEY_W_UP] = true;
        break;
    
    case 's':
        keyArray[KEY_S_UP] = true;
        break;

    case 'a':
        keyArray[KEY_A_UP] = true;
        break;

    case 'd':
        keyArray[KEY_D_UP] = true;
        break;

    // case GLUT_KEY_UP:
    //     keyArray[KEY_UP] = true;
    //     break;

    // case GLUT_KEY_DOWN:
    //     keyArray[KEY_DOWN] = true;
    //     break;

    case 32:
        keyArray[KEY_SPACE_UP] = true;
        break;

    case 27:
        exit(0);
        break;
    
    default:
        break;
    }
    /*
    switch (key)
    {
    case 'w':
        ccwRotatedAngle = 2;
        // glutPostRedisplay();
        break;
    
    case 's':
        ccwRotatedAngle = -2;
        // glutPostRedisplay();
        break;

    case 'a':
        leftRotate = true;
        // glutPostRedisplay();
        break;
    
    case 'd':
        rightRotate = true;
        // glutPostRedisplay();
        break;

    case 27:
        exit(0);
        break;
    
    default:
        break;
    }
    */
}

void keyboardUp(GLubyte key, GLint x, GLint y) 
{
    switch (key)
    {
    case 'w':
        keyArray[KEY_W_UP] = false;
        break;
    
    case 's':
        keyArray[KEY_S_UP] = false;
        break;

    case 'a':
        keyArray[KEY_A_UP] = false;
        break;

    case 'd':
        keyArray[KEY_D_UP] = false;
        break;

    case 32:
        keyArray[KEY_SPACE_UP] = false;
        break;

    // case GLUT_KEY_UP:
    //     keyArray[KEY_UP] = false;
    //     break;

    // case GLUT_KEY_DOWN:
    //     keyArray[KEY_DOWN] = false;
    //     break;

    case 27:
        exit(0);
        break;
    
    default:
        gunRotate = false;
        turretRotate = false;
        break;
    }
    /*
    switch (key)
    {
    case 'w':
        ccwRotatedAngle = 0;
        // glutPostRedisplay();
        break;
    
    case 's':
        ccwRotatedAngle = 0;
        // glutPostRedisplay();
        break;

    case 'a':
        leftRotate = false;
        // glutPostRedisplay();
        break;
    
    case 'd':
        rightRotate = false;
        // glutPostRedisplay();
        break;

    case 27:
        exit(0);
        break;
    
    default:
        break;
    }
    */
}

int main(int argc, char** argv) 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1280, 960);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("HW01");
    
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    glutIdleFunc(idle);
    
    glutMainLoop();
    
    // delete rearWheel;
    // delete frontWheel;
    delete aaTurret;

    return 0;
}