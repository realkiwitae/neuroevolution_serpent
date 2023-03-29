#include "Snake.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Engine/Utils/CommonValues.h"

#include "./GameValues.h"

#include <iostream>

extern GLfloat deltaTime,now;

Snake::Snake()
{

}

Snake::~Snake()
{

}

void Snake::init(Model* model)
{
    birth = now;
    model_snake = model;
    score = 0.f;
    bIsAlive = true;
}

void Snake::update(){
    if(!bIsAlive){
        return;
    }
   // health -= deltaTime;
    if(health<0.f){
        kill();
        return;
    }
    double inputs[3] = {
  //      fabs(game_arena_ceiling_y - pos.y + collision_h/2.f),
//        fabs(pos.y - collision_h/2.f - game_arena_floor_y),
        0,
        0,
        0,
        };

    snake_nn->calc(inputs);

    double* out = snake_nn->getOutput();
    bUp = out[0] > .05f;

  //  std::cout << pos.y << std::endl;
}

void Snake::render(GLuint uniformModel, GLuint uniformSpecularIntensity, GLuint uniformShininess)
{
    if(!bIsAlive){
        return;
    }
    glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0,pos.y,pos.x));
	model = glm::scale(model, glm::vec3(.1f, .1f, .1f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
//	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	model_snake->RenderModel();
}

void Snake::kill(){
    bIsAlive = false;
}

void Snake::possess(DNA* dna){
    bIsAI = true;
    snake_dna = dna;
    int stage_l[game_snakenn_nb_layers] = {3, 6, 1};
    if(!snake_nn)snake_nn = new NeuralNetwork(stage_l, game_snakenn_nb_layers);
    snake_nn->loadCoeffs(dna->data);
}

void Snake::addScore(GLfloat s){
    if(!bIsAlive)return;
    score += s;
}