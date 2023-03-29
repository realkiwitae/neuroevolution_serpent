#include "./CommonValues.h"

bool CHECK_BIT(unsigned long var, int pos){return var & (1UL << pos);}

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
GLfloat now = 0.0f;
