#ifndef COMMONVALS
#define COMMONVALS

#pragma once

#include "stb_image.h"

#include "GL/glew.h"

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

bool CHECK_BIT(unsigned long var, int pos);
void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount, 
						unsigned int vLength, unsigned int normalOffset);
#endif