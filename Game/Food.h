#ifndef Food_H
#define Food_H

#pragma once

#include "glm/glm.hpp"
#include <GL/glew.h>

#include "../Engine/Modeling/Model.h"
#include "./Snake.h"

class Food
{
public:
    Food();
    Food(int _id):id(_id){}
    ~Food();
    void init(Model* _Food);
    void update();
    void render(GLuint uniformModel, GLuint uniformSpecularIntensity, GLuint uniformShininess);
    bool checkCollision(double x, double y, double r);
    glm::vec2 getPos(){return pos;}
    bool collected = false;
private:
    glm::vec2 pos;
    GLfloat size_food_radius;
    Model* food;
    int id = 0;
};

#endif