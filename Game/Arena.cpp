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
    if(!dirtTexture){
        dirtTexture = new Texture("Textures/dirt.png");
        dirtTexture->LoadTextureA();
    }
    if(!food){
        food = new Model();
        food->LoadModel("snake/food");
    }

    for(size_t i = 0; i < game_arena_food_nb; i++){
        foods[i] = Food(i);
        foods[i].init(food);
    }

    if(!ground){
        unsigned int indices[] = {		
            0, 3, 1,
            1, 3, 2,
            2, 3, 0,
            0, 1, 2
        };

        GLfloat vertices[] = {
        //	x      y      z			u	  v			nx	  ny    nz
            -1.0f, -1.0f, -0.6f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
            0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
            1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
        };

        unsigned int floorIndices[] = {
            0, 2, 1,
            1, 2, 3
        };

        GLfloat floorVertices[] = {
            -10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
            10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
            -10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
            10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
        };

        calcAverageNormals(indices, 12, vertices, 32, 8, 5);

        ground = new Mesh();
        ground->CreateMesh(floorVertices, floorIndices, 32, 6);

    }

    if(!wall){
        wall = new Model();
        wall->LoadModel("snake/wall");
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
	model = glm::translate(model, glm::vec3(0.0f, game_arena_floor_y, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dirtTexture->UseTexture();
	ground->RenderMesh();

    model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(11.f, game_arena_floor_y, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 12.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	wall->RenderModel();
    model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-11.f, game_arena_floor_y, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 12.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	wall->RenderModel();

    model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.f, game_arena_floor_y, -11.f));
	model = glm::scale(model, glm::vec3(12.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	wall->RenderModel();
    model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.f, game_arena_floor_y, 11.f));
	model = glm::scale(model, glm::vec3(12.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	wall->RenderModel();

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