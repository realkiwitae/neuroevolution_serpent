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
    pos.x =  glm::linearRand(-game_arena_width/2.f +game_arena_food_radius/2.f,game_arena_width/2.f -game_arena_food_radius/2.f);
    pos.y =  glm::linearRand(-game_arena_height/2.f +game_arena_food_radius/2.f,game_arena_height/2.f -game_arena_food_radius/2.f);
    collected = false;
}

void Food::update(){

}

void Food::render(GLuint uniformModel, GLuint uniformSpecularIntensity, GLuint uniformShininess)
{
    //bot Food
    glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(pos.x,game_arena_floor_y+game_arena_food_radius/2.f,pos.y));
	model = glm::scale(model, glm::vec3(1.f, 1.f, 1.f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
//	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    food->RenderModel();

}

bool Food::checkCollision(double x, double y, double r){
    return glm::length(pos - glm::vec2(x,y)) < (r + game_arena_food_radius);
}