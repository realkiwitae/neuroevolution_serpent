#include "Arena.h"
#include "GameValues.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Arena::Arena()
{

}

Arena::~Arena()
{

}

void Arena::init()
{   
    if(!brickTexture){
        brickTexture = new Texture("Textures/brick.png");
        brickTexture->LoadTextureA();
    }

    if(!food){
        food = new Model();
        food->LoadModel("snake/food");
    }

    incoming_pipe_id = 0;
    for(size_t i = 0; i < game_arena_food_nb; i++){
        foods[i] = Food(i);
        foods[i].init(food);
    }

    if(!ground){
        ground = new Model();
        ground->LoadModel("snake/ground");
    }
}

bool Arena::update()
{
    bool b = false;
    return b;
}

void Arena::render(GLuint uniformModel, GLuint uniformSpecularIntensity, GLuint uniformShininess)
{
    glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, game_arena_floor_y + .8f, 0.0f));
	model = glm::scale(model, glm::vec3(10.f, 1.f, 10.f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//dirtTexture.UseTexture();
	//shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	ground->RenderModel();


    for(size_t i = 0; i < game_arena_food_nb; i++){
        foods[i].render(uniformModel, uniformSpecularIntensity, uniformShininess);
    }
}

void Arena::checkCollision(Snake* s){
    if(s->isDead())return;
    glm::vec2 snake_pos = s->getPos();

    for(Food& p : foods){
        p.checkCollision(s);
    }
}