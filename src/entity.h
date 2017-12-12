#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>
#include <assert.h>
#include "sprite.h"

typedef enum { UPDIR, DOWNDIR, LEFTDIR, RIGHTDIR } directions_t;
typedef enum { FORWARD, LEFT, RIGHT, BACKWARD } anim_t;
typedef enum { PLAYER, COMPLEX_IA, SIMPLE_IA, PROP, TILE } entity_type_t;

typedef enum {
    COMPONENT_NONE = 0,
    COMPONENT_POSITION = 1 << 0,
    COMPONENT_SPEED = 1 << 1,
    COMPONENT_VIEW = 1 << 2,
    COMPONENT_ANIMATION = 1 << 3,
    COMPONENT_PHYSICS = 1 << 4,
    COMPONENT_TIMER = 1 << 5
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
} component_speed_t;

typedef struct {
    spr_t *spr;
} component_view_t;

typedef struct {
    int timerId;
    int frame;
    int anim_cycle[4];
    int speed;
} component_animation_t;

typedef struct {
    bool solid;
    uint16_t w;
    uint16_t h;
} component_physics_t;

typedef struct {
    long counts[12];
    long timers[12];
    int (*callbacks[12])(int);
} component_timer_t;

typedef struct {
    long mask [MAX_ENTITYS];
    component_position_t position[MAX_ENTITYS];
    component_speed_t speed[MAX_ENTITYS];
    component_view_t view[MAX_ENTITYS];
    component_animation_t animation[MAX_ENTITYS];
    component_physics_t physics[MAX_ENTITYS];
    component_timer_t timer[MAX_ENTITYS];
} world_t;

extern world_t * world;


// CREATION AND DESTRUCTION FUNCTIONS
void world_create(void);
// Creates the world component and populates it with chunks.

int entity_get_empty_id(entity_type_t entity_type);
// Returns the first empty id.
//     entity_type is one of { PLAYER, COMPLEX_IA, SIMPLE_IA, PROP, TILE }, for convenience.
// Returns the first empty id in world.

void entity_mark_destroyed(int id);
// Marks the id as unused.
//     id is the id of the entity to destroy.

int entity_create(float x, float y, float z, entity_type_t entity_type);
// Creates an entity
//     x and y are the initial position.
//     z is the initial height (for rendering on top or below other entityes).
//     entity_type is one of { PLAYER, COMPLEX_IA, SIMPLE_IA, PROP, TILE }, for convenience.
// Returns the id of the entity created.

void entity_destroy(int id);
// Destroy the entity and free some memory.
//     id is the id of the entity to destroy.


// POSITION FUNCTIONS
void entity_set_x(int id, float x);
// Set the entity x to the specified x.
//     id is the id of the entity to move.
//     x is the new x.

void entity_set_y(int id, float y);
// Set the entity y to the specified y.
//     id is the id of the entity to move.
//     y is the new y.

void entity_set_z(int id, float z);
// Set the entity z to the specified z.
//     id is the id of the entity to move.
//     z is the new z.


// SPEED FUNCTIONS
void entity_set_speed(int id, directions_t dir, float qty);
// Sets the entity speed to a certain direction.
// If the entity was already moving in the same axis (same direction or opposite), new persists and older is overwritten.
//     id is the id of the entity to move.
//     dir is one of { UPDIR, DOWNDIR, RIGHTDIR, LEFTDIR }.
//     qty is the new speed (always positive, and please use entity_stop_speed() for stopping, instead of a qty of 0).

void entity_stop_speed(int id, directions_t dir);
// Stops the entity from moving in a certain direction.
//     id is the id of the entity to stop.
//     dir is one of { UPDIR, DOWNDIR, RIGHTDIR, LEFTDIR }.


// VIEW FUNCTIONS
void entity_set_view(int id, img_t *img, int sheet);
// Sets an image to an entity.
//     id is the id of the entity to assign the view.
//     img is the image file where the sprite for the entity is.
//     sheet is the sheet of sprite from that image file.

void entity_destroy_view(int id);
// Releases the memory that sprite occupies.
//     id is the id of the entity to destroy its view.


// ANIMATION FUNCTIONS
void entity_init_animation(int id, int frame, int * anim_cycle, int speed);
// Inits the animation component.
//     id is the id of the entity to animate.
//     frame is the initial frame.
//     anim_cycle is the animation cycle (like {0, 1, 2, 1}).
//     speed is the speed of the animation.

void entity_set_animation(int id, anim_t animation);
// Starts the animation of the sprite.
//     id is the id of the entity to animate.
//     animation is one of {FORWARD, LEFT, RIGHT, BACKWARD}.

void entity_freeze_animation(int id);
// Stops the animation of the sprite and sets its frame to the current frame.
//     id is the id of the entity to freeze.

void entity_stop_animation_at(int id, int frame);
// Stops the animation of the sprite and sets its frame to the specified one.
//     id is the id of the entity to un-animate.
//     the frame where to stop the animation.

void entity_stop_animation(int id);
// Stops the animation of the sprite and sets its frame to the base frame.
//     id is the id of the entity to un-animate.


// PHYSICS FUNCTIONS
void entity_set_solid(int id, bool solid);
// Sets wheter the entity is solid.
//     id is the id of the entity to set or clear.
//     solid is whether the entity blocks movement or not

void entity_set_size(int id, int w, int h);
// Sets the entity's real size (for the collisions).
//     id is the id of the entity to which to set the size.
//     w and h are the real size.


// TIMER FUNCTIONS
int entity_set_timer(int id, long time, void (*callback)(int));
// Sets a timer. Calls a callback function when timer ends.
//     id is the id of the entity to set the timer to.
//     callback is the callback function that the timer will call.
// Returns the id of the timer.

long entity_read_timer(int id, int timerId);
// Reads the value of the timer without stopping it.
//     id is the id of the entity to read the timer to.
//     timerId is the id of the timer to read.
// Returns the time ellapsed from the set of the timer.

long entity_cancel_timer(int id, int timerId);
// Cancels a timer.
//     id is the id of the entity to cancel the timer to.
//     timerId is the id of the timer to cancel.
// Returns the time ellapsed from the set of the timer.


// SYSTEMS
void entitys_timer();
// Updates all the timers.

void entitys_move();
// Moves all the entitys based on their velocityes. Also sets their animations if necessary.

void entitys_animate();
// Animates all the entitys.

void entitys_render();
// Renders all the entitys in world to the screen.

#endif
