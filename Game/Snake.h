#ifndef BIRD_H
#define BIRD_H

#pragma once

#include "glm/glm.hpp"
#include <GL/glew.h>

#include "../Engine/Modeling/Model.h"
#include "../Engine/Utils/CommonValues.h"

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
    void render(GLuint uniformModel, GLuint uniformSpecularIntensity, GLuint uniformShininess);
    void kill();
    bool isDead(){return !bIsAlive;}
    glm::vec2 getScreenCoord();
    glm::vec2 getPos(){return body[0].pos;}
    void possess(DNA* dna);
    void addScore(GLfloat s);
    bool checkCollision(double x, double y, double r);
    bool selfCollisionCheck();
    void endSpawn(){bspawning = false;};
private:
    Model* model_segment;
    std::vector<Segment> body = {};
    GLfloat age = 0.f;
    GLfloat angle;
    GLfloat score;
    GLfloat hue;
    GLfloat deathFade = 180.f;
    GLfloat health;
    bool bIsAI = false;
    bool bIsAlive;
    DNA* snake_dna;
    NeuralNetwork* snake_nn = nullptr;
    bool bspawning;
};

#endif