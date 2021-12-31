#include "Color.h"

Color::Color()
{
    r = 0.0f;
    g = 0.0f;
    b = 0.0f;
}

void Color::clamp(void)
{
    if (r > 1.0f)
    {
        r = 1.0f;
    }
    else if(r < 0.0f)
    {
        r = 0.0f;
    }

    if (g > 1.0f)
    {
        g = 1.0f;
    }
    else if(g < 0.0f)
    {
        g = 0.0f;
    }

    if (b > 1.0f)
    {
        b = 1.0f;
    }
    else if(b < 0.0f)
    {
        b = 0.0f;
    }
}

Color::Color(const float& red, const float& green, const float& blue)
{
    r = red;
    g = green;
    b = blue;
}

Color::~Color()
{
}

Color& Color::operator+=(const Color& c)
{
    r += c.r;
    g += c.g;
    b += c.b;
    return (*this);
}

Color& Color::operator*=(const Color& c)
{
    r *= c.r;
    g *= c.g;
    b *= c.b;
    return (*this);
}

Color& Color::operator*=(const GLfloat& f)
{
    r *= f;
    g *= f;
    b *= f;
    return (*this);
}

Color& Color::operator=(const Color& c)
{
    r = c.r;
    g = c.g;
    b = c.b;
    return (*this);
}

Color operator+(const Color& c1, const Color& c2)
{
    return Color(c1.r+c2.r, c1.g+c2.g, c1.b+c2.b);
}
Color operator*(const Color& c1, const Color& c2)
{
    return Color(c1.r*c2.r, c1.g*c2.g, c1.b*c2.b);
}
Color operator*(const Color& c1, const GLfloat& f)
{
    return Color(c1.r*f, c1.g*f, c1.b*f);
}