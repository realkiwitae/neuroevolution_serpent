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

struct Segment{
    public:
    glm::vec2 pos = glm::vec2(0,0);

};

public:
    Snake();
    ~Snake();

    void init(Model* model);
    void update();
    void update(glm::vec2 change);
    void render(GLuint uniformModel, GLuint uniformSpecularIntensity, GLuint uniformShininess);
    void kill();
    bool isDead(){return !bIsAlive;}
    glm::vec2 getScreenCoord();
    glm::vec2 getPos(){return body[0].pos;}
    void possess(DNA* dna);
    void addScore(GLfloat s);
private:
    Model* model_segment;
    std::vector<Segment> body = {};
    glm::vec2 dir;
    GLfloat speed = game_arena_snake_speed;
    bool bIsAlive = true;
    GLfloat birth = 0.f;
    GLfloat score = 0.f;
    bool bIsAI = false;
    DNA* snake_dna;
    NeuralNetwork* snake_nn = nullptr;
    GLfloat health = game_snake_maxhealth;
};

#endif