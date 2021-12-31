#include "ScreenCapture.h"

void captureScreen(const size_t& width, const size_t& height)
{
    if(width == 0 || height == 0) return;

    BYTE* pixels = new BYTE[3 * width * height];

    glReadPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, pixels);

    FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, (int)width, (int)height, 3 * width, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
    FreeImage_Save(FIF_JPEG, image, "sample01.jpeg", 0);

    FreeImage_Unload(image);
    delete [] pixels;
}

void printRayTracedImage(const size_t& width, const size_t& height, const GLfloat * floatBuffer)
{
    if (width == 0 || height == 0 || floatBuffer == nullptr) return;
    
    BYTE* pixels = new BYTE[3 * width * height];

    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            pixels[3*y*width + 3*x + 0] = (BYTE)(floatBuffer[3*y*width + 3*x + 2] * 255);
            pixels[3*y*width + 3*x + 1] = (BYTE)(floatBuffer[3*y*width + 3*x + 1] * 255);
            pixels[3*y*width + 3*x + 2] = (BYTE)(floatBuffer[3*y*width + 3*x + 0] * 255);
        }
    }

    FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, (int)width, (int)height, 3 * width, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
    FreeImage_Save(FIF_JPEG, image, "raytraced_sample01.jpeg", 0);

    FreeImage_Unload(image);
    delete [] pixels;
}

FIBITMAP* createBitmap(const std::string& filename)
{
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename.c_str(), 0);

    if (format == 1)
    {
        std::cout << "Could not find image: " << filename << "- Aborting" << std::endl;
        return nullptr;
    }   

    if (format == FIF_UNKNOWN)
    {
        std::cout << "Could not determine file format - attempting to get from file extension..." << std::endl;
        format = FreeImage_GetFIFFromFilename(filename.c_str());

        if(!FreeImage_FIFSupportsReading(format))
        {
            std::cout << "Detected image format cannot be read!" << std::endl;
            return nullptr;
        }
    }
    
    FIBITMAP * bitmap = FreeImage_Load(format, filename.c_str());

    GLint bitsPerPixel = FreeImage_GetBPP(bitmap);

    FIBITMAP* bitmap32;

    if(bitsPerPixel == 32)
    {
        std::cout << "Source image has " << bitsPerPixel << " bits per pixel. Skipping conversion." << std::endl;
        bitmap32 = bitmap;
    }
    else
    {
        std::cout << "Source image has " << bitsPerPixel << " bits per pixel. Converting to 32-bit color" << std::endl;
        bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
    }
    return bitmap32;
}