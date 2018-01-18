#ifndef ENTITY_H
#define ENTITY_H

#include "sprite.h"
#include "message.h"

typedef enum { PLAYER, COMPLEX_IA, SIMPLE_IA, PROP, TILE } entity_type_t;


// CREATION AND DESTRUCTION FUNCTIONS
int entity_create(float x, float y, float z, entity_type_t entity_type);
void entity_destroy(int id);
bool entity_exists(int id);
int entity_get_type(int id);

void entity_set_view(int id, img_t *img, int sheet, int w_spr, int h_spr);
void entity_set_view_variations(int id, int * variations);
void entity_set_view_types(int id, int * types);
void entity_change_colors(int id, int r, int g, int b);
void entity_change_alpha(int id, int a);

void entity_init_animation(int id, int frame, int * anim_cycle, int speed);
void entity_set_solid(int id, bool solid);
void entity_set_size(int id, int w, int h);
void entity_set_height_offset(int id, int ho);
void entity_set_on_collide(int id, char * on_collide);

// SYSTEMS
void entitys_timer();
// Updates all the timers.

void entitys_move();
// Moves all the entitys based on their velocityes. Also sets their animations if necessary.

void entitys_animate();
// Animates all the entitys.

void entitys_render();
// Renders all the entitys in world to the screen.

void entitys_read_messages();

#endif
