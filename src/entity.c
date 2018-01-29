#include <stdbool.h>
#include <assert.h>
#include "entity.h"

#define MAX_ANIM_CYCLE 4
#define DAMAGE_TYPES 12
#define MAX_TIMERS 12

typedef enum { FORWARD, LEFT, RIGHT, BACKWARD, IDDLE } anim_type_t;

typedef enum {
    COMPONENT_NONE = 0,
    COMPONENT_TYPE = 1 << 0,
    COMPONENT_POSITION = 1 << 1,
    COMPONENT_SPEED = 1 << 2,
    COMPONENT_VIEW = 1 << 3,
    COMPONENT_ANIMATION = 1 << 4,
    COMPONENT_PHYSICS = 1 << 5,
    COMPONENT_HEALTH = 1 << 6,
    COMPONENT_TIMER = 1 << 7,
} component_t;

typedef struct {
    entity_type_t type;
} component_type_t;

typedef struct {
    float x;
    float y;
    float z;
} component_position_t;

typedef struct {
    float x;
    float y;
} component_speed_t;

typedef struct {
    spr_t *spr;
    bool animated_on_move;
} component_view_t;

typedef struct {
    anim_type_t type;
    int timerId;
    int frame;
    int speed;
    int anim_cycle[MAX_ANIM_CYCLE];
} component_animation_t;

typedef struct {
    bool solid;
    int w;
    int h;
    int w_off;
    int h_off;
    message_types_t on_collide_action;
    char on_collide_props [PAYLOAD_SIZE];
} component_physics_t;

typedef struct {
    bool immune;
    float health;
    float resistences [DAMAGE_TYPES];
} component_health_t;

typedef struct {
    // TODO: identifiers
    long counts[MAX_TIMERS];
    long timers[MAX_TIMERS];
    void (*callbacks[MAX_TIMERS])(int);
} component_timer_t;

typedef struct {
    long mask [MAX_ENTITYS];
    component_type_t type[MAX_ENTITYS];
    component_position_t position[MAX_ENTITYS];
    component_speed_t speed[MAX_ENTITYS];
    component_view_t view[MAX_ENTITYS];
    component_animation_t animation[MAX_ENTITYS];
    component_physics_t physics[MAX_ENTITYS];
    component_health_t health[MAX_ENTITYS];
    component_timer_t timer[MAX_ENTITYS];
} world_t;

static world_t world;

// CREATION AND DESTRUCTION FUNCTIONS
static int entity_get_empty_id();
static void entity_mark_destroyed(int id);

// POSITION FUNCTIONS
typedef enum {X, Y, Z} axis_t;
static void entity_set_pos(int id, axis_t axis, float value);
static float entity_get_pos(int id, axis_t axis);

// SPEED FUNCTIONS
typedef enum { UPDIR, DOWNDIR, LEFTDIR, RIGHTDIR } directions_t;
static void entity_set_speed(int id, directions_t dir, float qty);
static void entity_stop_speed(int id, directions_t dir);

// VIEW FUNCTIONS
static void entity_update_view_variation(int id, int variationId, int variation);
static void entity_update_view_type(int id, int typeId, int type);
static void entity_destroy_view(int id);



// CREATION AND DESTRUCTION FUNCTIONS
static int entity_get_empty_id() {
    int id = 0;
    bool found = false;
    while (!found && id < MAX_ENTITYS) {
        if (world.mask[id] == COMPONENT_NONE) {
            found = true;
        } else id++;
    }

    if (id >= MAX_ENTITYS) {
        return -1;
    } else {
        return id;
    }
}

static void entity_mark_destroyed(int id) {
    world.mask[id] = COMPONENT_NONE;
}

bool entity_exists(int id) {
    return (world.mask[id] != COMPONENT_NONE);
}

int entity_get_type(int id) {
    return world.type[id].type;
}

int entity_create(float x, float y, float z, entity_type_t entity_type) {
    int id = entity_get_empty_id();
    if (id == -1) {
        fprintf(stderr, "ERROR: Too much entities.");
        return -1;
    }

    world.mask[id] = COMPONENT_POSITION;
    world.type[id].type = entity_type;
    world.position[id].x = x;
    world.position[id].y = y;
    world.position[id].z = z;

    return id;
}

void entity_destroy(int id) {
    entity_destroy_view(id);
    entity_mark_destroyed(id);
}


// POSITION FUNCTIONS
static void entity_set_pos(int id, axis_t axis, float value) {
    component_position_t *pos = &(world.position[id]);
    switch (axis) {
        case X: {
            pos->x = value;
            break;
        }
        case Y: {
            pos->y = value;
            break;
        }
        case Z: {
            pos->z = value;
            break;
        }
    }
}

static float entity_get_pos(int id, axis_t axis) {
    component_position_t *pos = &(world.position[id]);
    float value;
    switch (axis) {
        case X: {
            value = pos->x ;
            break;
        }
        case Y: {
            value = pos->y;
            break;
        }
        case Z: {
            value = pos->z;
            break;
        }
    }

    return value;
}


// SPEED FUNCTIONS
static void entity_set_speed(int id, directions_t dir, float qty) {
    component_speed_t *speed = &(world.speed[id]);
    switch (dir) {
        case UPDIR: {
            speed->y = -qty;
            break;
        }
        case DOWNDIR: {
            speed->y = qty;
            break;
        }
        case LEFTDIR: {
            speed->x = -qty;
            break;
        }
        case RIGHTDIR: {
            speed->x = qty;
            break;
        }
    }
    world.mask[id] |= COMPONENT_SPEED;
}

static void entity_stop_speed(int id, directions_t dir) {
    if ((world.mask[id] & COMPONENT_SPEED) == COMPONENT_SPEED) {
        component_speed_t *speed = &(world.speed[id]);
        // Stop the required axis speed if active
        switch (dir) {
            case UPDIR: {
                if (speed->y < 0) speed->y = 0;
                break;
            }
            case DOWNDIR: {
                if (speed->y > 0) speed->y = 0;
                break;
            }
            case LEFTDIR: {
                if (speed->x < 0) speed->x = 0;
                break;
            }
            case RIGHTDIR: {
                if (speed->x > 0) speed->x = 0;
                break;
            }
        }

        // If all speeds 0, mark the component as unused
        if (speed->x == 0 && speed->y == 0) {
            world.mask[id] &= ~COMPONENT_SPEED;
        }
    }
}



// VIEW FUNCTIONS
void entity_set_view(int id, img_t *img, int sheet, int w_spr, int h_spr) {
    component_view_t *view = &(world.view[id]);
    int types[25] = {0};
    int variations[25] = {0};
    view->spr = sprite_create(img, w_spr, h_spr, sheet, types, variations, 0, 0, WHITE);
    view->animated_on_move = false;

    world.mask[id] |= COMPONENT_VIEW;
}

void entity_set_view_variations(int id, int * variations) {
    if ((world.mask[id] & COMPONENT_VIEW) == COMPONENT_VIEW) {
        component_view_t *view = &(world.view[id]);
        sprite_change_variations(view->spr, variations);
    }
}

static void entity_update_view_variation(int id, int variationId, int variation) {
    if ((world.mask[id] & COMPONENT_VIEW) == COMPONENT_VIEW) {
        component_view_t *view = &(world.view[id]);
        sprite_update_variation(view->spr, variationId, variation);
    }
}

void entity_set_view_types(int id, int * types) {
    if ((world.mask[id] & COMPONENT_VIEW) == COMPONENT_VIEW) {
        component_view_t *view = &(world.view[id]);
        sprite_change_types(view->spr, types);
    }
}

static void entity_update_view_type(int id, int typeId, int type) {
    if ((world.mask[id] & COMPONENT_VIEW) == COMPONENT_VIEW) {
        component_view_t *view = &(world.view[id]);
        sprite_update_type(view->spr, typeId, type);
    }
}

void entity_change_colors(int id, int r, int g, int b) {
    if ((world.mask[id] & COMPONENT_VIEW) == COMPONENT_VIEW) {
        component_view_t *view = &(world.view[id]);
        sprite_change_color(view->spr, r, g, b, -1);
    }
}

void entity_change_alpha(int id, int a) {
    if ((world.mask[id] & COMPONENT_VIEW) == COMPONENT_VIEW) {
        component_view_t *view = &(world.view[id]);
        sprite_change_color(view->spr, -1, -1, -1, a);
    }
}

static void entity_destroy_view(int id) {
    if ((world.mask[id] & COMPONENT_VIEW) == COMPONENT_VIEW) {
        component_view_t *view = &(world.view[id]);
        sprite_destroy(view->spr);
        world.mask[id] &= ~COMPONENT_VIEW;
    }
}



// ANIMATION FUNCTIONS
void entity_init_animation(int id, int frame, int * anim_cycle, int speed, bool animated_on_move) {
    component_animation_t *animation = &(world.animation[id]);
    component_view_t *view = &(world.view[id]);

    animation->speed = 40 - speed;
    animation->frame = frame;
    memcpy(animation->anim_cycle, anim_cycle, sizeof(int)*4);
    animation->timerId = -1;
    view->animated_on_move = animated_on_move;

    world.mask[id] |= COMPONENT_ANIMATION;
}

static void entity_set_animation(int id, anim_type_t anim_type) {
    if ((world.mask[id] & COMPONENT_VIEW) == COMPONENT_VIEW) {
        component_view_t *view = &(world.view[id]);
        component_animation_t *animation = &(world.animation[id]);

        if (world.type[id].type != TILE) {
            int i, types[25];
            for (i = 0; i < 25; i++) {
                 types[i] = anim_type;
            }
            sprite_change_types(view->spr, &types[0]);
        }

        animation->type = anim_type;
    }

    world.mask[id] |= COMPONENT_ANIMATION;
}

static void entity_freeze_animation(int id) {
    assert(world.mask[id] != COMPONENT_NONE);
    component_animation_t *animation = &(world.animation[id]);
    if ((world.mask[id] & COMPONENT_ANIMATION) == COMPONENT_ANIMATION) {
        if (animation->timerId != -1) entity_cancel_timer(id, animation->timerId);

        world.mask[id] &= ~COMPONENT_ANIMATION;
    }
}

static void entity_stop_animation_at(int id, int frame) {
    entity_freeze_animation(id);
    assert(world.mask[id] != COMPONENT_NONE);
    component_animation_t *animation = &(world.animation[id]);
    component_view_t *view = &(world.view[id]);

    animation->frame = frame % 4;

    if (world.type[id].type != TILE) {
        int i, vars[25];
        for (i = 0; i < (view->spr->w_spr * view->spr->h_spr); i++) {
            vars[i] = animation->anim_cycle[animation->frame];
        }
        sprite_change_variations(view->spr, &vars[0]);
    } else {
        sprite_change_sheet(view->spr, animation->anim_cycle[animation->frame]);
    }
}

static void entity_stop_animation(int id) {
    entity_stop_animation_at(id, 1);
}



// TIMER FUNCTIONS
static int entity_set_timer(int id, long time, void (*callback)(int)) {
    assert(world.mask[id] != COMPONENT_NONE);
    component_timer_t *timer = &(world.timer[id]);
    int i;
    for (i = 0; i < 12; i++) {
        if (timer->timers[i] == 0) {
            timer->counts[i] = 0;
            timer->timers[i] = time;
            timer->callbacks[i] = callback;
            break;
        }
    }

    world.mask[id] |= COMPONENT_TIMER;
    return i;
}

static long entity_read_timer(int id, int timerId) {
    assert(world.mask[id] != COMPONENT_NONE);
    component_timer_t *timer = &(world.timer[id]);

    return timer->counts[timerId];
}

static long entity_cancel_timer(int id, int timerId) {
    assert(world.mask[id] != COMPONENT_NONE);
    component_timer_t *timer = &(world.timer[id]);
    long c = timer->counts[timerId];
    timer->counts[timerId] = 0;
    timer->timers[timerId] = 0;
    timer->callbacks[timerId] = NULL;

    int i;
    for (i = 0; i < 12; i++) {
        if (timer->timers[i] != 0) {
            return c;
        }
    }

    world.mask[id] &= ~COMPONENT_TIMER;
    return c;
}



// PHYSICS FUNCTIONS
void entity_set_solid(int id, bool solid) {
    assert(world.mask[id] != COMPONENT_NONE);
    component_physics_t *physics = &(world.physics[id]);
    physics->solid = solid;

    world.mask[id] |= COMPONENT_PHYSICS;
}

void entity_set_size(int id, int w, int h) {
    assert(world.mask[id] != COMPONENT_NONE);
    component_physics_t *physics = &(world.physics[id]);
    physics->w = w;
    physics->h = h;

    world.mask[id] |= COMPONENT_PHYSICS;
}

void entity_set_height_offset(int id, int h_off) {
    assert(world.mask[id] != COMPONENT_NONE);
    component_physics_t *physics = &(world.physics[id]);
    physics->h_off = h_off;

    world.mask[id] |= COMPONENT_PHYSICS;
}

void entity_set_on_collide(int id, message_types_t on_collide_action, char * on_collide_props) {
    assert(world.mask[id] != COMPONENT_NONE);
    component_physics_t *physics = &(world.physics[id]);
    physics->on_collide_action = on_collide_action;
    strcpy(physics->on_collide_props, on_collide_props);
}



// HEALTH FUNCTIONS
typedef enum {ACID, BLUDGEONING, COLD, FIRE, FORCE, LIGHTNING, NECROTIC, PIERCING, POISON, PSYCHIC, RADIANT, SLASHING, THUNDER} damage_types_t;

void entity_set_health (int id, int ammount, float * resistences) {
    component_health_t *health = &(world.health[id]);
    health->health = ammount;
    memcpy(health->resistences, resistences, sizeof(float)*12);
    world.mask[id] |= COMPONENT_HEALTH;
}

static void entity_make_immune (int id, bool value) {
    component_health_t *health = &(world.health[id]);
    health->immune = value;
}

static void go_back (int id) {
    entity_change_colors(id, 255, 255, 255);
    entity_make_immune (id, true);
}

static void entity_take_damage (int id, damage_types_t type, int ammount) {
    component_health_t *health = &(world.health[id]);
    entity_change_colors(id, 239, 154, 154);
    entity_make_immune (id, true);
    entity_set_timer(id, 200, go_back);
    health->health -= health->resistences[type] * ammount;
}


// SYSTEMS
void entitys_timer() {
    component_timer_t *timer;

    // Set all the entity with timer component  ids to an array
    for (int id = 0; id < MAX_ENTITYS; id++) {
        if ((world.mask[id] & COMPONENT_TIMER) == COMPONENT_TIMER) {
            timer = &(world.timer[id]);
            int i;
            for (i = 0; i < 12; i++) {
                if (timer->timers[i] != 0) {
                    timer->counts[i] += MS_PER_UPDATE;

                    if (timer->timers[i] != -1 && timer->counts[i] >= timer->timers[i]) {
                        timer->counts[i] = 0;
                        timer->timers[i] = 0;
                        timer->callbacks[i](id);
                        timer->callbacks[i] = NULL;
                    }
                }
             }

             bool none = true;
             for (i = 0; i < 12; i++) {
                 if (timer->timers[i] != 0) {
                     none = false;
                     break;
                 }
             }
             if (none) world.mask[id] &= ~COMPONENT_TIMER;
        }
    }
}

bool entity_overlaps(int id1, int id2) {
    assert(world.mask[id1] != COMPONENT_NONE);
    assert(world.mask[id2] != COMPONENT_NONE);

    component_position_t *pos1 = &(world.position[id1]);
    component_position_t *pos2 = &(world.position[id2]);
    component_physics_t *phys1 = &(world.physics[id1]);
    component_physics_t *phys2 = &(world.physics[id2]);

    int x_left_1 = pos1->x - (phys1->w/2) + 1;
    int x_right_1 = pos1->x + (phys1->w/2);
    int x_left_2 = pos2->x - (phys2->w/2) + 1;
    int x_right_2 = pos2->x + (phys2->w/2);
    int y_top_1 = pos1->y - (phys1->h/2);
    int y_bot_1 = pos1->y + (phys1->h/2);
    int y_top_2 = pos2->y - (phys2->h/2);
    int y_bot_2 = pos2->y + (phys2->h/2);

    return (x_left_1 < x_right_2 &&
            x_left_2 < x_right_1 &&
            y_top_1 < y_bot_2 &&
            y_top_2 < y_bot_1 );
}

static void collision(int id1, int id2) {
    component_physics_t *phys = &(world.physics[id1]);
    message_send(id1, id2, phys->on_collide_action, phys->on_collide_props);
}

void entitys_move() {
    for (int id = 0; id < MAX_ENTITYS; id++) {
        if ((world.mask[id] & COMPONENT_SPEED) == COMPONENT_SPEED && (world.mask[id] & COMPONENT_PHYSICS) == COMPONENT_PHYSICS) {
            component_position_t * pos = &(world.position[id]);
            component_speed_t * speed = &(world.speed[id]);
            component_view_t * view = &(world.view[id]);

            // Update the position if can move
            float old_x = pos->x;
            pos->x += speed->x;

            for (int i = 0; i < MAX_ENTITYS; i++) {
                if (i != id && (world.mask[i] & COMPONENT_PHYSICS) == COMPONENT_PHYSICS) {
                    if (entity_overlaps(id, i)) {
                        component_physics_t *phys1 = &(world.physics[id]);
                        component_physics_t *phys2 = &(world.physics[i]);

                        collision(i, id);

                        if (phys1->solid && phys2->solid) {
                            pos->x = old_x;
                            break;
                        }
                    }
                }
            }

            float old_y = pos->y;
            pos->y += speed->y;

            for (int i = 0; i < MAX_ENTITYS; i++) {
                if (i != id && (world.mask[i] & COMPONENT_PHYSICS) == COMPONENT_PHYSICS) {
                    if (entity_overlaps(id, i)) {
                        component_physics_t *phys1 = &(world.physics[id]);
                        component_physics_t *phys2 = &(world.physics[i]);

                        collision(i, id);

                        if (phys1->solid && phys2->solid) {
                            pos->y = old_y;
                            break;
                        }
                    }
                }
            }


            // Update the animation
            if ((world.mask[id] & COMPONENT_VIEW) == COMPONENT_VIEW && view->animated_on_move) {
                if (speed->x > 0) entity_set_animation(id, RIGHT);
                if (speed->x < 0) entity_set_animation(id, LEFT);
                if (speed->y > 0) entity_set_animation(id, FORWARD);
                if (speed->y < 0) entity_set_animation(id, BACKWARD);
            }
        } else {
            // Stop the animation if animation is moving
            if ((world.mask[id] & COMPONENT_ANIMATION) == COMPONENT_ANIMATION &&
                world.type[id].type != PROP &&
                world.animation[id].type <= BACKWARD) {
                entity_stop_animation(id);
            }
        }
    }
}

void entitys_render() {
    for (int id = 0; id < MAX_ENTITYS; id++) {
        if ((world.mask[id] & COMPONENT_VIEW) == COMPONENT_VIEW) {
            component_position_t * pos = &(world.position[id]);
            component_view_t * view = &(world.view[id]);

            if ((world.mask[id] & COMPONENT_PHYSICS) == COMPONENT_PHYSICS) {
                component_physics_t *phys = &(world.physics[id]);
                sprite_render(view->spr, pos->x, pos->y - phys->h_off, pos->y, pos->z);
            } else {
                sprite_render(view->spr, pos->x, pos->y, pos->y, pos->z);
            }

            if (debug) {
                graphics_draw_point(pos->x, pos->y, pos->z + 1, RED);

                if ((world.mask[id] & COMPONENT_PHYSICS) == COMPONENT_PHYSICS) {
                    component_physics_t *phys = &(world.physics[id]);
                    graphics_draw_rect(pos->x - phys->w/2 + 1, pos->y - phys->h/2, pos->x + phys->w/2, pos->y + phys->h/2, pos->z + 1, BLUE);
                }
            }
        }
    }
}

void entitys_animate() {
    for (int id = 0; id < MAX_ENTITYS; id++) {
        if ((world.mask[id] & COMPONENT_ANIMATION) == COMPONENT_ANIMATION) {
            component_animation_t * animation = &(world.animation[id]);
            component_view_t * view = &(world.view[id]);

            long delta;
            if (animation->timerId != -1) delta = entity_read_timer(id, animation->timerId) / MS_PER_UPDATE;
            else delta = 0;

            if (delta == 0 || delta >= animation->speed) {
                if (delta >= animation->speed) entity_cancel_timer(id, animation->timerId);
                animation->frame = (animation->frame + 1) % 4;

                if (world.type[id].type != TILE) {
                    int i, vars[25];
                    for (i = 0; i < (view->spr->w_spr * view->spr->h_spr); i++) {
                        vars[i] = animation->anim_cycle[animation->frame];
                    }
                    sprite_change_variations(view->spr, &vars[0]);
                } else {
                    sprite_change_sheet(view->spr, animation->anim_cycle[animation->frame]);
                }

                animation->timerId = entity_set_timer(id, -1, NULL);
            }
        }
    }
}

void entitys_read_messages() {
    message_t msg;
    int i = 0;
    while ((i = message_read_one(i, &msg)) != 0) {
        int id = msg.to;
        switch (msg.type) {
            case MOVE: {
                directions_t dir;
                char *token;
                token = strtok(msg.payload, ",");
                if (strcmp(token, "UPDIR") == 0) dir = UPDIR;
                else if (strcmp(token, "DOWNDIR") == 0) dir = DOWNDIR;
                else if (strcmp(token, "LEFTDIR") == 0) dir = LEFTDIR;
                else if (strcmp(token, "RIGHTDIR") == 0) dir = RIGHTDIR;
                else assert(false);

                token = strtok(NULL, ",");
                entity_set_speed(id, dir, atoi(token));
                break;
            }
            case STOP: {
                directions_t dir;
                if (strcmp(msg.payload, "UPDIR") == 0) dir = UPDIR;
                else if (strcmp(msg.payload, "DOWNDIR") == 0) dir = DOWNDIR;
                else if (strcmp(msg.payload, "LEFTDIR") == 0) dir = LEFTDIR;
                else if (strcmp(msg.payload, "RIGHTDIR") == 0) dir = RIGHTDIR;
                else assert(false);

                entity_stop_speed(id, dir);
                break;
            }
            case ANIMATE: {
                anim_type_t anim;
                if (strcmp(msg.payload, "IDDLE") == 0) anim = IDDLE;
                else if (strcmp(msg.payload, "FORWARD") == 0) anim = FORWARD;
                else if (strcmp(msg.payload, "LEFT") == 0) anim = LEFT;
                else if (strcmp(msg.payload, "RIGHT") == 0) anim = RIGHT;
                else if (strcmp(msg.payload, "BACKWARD") == 0) anim = BACKWARD;
                else if (msg.payload == "RIGHT") anim = RIGHT;
                else  assert(false);

                entity_set_animation(id, anim);
                break;
            }
            case DAMAGE: {
                damage_types_t type;
                char *token;
                token = strtok(msg.payload, ",");
                if (strcmp(token, "ACID") == 0) type = ACID;
                else if (strcmp(token, "BLUDGEONING") == 0) type = BLUDGEONING;
                else if (strcmp(token, "COLD") == 0) type = COLD;
                else if (strcmp(token, "FIRE") == 0) type = FIRE;
                else if (strcmp(token, "FORCE") == 0) type = FORCE;
                else if (strcmp(token, "LIGHTNING") == 0) type = LIGHTNING;
                else if (strcmp(token, "NECROTIC") == 0) type = NECROTIC;
                else if (strcmp(token, "PIERCING") == 0) type = PIERCING;
                else if (strcmp(token, "POISON") == 0) type = POISON;
                else if (strcmp(token, "PSYCHIC") == 0) type = PSYCHIC;
                else if (strcmp(token, "RADIANT") == 0) type = RADIANT;
                else if (strcmp(token, "SLASHING") == 0) type = SLASHING;
                else if (strcmp(token, "THUNDER") == 0) type = THUNDER;
                else  assert(false);

                token = strtok(NULL, ",");

                entity_take_damage(id, type, atoi(token));
            }
            case FREEZE: {
                entity_freeze_animation(id);
                break;
            }
        }
    }
}
