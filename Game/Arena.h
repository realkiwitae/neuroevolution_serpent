#ifndef ARENA_H
#define ARENA_H

#pragma once

#include <vector>

#include "./Snake.h"
#include "./GameValues.h"
#include "./Food.h"

class Arena
{
public:
    Arena();
    ~Arena();

    void init();
    bool update();
    void render(GLuint uniformModel, GLuint uniformSpecularIntensity, GLuint uniformShininess);
    void checkCollision(Snake* s);
private:
    Food foods[game_arena_food_nb]; 
    Mesh *ground;
    Model *wall;
    Model* food;
    Texture* dirtTexture;
    Texture* brickTexture;

};

#endif