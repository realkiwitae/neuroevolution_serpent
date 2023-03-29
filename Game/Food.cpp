#include "Food.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include "../Engine/Utils/CommonValues.h"

#include "./GameValues.h"

extern GLfloat deltaTime;

Food::Food()
{

}

Food::~Food()
{

}

void Food::init(Model* _Food)
{
    food = _Food;
    pos.x =  glm::linearRand(0.f,game_arena_width -game_arena_food_radius/2.f);
    pos.y =  glm::linearRand(0.f,game_arena_height -game_arena_food_radius/2.f);

}

void Food::update(){

}

void Food::render(GLuint uniformModel, GLuint uniformSpecularIntensity, GLuint uniformShininess)
{
    //bot Food
    glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(pos.x,0,game_arena_floor_y+game_arena_food_radius/2.f));
	model = glm::scale(model, glm::vec3(1.f, 1.f, 1.f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
//	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    food->RenderModel();

}

void Food::checkCollision(Snake* b){
    glm::vec2 bird_pos = b->getPos();

    if((b->getPos() - pos).length() < (game_snake_segment_radius+game_arena_food_radius)){
        b->kill();
    }
}