#ifndef GAMEVALUES_H
#define GAMEVALUES_H

#pragma once

#include <cmath>

#define game_arena_snake_speed 1.5f
#define game_arena_snake_aspeed M_PI/32.f
#define game_arena_snake_sighting_range 6
#define game_arena_snake_FOV M_PI*2/3

#define game_arena_snake_nb 20
#define game_arena_width 20.f
#define game_arena_height 20.f
#define game_arena_floor_y 0.f

#define game_arena_food_nb 20
#define game_arena_food_radius .25f

#define game_snake_segment_radius .3f
#define game_snake_health_bonus 10.f
#define game_snake_health_decay .02f

#define game_snakenn_nb_layers 3
#define game_snakenn_data_size 24
#define game_snakenn_pool_size 20

#endif