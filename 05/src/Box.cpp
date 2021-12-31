#include "Box.h"

Box::Box(/* args */)
{
    setObjType(OBJECT_TYPE::TEXTURE_BOX);
    position = glm::dvec3(0, 0, 0);
    scale = 1.0f;
    orientation = glm::quat(1,0,0,0);
    textureLoadEnable = false;
    textureID = 0;
    imageHeight = 0;
    imageWidth = 0;
    textureData = nullptr;
}
void Box::setFilename(const std::string& name)
{
    filename = name;
}

void Box::setTextureID(const GLuint& id)
{
    textureID = id;
}

void Box::initTexture(void)
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

    // glEnable(GL_COLOR_MATERIAL);
    // glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void Box::generateTexture(void)
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

    // glBindTexture(GL_TEXTURE_2D, 0);
    
}

// FIBITMAP* Box::createBitmap(void)
// {
//     FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename.c_str(), 0);

//     if (format == 1)
//     {
//         std::cout << "Could not find image: " << filename << "- Aborting" << std::endl;
//         textureLoadEnable = false;
//         return nullptr;
//     }   

//     if (format == FIF_UNKNOWN)
//     {
//         std::cout << "Could not determine file format - attempting to get from file extension..." << std::endl;
//         format = FreeImage_GetFIFFromFilename(filename.c_str());

//         if(!FreeImage_FIFSupportsReading(format))
//         {
//             std::cout << "Detected image format cannot be read!" << std::endl;
//             return nullptr;
//         }
//     }
    
//     FIBITMAP * bitmap = FreeImage_Load(format, filename.c_str());

//     GLint bitsPerPixel = FreeImage_GetBPP(bitmap);

//     FIBITMAP* bitmap32;

//     if(bitsPerPixel == 32)
//     {
//         std::cout << "Source image has " << bitsPerPixel << " bits per pixel. Skipping conversion." << std::endl;
//         bitmap32 = bitmap;
//     }
//     else
//     {
//         std::cout << "Source image has " << bitsPerPixel << " bits per pixel. Converting to 32-bit color" << std::endl;
//         bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
//     }
//     textureLoadEnable = true;
//     return bitmap32;
// }

void Box::draw(void) const
{
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat.getAmbient());
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat.getDiffuse());
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat.getSpecular());
    glMaterialfv(GL_FRONT, GL_EMISSION, mat.getEmission());
    glMaterialfv(GL_FRONT, GL_SHININESS, mat.getShiness());

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(position.x, position.y, position.z);
    // glutSolidCube(1.0f);    

    glm::dvec3 vertices[8];
    vertices[0] = glm::dvec3(-scale, scale, scale)*0.5;
    vertices[1] = glm::dvec3(-scale, scale, -scale)*0.5;
    vertices[2] = glm::dvec3(scale, scale, -scale)*0.5;
    vertices[3] = glm::dvec3(scale, scale, scale)*0.5;
    vertices[4] = glm::dvec3(-scale, -scale, scale)*0.5;
    vertices[5] = glm::dvec3(-scale, -scale, -scale)*0.5;
    vertices[6] = glm::dvec3(scale, -scale, -scale)*0.5;
    vertices[7] = glm::dvec3(scale, -scale, scale)*0.5;

    // vertices[0] = position + glm::dvec3(-scale, scale, scale)*0.5;
    // vertices[1] = position + glm::dvec3(-scale, scale, -scale)*0.5;
    // vertices[2] = position + glm::dvec3(scale, scale, -scale)*0.5;
    // vertices[3] = position + glm::dvec3(scale, scale, scale)*0.5;
    // vertices[4] = position + glm::dvec3(-scale, -scale, scale)*0.5;
    // vertices[5] = position + glm::dvec3(-scale, -scale, -scale)*0.5;
    // vertices[6] = position + glm::dvec3(scale, -scale, -scale)*0.5;
    // vertices[7] = position + glm::dvec3(scale, -scale, scale)*0.5;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_QUADS);
        glNormal3i(0, 1, 0);
        glTexCoord2i(0,0); 
        glVertex3d(vertices[0].x, vertices[0].y, vertices[0].z);
        glTexCoord2i(0,1);
        glVertex3d(vertices[1].x, vertices[1].y, vertices[1].z);
        glTexCoord2i(1,1);
        glVertex3d(vertices[2].x, vertices[2].y, vertices[2].z);
        glTexCoord2i(1,0);
        glVertex3d(vertices[3].x, vertices[3].y, vertices[3].z);

        glNormal3i(0, 0, 1);
        glTexCoord2i(0,0); 
        glVertex3d(vertices[4].x, vertices[4].y, vertices[4].z);
        glTexCoord2i(0,1);
        glVertex3d(vertices[0].x, vertices[0].y, vertices[0].z);
        glTexCoord2i(1,1);
        glVertex3d(vertices[3].x, vertices[3].y, vertices[3].z);
        glTexCoord2i(1,0);
        glVertex3d(vertices[7].x, vertices[7].y, vertices[7].z);

        glNormal3i(1, 0, 0);
        glTexCoord2i(0,0); 
        glVertex3d(vertices[7].x, vertices[7].y, vertices[7].z);
        glTexCoord2i(0,1);
        glVertex3d(vertices[3].x, vertices[3].y, vertices[3].z);
        glTexCoord2i(1,1);
        glVertex3d(vertices[2].x, vertices[2].y, vertices[2].z);      
        glTexCoord2i(1,0);
        glVertex3d(vertices[6].x, vertices[6].y, vertices[6].z);

        glNormal3i(0, 0, -1);
        glTexCoord2i(0,0); 
        glVertex3d(vertices[6].x, vertices[6].y, vertices[6].z);
        glTexCoord2i(0,1);
        glVertex3d(vertices[2].x, vertices[2].y, vertices[2].z);
        glTexCoord2i(1,1);
        glVertex3d(vertices[1].x, vertices[1].y, vertices[1].z);
        glTexCoord2i(1,0);
        glVertex3d(vertices[5].x, vertices[5].y, vertices[5].z);

        glNormal3i(-1, 0, 0);
        glTexCoord2i(0,0); 
        glVertex3d(vertices[5].x, vertices[5].y, vertices[5].z);
        glTexCoord2i(0,1);
        glVertex3d(vertices[1].x, vertices[1].y, vertices[1].z);
        glTexCoord2i(1,1);
        glVertex3d(vertices[0].x, vertices[0].y, vertices[0].z);
        glTexCoord2i(1,0);
        glVertex3d(vertices[4].x, vertices[4].y, vertices[4].z);

        glNormal3i(0, -1, 0);
        glTexCoord2i(0,0); 
        glVertex3d(vertices[5].x, vertices[5].y, vertices[5].z);
        glTexCoord2i(0,1);
        glVertex3d(vertices[4].x, vertices[4].y, vertices[4].z);
        glTexCoord2i(1,1);
        glVertex3d(vertices[7].x, vertices[7].y, vertices[7].z); 
        glTexCoord2i(1,0);
        glVertex3d(vertices[6].x, vertices[6].y, vertices[6].z);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    // glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
    
}

Box::~Box()
{
}