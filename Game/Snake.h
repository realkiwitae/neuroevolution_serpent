#ifndef BIRD_H
#define BIRD_H

#pragma once

#include "glm/glm.hpp"
#include <GL/glew.h>

#include "../Engine/Modeling/Model.h"

#include "./GameValues.h"

#include "../kiwinn/DNA.hpp"
#include "../kiwinn/NeuralNetwork.hpp"

class Snake
{
public:
    Snake();
    ~Snake();

    void init(Model* model);
    void update();
    void render(GLuint uniformModel, GLuint uniformSpecularIntensity, GLuint uniformShininess);
    void setbUp(bool val){bUp = val;}
    void kill();
    bool isDead(){return !bIsAlive;}
    glm::vec2 getPos(){return pos;}
    GLfloat getCollisionW(){return collision_w;}
    GLfloat getCollisionH(){return collision_h;}

    void possess(DNA* dna);
    void addScore(GLfloat s);
private:
    Model* model_snake;
    //Physics
    glm::vec2 pos;
    GLfloat speed = 0.f;
    GLfloat mass = 2.f;
    GLfloat maxspeed = 10.f;// ms-1
    GLfloat collision_w = 1.f;
    GLfloat collision_h = .5f;
    bool bUp = false;
    bool bIsAlive = true;
    GLfloat birth = 0.f;
    GLfloat score = 0.f;
    bool bIsAI = false;
    DNA* snake_dna;
    NeuralNetwork* snake_nn = nullptr;
    GLfloat health = game_snake_maxhealth;
};

#endif