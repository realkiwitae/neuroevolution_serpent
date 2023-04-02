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
    model_segment = model;

    body.clear();
    bspawning = true;
    glm::vec2 p(-12,0);
    body.push_back(Segment());
    body.back().pos = p;

    angle = std::atan2(0.f, 12.f);
    score = 0.f;
    bIsAlive = true;
    health = game_snake_health_bonus*3/2;
    age = 0.f;
}

void Snake::update(){
    if(!bIsAlive){
        if(now - death_clock > game_snake_fading_s){
            init(model_segment);
        }
        return;
    }
    GLfloat velocity = game_arena_snake_speed * deltaTime;
    if(bspawning){
        body[0].pos.x += velocity;
        return;
    }
    age += .1;
    GLfloat slowdown = 49.f/(48.f+body.size());
    GLfloat angle_inc;
    if(bIsAI){
            double inputs[3] = {
        //      fabs(game_arena_ceiling_y - pos.y + collision_h/2.f),
        //        fabs(pos.y - collision_h/2.f - game_arena_floor_y),
                0,
                0,
                0,
                };

            snake_nn->calc(inputs);

            double* out = snake_nn->getOutput();
            angle_inc = (10*game_arena_snake_aspeed / SIGNAL_MULTIPLIER)*(out[1]-out[0]);
            angle_inc = glm::clamp(angle_inc,-(GLfloat)game_arena_snake_aspeed,(GLfloat)game_arena_snake_aspeed);
    }
    else{
        long* keys = mainWindow.getKeys();
        if(CHECK_BIT(*keys,GLFW_KEY_A)){
            angle_inc = (10*game_arena_snake_aspeed / SIGNAL_MULTIPLIER)*(-1);
            angle_inc = glm::clamp(angle_inc,-(GLfloat)game_arena_snake_aspeed,(GLfloat)game_arena_snake_aspeed);

        }
        if(CHECK_BIT(*keys,GLFW_KEY_D)){
            angle_inc = (10*game_arena_snake_aspeed / SIGNAL_MULTIPLIER)*(1);
            angle_inc = glm::clamp(angle_inc,-(GLfloat)game_arena_snake_aspeed,(GLfloat)game_arena_snake_aspeed);

        }
    }

    angle += slowdown * angle_inc;
    if(angle>=2*M_PI)angle -= 2*M_PI;
    if(angle<0.f)angle += 2*M_PI;

    body[0].pos += deltaTime * game_arena_snake_speed * slowdown * glm::vec2(cos(angle), sin(angle)); 
	for(int i = 1; i < body.size();i++){
        GLfloat a = atan2(body[i].pos.y - body[i-1].pos.y,body[i].pos.x - body[i-1].pos.x);
        body[i].pos = body[i-1].pos + 2*game_snake_segment_radius*glm::vec2(cos(a),sin(a));
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

void Snake::render(GLuint uniformModel, GLuint uniformSpecularIntensity, GLuint uniformShininess)
{   
    if(!bIsAlive){
        return;
    }
    glm::mat4 model = glm::mat4(1.0f);
	
    for(Segment s : body){
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(s.pos.x,game_arena_floor_y+game_snake_segment_radius/2.f,s.pos.y));
        model = glm::scale(model, (game_snake_segment_radius/game_arena_food_radius)*glm::vec3(1.f, 1.f, 1.f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    //	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        model_segment->RenderModel();
    }
    model = glm::mat4(1.0f);
    glm::vec2 p(body[0].pos.x+cos(angle),body[0].pos.y+sin(angle));
    model = glm::translate(model, glm::vec3(p.x,game_arena_floor_y+game_snake_segment_radius/2.f,p.y));
    model = glm::scale(model, .3f*(game_snake_segment_radius/game_arena_food_radius)*glm::vec3(1.f, 1.f, 1.f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
//	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    model_segment->RenderModel();
}

void Snake::kill(){
    if(bspawning)return;
    if(!bIsAlive)return;
    death_clock = now;
    bIsAlive = false;
    score /=2.f;
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
    body.emplace_back();
    GLfloat a = angle;
    if(body.size()<2){
        body.back().pos = body[0].pos - 2*game_snake_segment_radius*glm::vec2(cos(a),sin(a));
    }
    else{
        a = atan2(body[body.size()-3].pos.y - body[body.size()-2].pos.y
            ,body[body.size()-3].pos.x - body[body.size()-2].pos.x);
        body.back().pos = body[body.size()-2].pos - 2*game_snake_segment_radius*glm::vec2(cos(a),sin(a));
    }
}

bool Snake::checkCollision(double x, double y, double r){
    return glm::length(body[0].pos - glm::vec2(x,y)) < (r + game_arena_food_radius);
}

bool Snake::selfCollisionCheck(){
    for(int i = 1 ; i < body.size();i++){
        if(checkCollision(body[i].pos.x,body[i].pos.y, game_snake_segment_radius)){
            return true;
        }
    }
    return false;
}

void Snake::endSpawn(){
    if(!bspawning)return;
    bspawning = false;
    std::cout << "Spawn snake OK at " << body[0].pos.x << " "<< body[0].pos.y << std::endl;
}