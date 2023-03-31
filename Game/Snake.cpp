#include "Snake.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Engine/Utils/CommonValues.h"

#include "./GameValues.h"

#include <iostream>

extern GLfloat deltaTime,now;
extern Camera camera;
extern glm::mat4 projection;
extern Window mainWindow;

Snake::Snake()
{

}

Snake::~Snake()
{

}

void Snake::init(Model* model)
{
    birth = now;
    model_segment = model;
    score = 0.f;
    bIsAlive = true;
    body.push_back(Segment());
}

void Snake::update(){

    if(bIsAI){
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
        //  std::cout << pos.y << std::endl;
        return;
    }

    long* keys = mainWindow.getKeys();
	GLfloat velocity = game_arena_snake_speed * deltaTime;

    if(CHECK_BIT(*keys,GLFW_KEY_W)){
        body[0].pos.x += velocity;
    }
    if(CHECK_BIT(*keys,GLFW_KEY_S)){
        body[0].pos.x -= velocity;
    }
    if(CHECK_BIT(*keys,GLFW_KEY_A)){
        body[0].pos.y -= velocity;
    }
    if(CHECK_BIT(*keys,GLFW_KEY_D)){
        body[0].pos.y += velocity;
    }
}


glm::vec2 Snake::getScreenCoord(){
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(body[0].pos.x,game_arena_floor_y+game_arena_food_radius/2.f,body[0].pos.y));
    model = glm::scale(model, glm::vec3(1.f, 1.f, 1.f));
    glm::vec4 clip_coordinates = projection * camera.calculateViewMatrix() * model * glm::vec4(glm::vec3(body[0].pos.x,game_arena_floor_y+game_arena_food_radius/2.f,body[0].pos.y), 1.0f);
    glm::vec3 ndc_coordinates = glm::vec3(clip_coordinates) / clip_coordinates.w;
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glm::vec2 screen_coordinates = glm::vec2(
        (ndc_coordinates.x + 1.0f) / 2.0f * viewport[2] + viewport[0],
        (ndc_coordinates.y + 1.0f) / 2.0f * viewport[3] + viewport[1]
    );
    return screen_coordinates;
}

void Snake::update(glm::vec2 change){
    if(std::isnan(change.x) || std::isnan(change.y))return;
    glm::vec2 n_change = glm::normalize(change);
    float dx = n_change.x*game_arena_snake_speed*deltaTime;
    float dy = n_change.y*game_arena_snake_speed*deltaTime;
    std::cout << deltaTime<< " "<< dx << " " << dy << " " << body[0].pos.x << " " << body[0].pos.y <<std::endl;

    //body[0].pos.x -= dy;
    // body[0].pos.y += dy;
    


}

void Snake::render(GLuint uniformModel, GLuint uniformSpecularIntensity, GLuint uniformShininess)
{   
    if(!bIsAlive){
        return;
    }
    glm::mat4 model = glm::mat4(1.0f);
	
    for(Segment s : body){
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(s.pos.x,game_arena_floor_y+game_arena_food_radius/2.f,s.pos.y));
        model = glm::scale(model, glm::vec3(1.f, 1.f, 1.f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    //	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        model_segment->RenderModel();
    }

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