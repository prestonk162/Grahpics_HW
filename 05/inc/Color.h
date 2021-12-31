#pragma once
#include <GL/glut.h>

class Color
{
private:
    /* data */
public:
    GLfloat r;
    GLfloat g;
    GLfloat b;
    Color(/* args */);
    void clamp(void);
    Color(const float& red, const float& green, const float& blue);

    Color& operator+=(const Color& c);
    Color& operator*=(const Color& c);
    Color& operator*=(const GLfloat& f);
    Color& operator=(const Color& c);
    ~Color();
};

Color operator+(const Color& c1, const Color& c2);
Color operator*(const Color& c1, const Color& c2);
Color operator*(const Color& c1, const GLfloat& f);