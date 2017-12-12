#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>
#include <assert.h>
#include "sprite.h"

typedef enum { UPDIR, DOWNDIR, LEFTDIR, RIGHTDIR } directions_t;
typedef enum { PLAYER, COMPLEX_IA, SIMPLE_IA, PROP, TILE } entity_type_t;

typedef enum {
    COMPONENT_NONE = 0,
    COMPONENT_POSITION = 1 << 0,
    COMPONENT_VELOCITY = 1 << 1,
    COMPONENT_VIEW = 1 << 2,
    COMPONENT_PHYSICS = 1 << 3
} component_t;

typedef struct {
    float x;
    float y;
    float z;
} component_position_t;

typedef struct {
    float x;
    float y;
    int chunkId;
} component_velocity_t;

typedef struct {
    spr_t *spr;
} component_view_t;

typedef struct {
    bool solid;
    uint16_t w;
    uint16_t h;
} component_physics_t;

typedef struct {
    uint8_t tile;
    uint8_t variation;
} component_tile_h;

typedef struct {
    int mask [MAX_ENTITYS];
    component_position_t position[MAX_ENTITYS];
    component_velocity_t velocity[MAX_ENTITYS];
    component_view_t view[MAX_ENTITYS];
    component_physics_t physics[MAX_ENTITYS];
} world_t;

// CREATION AND DESTRUCTION FUNCTIONS
int entity_get_empty_id(world_t *world, entity_type_t entity_type); // Returns the first empty id.
// entity_type is one of { PLAYER, COMPLEX_IA, SIMPLE_IA, PROP, TILE }, for convenience.

void entity_mark_destroyed(world_t *world, int id); // Marks the id as unused.
// entity is the id of the entity to destroy.

int entity_create(world_t *world, float x, float y, float z, entity_type_t entity_type); // Creates an entity
// x and y are the initial position.
// z is the initial height (for rendering on top or below other entityes).
// entity_type is one of { PLAYER, COMPLEX_IA, SIMPLE_IA, PROP, TILE }, for convenience.

void entity_destroy(world_t *world, int id);// Destroy the entity and free some memory.

world_t * world_create(void); // Creates the world component and populates it with chunks.

// POSITION FUNCTIONS
void entity_set_position(world_t *world, int id, float x, float y, float z); // Set the entity position to the specified coords.
// Use NULL in x, y or z to leave it equal.

// VELOCITY FUNCTIONS
void entity_set_velocity(world_t *world, int id, directions_t dir, float qty); // Sets the entity velocity to a certain direction.
// If the entity was already moving in the same axis (same direction or opposite), new persists and older is overwritten.
// id is the entity id to move.
// dir is one of { UPDIR, DOWNDIR, RIGHTDIR, LEFTDIR }.
// qty is the new velocity (always positive, and please use entity_stop_velocity() for stopping, instead of a qty of 0).

void entity_stop_velocity(world_t *world, int id, directions_t dir); // Stops the entity from moving in a certain direction.
// id is the entity id to stop.
// dir is one of { UPDIR, DOWNDIR, RIGHTDIR, LEFTDIR }.

// VIEW FUNCTIONS
void entity_set_view(world_t *world, int id, img_t *img, uint8_t type); // Sets an image to an entity
// img is the image file where the sprite for the entity is.
// type is the type of sprite from that image file.

void entity_set_view_tile(world_t *world, int id, img_t *img, uint8_t terrain, uint8_t variation); // Sets an image to tile
// terrain is the type of terrain from that image file.
// variation is one of the variations of that terrain.

void entity_destroy_view(world_t *world, int id); // Free the sprite.

// PHYSICS FUNCTIONS
void entity_set_solid(world_t *world, int id, bool solid); // Sets wheter the entity is solid

void entity_set_size(world_t *world, int id, uint8_t w, uint8_t h); // Sets the entity's width and height

// SYSTEMS
void entitys_move(world_t *world); // Moves all the entitys based on their velocityes. Also animates them if necessary.

void entitys_render(world_t *world, screen_t *screen); // Renders all the entitys in world to the screen.

#endif
