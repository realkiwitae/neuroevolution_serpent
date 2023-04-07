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
        bool b = true;
        while(b){
            b = false;
            for(size_t j = 0; j < i; j++){
                if(foods[i].checkCollision(foods[j].getPos().x,foods[j].getPos().y,3*game_arena_food_radius)){
                    b = true;
                    foods[i].init(food);
                    break;
                }
            }     
        }
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
            -12.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
            11.0f, 0.0f, -10.0f,	12.0f, 0.0f,	0.0f, -1.0f, 0.0f,
            -12.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
            10.0f, 0.0f, 10.0f,		12.0f, 10.0f,	0.0f, -1.0f, 0.0f
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
    for(size_t i = 0; i < game_arena_food_nb; i++){
        if(!foods[i].collected)continue;
        
        foods[i] = Food(i);
        foods[i].init(food);
        bool b = true;
        while(b){
            b = false;
            for(size_t j = 0; j < game_arena_food_nb; j++){
                if(foods[j].id == foods[i].id)continue;
                if(foods[i].checkCollision(foods[j].getPos().x,foods[j].getPos().y,3*game_arena_food_radius)){
                    b = true;
                    foods[i].init(food);
                    break;
                }
            }     
        }
    }
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
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 10.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	wall->RenderModel();
    model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-11.f, game_arena_floor_y, -6.f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f,4.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	wall->RenderModel();
    model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-11.f, game_arena_floor_y, 6.f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f,4.0f));
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

    // model = glm::mat4(1.0f);
	// model = glm::translate(model, glm::vec3(0.f, game_arena_floor_y, 0.f));
	// model = glm::scale(model, glm::vec3(.5f, .5f, .5f));
	// glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	// wall->RenderModel();
    // foods[0].setPos(0,0);
    // foods[0].render(uniformModel, uniformSpecularIntensity, uniformShininess);
    for(size_t i = 0; i < game_arena_food_nb; i++){
        foods[i].render(uniformModel, uniformSpecularIntensity, uniformShininess);
    }
}

struct s_thing{
    double distance;
    int type;
};

void Arena::updateInfo(Snake* s){
    // if(s->isDead())return;
    // glm::vec2 snake_pos = s->getPos();

    // for(Food& p : foods){
    //     p.checkCollision(s);
    // }
    if(fabs(s->getPos().x) > game_arena_width/2 - game_snake_segment_radius){
        s->kill();
        return;
    }
    if(fabs(s->getPos().y) > game_arena_height/2 - game_snake_segment_radius){
        s->kill();
        return;
    }

    s->endSpawn();

    if(s->selfCollisionCheck()){
        s->kill();
        return;
    }

    for(Food& f : foods){
        if(f.checkCollision(s->getPos().x,s->getPos().y,game_snake_segment_radius)){
            s->addScore(1.f);
        }
    }
    // Input feed
   /*
   For each FOV division add input for each type distance
   */ 
    for(int i = 0; i < game_arena_snake_FOV_DIVISION*3;i++){
        s->inputs[i] = 0.;
    }


    s_thing input[2*game_arena_snake_FOV_DIVISION];
    //init fogofwar
    for(int i = 0; i < 2*game_arena_snake_FOV_DIVISION; i++){
        input[i].distance = 9999.;
        input[i].type = 9999;
    }
    //food
    for (auto f : foods) {
        double a = std::atan2(f.getPos().y - s->getPos().y , f.getPos().x - s->getPos().x);
        a = std::fmod(a - s->angle, 2*M_PI);
        double d = glm::distance(s->getPos(),f.getPos()) - game_arena_food_radius - game_snake_segment_radius;
        if (a >= 0 && a < game_arena_snake_FOV/2) {
            if (d < input[(int) (a * game_arena_snake_FOV_DIVISION / (game_arena_snake_FOV/2))].distance) {
                input[(int) (a * game_arena_snake_FOV_DIVISION / (game_arena_snake_FOV/2))].distance = d;
                input[(int) (a * game_arena_snake_FOV_DIVISION / (game_arena_snake_FOV/2))].type = E_TYPE_FOOD;
            }
        } else if (a <= 0 && -a < game_arena_snake_FOV/2) {
            if (d < input[(int) (-a * game_arena_snake_FOV_DIVISION / (game_arena_snake_FOV/2)) + game_arena_snake_FOV_DIVISION].distance) {
                input[(int) (-a * game_arena_snake_FOV_DIVISION / (game_arena_snake_FOV/2)) + game_arena_snake_FOV_DIVISION].distance = d;
                input[(int) (-a * game_arena_snake_FOV_DIVISION / (game_arena_snake_FOV/2)) + game_arena_snake_FOV_DIVISION].type = E_TYPE_FOOD;
            }
        }
    } 
    // Body
    auto b = s->getBody();
    for (int i = 1 ; i < b.size();i++) {
        double a = std::atan2(b[i].pos.y - s->getPos().y , b[i].pos.x - s->getPos().x);
        a = std::fmod(a - s->angle, 2*M_PI);
        double d = glm::distance(s->getPos(),b[i].pos) - 2*game_snake_segment_radius;
        if (a >= 0 && a < game_arena_snake_FOV/2) {
            if (d < input[(int) (a * game_arena_snake_FOV_DIVISION / (game_arena_snake_FOV/2))].distance) {
                input[(int) (a * game_arena_snake_FOV_DIVISION / (game_arena_snake_FOV/2))].distance = d;
                input[(int) (a * game_arena_snake_FOV_DIVISION / (game_arena_snake_FOV/2))].type = E_TYPE_SNAKE;
            }
        } else if (a <= 0 && -a < game_arena_snake_FOV/2) {
            if (d < input[(int) (-a * game_arena_snake_FOV_DIVISION / (game_arena_snake_FOV/2)) + game_arena_snake_FOV_DIVISION].distance) {
                input[(int) (-a * game_arena_snake_FOV_DIVISION / (game_arena_snake_FOV/2)) + game_arena_snake_FOV_DIVISION].distance = d;
                input[(int) (-a * game_arena_snake_FOV_DIVISION / (game_arena_snake_FOV/2)) + game_arena_snake_FOV_DIVISION].type = E_TYPE_SNAKE;
            }
        }
    }
    // Wall
    double section_size = game_arena_snake_FOV / (game_arena_snake_FOV_DIVISION*2);
        // Calculate distances to walls for each section
    for (int i = 0; i <  (game_arena_snake_FOV_DIVISION*2); i++) {
        // Calculate section start and end angles
        double start_angle = (s->angle - (game_arena_snake_FOV/2.0)) + i*section_size;
        double end_angle = start_angle + section_size;
        double a = (start_angle + end_angle)/2;
             
        // Calculate distances to walls for start and end angles
        double dist_to_left_wall = INFINITY;
        double dist_to_right_wall = INFINITY;
        double dist_to_top_wall = INFINITY;
        double dist_to_bottom_wall = INFINITY;
        
        if (start_angle <= M_PI_2 && end_angle >= 0.0) {
            dist_to_left_wall = fabs(s->getPos().x / cos(start_angle));
        }
        
        if (start_angle <= M_PI && end_angle >= M_PI_2) {
            dist_to_top_wall = fabs(s->getPos().y / sin(start_angle));
        }
        
        if (start_angle <= 3.0*M_PI_2 && end_angle >= M_PI) {
            dist_to_right_wall = fabs((game_arena_width - s->getPos().x) / cos(end_angle));
        }
        
        if (start_angle <= 2.0*M_PI && end_angle >= 3.0*M_PI_2) {
            dist_to_bottom_wall = fabs((game_arena_height - s->getPos().y) / sin(end_angle));
        }
        
        // Find minimum distance to a wall
        double min_dist_to_wall = std::min({dist_to_left_wall, dist_to_right_wall, dist_to_top_wall, dist_to_bottom_wall});
        
        // Output result



        std::cout << "Section "
                << i+1 << " _ " << (int) (fabs(a) * game_arena_snake_FOV_DIVISION / (game_arena_snake_FOV/2)) + (a<0)*game_arena_snake_FOV_DIVISION <<": " << start_angle*180/M_PI << " " << end_angle*180/M_PI
                <<" "<< min_dist_to_wall << std::endl;

        if (a >= 0 && a < game_arena_snake_FOV/2) {
            if (min_dist_to_wall < input[(int) (a * game_arena_snake_FOV_DIVISION / (game_arena_snake_FOV/2))].distance) {
                input[(int) (a * game_arena_snake_FOV_DIVISION / (game_arena_snake_FOV/2))].distance = min_dist_to_wall;
                input[(int) (a * game_arena_snake_FOV_DIVISION / (game_arena_snake_FOV/2))].type = E_TYPE_SNAKE;
            }
        } else if (a <= 0 && -a < game_arena_snake_FOV/2) {
            if (min_dist_to_wall < input[(int) (-a * game_arena_snake_FOV_DIVISION / (game_arena_snake_FOV/2)) + game_arena_snake_FOV_DIVISION].distance) {
                input[(int) (-a * game_arena_snake_FOV_DIVISION / (game_arena_snake_FOV/2)) + game_arena_snake_FOV_DIVISION].distance = min_dist_to_wall;
                input[(int) (-a * game_arena_snake_FOV_DIVISION / (game_arena_snake_FOV/2)) + game_arena_snake_FOV_DIVISION].type = E_TYPE_SNAKE;
            }
        }


    }
    
    for (int i = 0; i < game_arena_snake_FOV_DIVISION; i++) {
        s->inputs[input[i].type * game_arena_snake_FOV_DIVISION * 2 + i] = SIGNAL_MULTIPLIER * (game_arena_snake_sighting_range - input[i].distance) / game_arena_snake_sighting_range;
        s->inputs[input[i + game_arena_snake_FOV_DIVISION].type * game_arena_snake_FOV_DIVISION * 2 + game_arena_snake_FOV_DIVISION * 2 - 1 - i] = SIGNAL_MULTIPLIER
                * (game_arena_snake_sighting_range - input[i + game_arena_snake_FOV_DIVISION].distance) / game_arena_snake_sighting_range;
    }
}