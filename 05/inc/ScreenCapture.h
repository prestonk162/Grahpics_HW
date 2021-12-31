#pragma once
#include <FreeImage.h>
#include <GL/glut.h>
#include <string>
#include <iostream>

void captureScreen(const size_t& width, const size_t& height);
void printRayTracedImage(const size_t& width, const size_t& height, const GLfloat * floatBuffer);
FIBITMAP * createBitmap(const std::string& filename);