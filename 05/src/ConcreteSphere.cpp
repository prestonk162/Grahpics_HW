#include "ConcreteSphere.h"

ConcreteSphere::ConcreteSphere(/* args */)
{
    scale = 1.0f;
    position = glm::dvec3(0,0,0);
    orientation = glm::quat(1,0,0,0);
    textureLoadEnable = false;
    textureID = 0;
    imageHeight = 0;
    imageWidth = 0;
    textureData = nullptr;
    sphere = gluNewQuadric();
    setObjType(OBJECT_TYPE::TEXTURE_SPHERE);
}

void ConcreteSphere::initTexture(void)
{
    FIBITMAP* bitmap32 = createBitmap(filename);

    if (bitmap32 == nullptr) 
    {
        textureLoadEnable = false;
        return;
    }
    else
    {
        textureLoadEnable = true;
    }

    imageWidth = FreeImage_GetWidth(bitmap32);
    imageHeight = FreeImage_GetHeight(bitmap32);
    textureData = FreeImage_GetBits(bitmap32);

    generateTexture();

    FreeImage_Unload(bitmap32);
}

void ConcreteSphere::generateTexture(void)
{
    if(!textureLoadEnable) return;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)textureData);
}

void ConcreteSphere::draw(void) const
{
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat.getAmbient());
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat.getDiffuse());
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat.getSpecular());
    glMaterialfv(GL_FRONT, GL_EMISSION, mat.getEmission());
    glMaterialfv(GL_FRONT, GL_SHININESS, mat.getShiness());

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(position.x, position.y, position.z);

    glEnable(GL_TEXTURE_2D);
    glColor4f(1,1,1,1);
    gluQuadricDrawStyle(sphere, GLU_FILL);
    glBindTexture(GL_TEXTURE_2D, textureID);
    gluQuadricTexture(sphere, GL_TRUE);
    gluQuadricNormals(sphere, GLU_SMOOTH);
    gluSphere(sphere, scale, 20, 20);
    glDisable(GL_TEXTURE_2D);
}

void ConcreteSphere::setFilename(const std::string& name)
{
    filename = name;
}

void ConcreteSphere::setTextureID(const GLuint& id)
{
    textureID = id;
}

ConcreteSphere::~ConcreteSphere()
{
}