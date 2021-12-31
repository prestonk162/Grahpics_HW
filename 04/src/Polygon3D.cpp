#include "Polygon3D.h"

void Polygon3D::draw(void) const
{
    // glShadeModel(GL_SMOOTH);
    // glEnable(GL_POLYGON_SMOOTH);
    // glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat.getAmbient());
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat.getDiffuse());
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat.getSpecular());
    glMaterialfv(GL_FRONT, GL_EMISSION, mat.getEmission());
    glMaterialfv(GL_FRONT, GL_SHININESS, mat.getShiness());

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(position.x, position.y, position.z);
    
    switch (polygonType)
    {
    case SPHERE:
        glScalef(scale, scale, scale);    
        glutSolidSphere(o_radius, slices, stacks);
        break;
    case TORUS:
        glScalef(scale, scale, scale);
        glutSolidTorus(i_radius, o_radius, slices, stacks);
        break;
    case TETRAHEDRONE:
        glScalef(scale, scale, scale);
        glutSolidTetrahedron();
        break;
    case TEAPOT:
        glutSolidTeapot(scale);
        break;
    case CONE:
        glScalef(scale, scale, scale);
        glutSolidCone(o_radius, height, slices, stacks);
        break;
    case DEDECAHEDRON:
        glScalef(scale, scale, scale);
        glutSolidDodecahedron();
        break;
    case ICOSAHEDRON:
        glScalef(scale, scale, scale);
        glutSolidIcosahedron();
        break;
    case OCTAHEDRON:
        glScalef(scale, scale, scale);
        glutSolidOctahedron();
        break;
    default:
        break;
    }
    // glDisable(GL_POLYGON_SMOOTH);
    // glHint(GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);
    glPopMatrix();
}

Polygon3D::Polygon3D()
{
    o_radius = 0;
    i_radius = 0;
    slices = 0;
    stacks = 0;
    height = 0;
}

Polygon3D::Polygon3D(const size_t& o_r, const size_t& i_r, const size_t& sl, const size_t& st, const size_t& h)
{
    o_radius = o_r;
    i_radius = i_r;
    slices = sl;
    stacks = st;
    height = h;
}

void Polygon3D::setType(const POLYGON_TYPE& type)
{
    polygonType = type;
}

Polygon3D::~Polygon3D()
{
}