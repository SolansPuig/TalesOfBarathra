#include <stdio.h>
#include "entity.h"

world_t * world;

// CREATION AND DESTRUCTION FUNCTIONS
void world_create(void) {
    world = malloc(sizeof(world_t));
}

int entity_get_empty_id(entity_type_t entity_type) {
    int id = 0;
    int max = MAX_ENTITYS;
    for (id; id < max; id++) {
        if (world->mask[id] == COMPONENT_NONE) {
            break;
        }
    }

    assert(id < max);
    return id;
}

void entity_mark_destroyed(int id) {
    world->mask[id] = COMPONENT_NONE;
}

int entity_create(float x, float y, float z, entity_type_t entity_type) {
    int id = entity_get_empty_id(entity_type);
    world->mask[id] = COMPONENT_POSITION;
    world->position[id].x = x;
    world->position[id].y = y;
    world->position[id].z = z;

    return id;
}

void entity_destroy(int id) {
    entity_destroy_view(id);
    entity_mark_destroyed(id);
}



// POSITION FUNCTIONS
void entity_set_x(int id, float x) {
    assert(world->mask[id] != COMPONENT_NONE);
    component_position_t *pos = &(world->position[id]);
    pos->x = x;

    world->mask[id] |= COMPONENT_POSITION;
}

void entity_set_y(int id, float y) {
    assert(world->mask[id] != COMPONENT_NONE);
    component_position_t *pos = &(world->position[id]);
    pos->y = y;

    world->mask[id] |= COMPONENT_POSITION;
}

void entity_set_z(int id, float z) {
    assert(world->mask[id] != COMPONENT_NONE);
    component_position_t *pos = &(world->position[id]);
    pos->z = z;

    world->mask[id] |= COMPONENT_POSITION;
}



// SPEED FUNCTIONS
void entity_set_speed(int id, directions_t dir, float qty) {
    assert(world->mask[id] != COMPONENT_NONE);
    component_speed_t *speed = &(world->speed[id]);
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
    world->mask[id] |= COMPONENT_SPEED;
}

void entity_stop_speed(int id, directions_t dir) {
    assert(world->mask[id] != COMPONENT_NONE);
    if ((world->mask[id] & COMPONENT_SPEED) == COMPONENT_SPEED) {
        component_speed_t *speed = &(world->speed[id]);
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
            world->mask[id] ^= COMPONENT_SPEED;
        }
    }
}



// VIEW FUNCTIONS
void entity_set_view(int id, img_t *img, int sheet) {
    assert(world->mask[id] != COMPONENT_NONE);
    component_view_t *view = &(world->view[id]);
    int types[25] = {0};
    int variations[25] = {0};
    view->spr = sprite_create(img, 1, 1, sheet, types, variations);

    world->mask[id] |= COMPONENT_VIEW;
}

void entity_destroy_view(int id) {
    assert(world->mask[id] != COMPONENT_NONE);
    component_view_t *view = &(world->view[id]);
    sprite_destroy(view->spr);

    world->mask[id] ^= COMPONENT_VIEW;
}



// ANIMATION FUNCTIONS
void entity_init_animation(int id, int frame, int * anim_cycle, int speed) {
    assert(world->mask[id] != COMPONENT_NONE);
    component_animation_t *animation = &(world->animation[id]);
    animation->speed = 2000 - speed;
    animation->frame = frame;
    memcpy(animation->anim_cycle, anim_cycle, 4);
}

void entity_set_animation(int id, anim_t anim_type) {
    assert(world->mask[id] != COMPONENT_NONE);

    if ((world->mask[id] & COMPONENT_VIEW) == COMPONENT_VIEW) {
        component_view_t *view = &(world->view[id]);
        int i, types[25];
        for (i = 0; i < 25; i++) {
             types[i] = anim_type;
        }
        sprite_change_types(view->spr, &types[0]);
    }

    entity_set_timer(id, -1, NULL);

    world->mask[id] |= COMPONENT_ANIMATION;
}

void entity_freeze_animation(int id) {
    assert(world->mask[id] != COMPONENT_NONE);
    component_animation_t *animation = &(world->animation[id]);

    entity_cancel_timer(id, animation->timerId);

    world->mask[id] ^= COMPONENT_ANIMATION;
}

void entity_stop_animation_at(int id, int frame) {
    entity_freeze_animation(id);
    assert(world->mask[id] != COMPONENT_NONE);
    component_animation_t *animation = &(world->animation[id]);
    component_view_t *view = &(world->view[id]);

    animation->frame = frame % 4;

    int i, vars[25];
    for (i = 0; i < (view->spr->w_spr * view->spr->h_spr); i++) {
        vars[i] = animation->anim_cycle[animation->frame];
    }
    sprite_change_variations(view->spr, &vars[0]);
}

void entity_stop_animation(int id) {
    entity_stop_animation_at(id, 0);
}



// PHYSICS FUNCTIONS
void entity_set_solid(int id, bool solid) {
    assert(world->mask[id] != COMPONENT_NONE);
    component_physics_t *physics = &(world->physics[id]);
    physics->solid = solid;

    world->mask[id] |= COMPONENT_PHYSICS;
}

void entity_set_size(int id, int w, int h) {
    assert(world->mask[id] != COMPONENT_NONE);
    component_physics_t *physics = &(world->physics[id]);
    physics->w = w;
    physics->h = h;

    world->mask[id] |= COMPONENT_PHYSICS;
}



// TIMER FUNCTIONS
int entity_set_timer(int id, long time, void (*callback)(int)) {
    assert(world->mask[id] != COMPONENT_NONE);
    component_timer_t *timer = &(world->timer[id]);
    int i;
    for (i = 0; i < 12; i++) {
        if (timer->timers[i] == 0) {
            timer->counts[i] = 0;
            timer->timers[i] = time;
            //timer->callbacks[i] = callback;
            break;
        }
    }

    world->mask[id] |= COMPONENT_TIMER;
    return i;
}

long entity_read_timer(int id, int timerId) {
    assert(world->mask[id] != COMPONENT_NONE);
    component_timer_t *timer = &(world->timer[id]);

    return timer->counts[timerId];
}

long entity_cancel_timer(int id, int timerId) {
    assert(world->mask[id] != COMPONENT_NONE);
    component_timer_t *timer = &(world->timer[id]);
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

    world->mask[id] ^= COMPONENT_TIMER;
    return c;
}


// SYSTEMS
void entitys_timer() {
    component_timer_t *timer;

    // Set all the entity with timer component  ids to an array
    int id;
    for (id = 0; id < MAX_ENTITYS; id++) {
        if ((world->mask[id] & COMPONENT_TIMER) == COMPONENT_TIMER) {
            timer = &(world->timer[id]);
            int i;
            for (i = 0; i < 12; i++) {
                if (timer->timers[i] != 0) {
                    timer->counts[i]++;
                    if (timer->counts[i] >= timer->timers[i]) {
                        timer->counts[i] = 0;
                        timer->timers[i] = -1;
                        //timer->callbacks[i](id);
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
             if (none) world->mask[id] ^= COMPONENT_TIMER;
        }
    }
}

bool entity_overlaps(int id1, int id2) {
    assert(world->mask[id1] != COMPONENT_NONE);
    assert(world->mask[id2] != COMPONENT_NONE);

    component_position_t *pos1 = &(world->position[id1]);
    component_position_t *pos2 = &(world->position[id2]);
    component_physics_t *phys1 = &(world->physics[id1]);
    component_physics_t *phys2 = &(world->physics[id2]);

    int x_left_1 = pos1->x - (phys1->w/2);
    int x_right_1 = pos1->x + (phys1->w/2);
    int x_left_2 = pos2->x - (phys2->w/2);
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

bool entity_can_move(int id) {
    int i;
    for (i = 0; i < MAX_ENTITYS; i++) {
        if (i != id && (world->mask[i] & COMPONENT_PHYSICS) == COMPONENT_PHYSICS) {
            if (entity_overlaps(id, i)) {
                component_physics_t *phys1 = &(world->physics[id]);
                component_physics_t *phys2 = &(world->physics[i]);

                if (phys1->solid && phys2->solid) {
                    return false;
                }
            }
        }
    }
    return true;
}

void entitys_move() {
    component_position_t *pos;
    component_speed_t *speed;
    component_view_t *view;
    int id;
    for (id = 0; id < MAX_ENTITYS; id++) {
        if ((world->mask[id] & COMPONENT_SPEED) == COMPONENT_SPEED) {
            pos = &(world->position[id]);
            speed = &(world->speed[id]);
            view = &(world->view[id]);

            // Update the position if can move
            float old_x = pos->x;
            pos->x += speed->x;
            if (!entity_can_move(id)) pos->x = old_x;


            float old_y = pos->y;
            pos->y += speed->y;
            if (!entity_can_move(id)) pos->y = old_y;

            // Update the animation
            if ((world->mask[id] & COMPONENT_VIEW) == COMPONENT_VIEW) {
                if (speed->x > 0) entity_set_animation(id, RIGHT);
                if (speed->x < 0) entity_set_animation(id, LEFT);
                if (speed->y > 0) entity_set_animation(id, FORWARD);
                if (speed->y < 0) entity_set_animation(id, BACKWARD);
            }
        } else {
            // Stop the animation
            if ((world->mask[id] & COMPONENT_VIEW) == COMPONENT_VIEW) {
                view = &(world->view[id]);
                entity_stop_animation(id);
            }
        }
    }
}

void entitys_render() {
    component_position_t *pos;
    component_view_t *view;

    // Set all the entity with view component  ids to an array
    int id;
    for (id = 0; id < MAX_ENTITYS; id++) {
        if ((world->mask[id] & COMPONENT_VIEW) == COMPONENT_VIEW) {
            pos = &(world->position[id]);
            view = &(world->view[id]);
            sprite_render(view->spr, pos->x, pos->y, pos->z);
            //if (debug) graphics_draw_point(screen, pos->x, pos->y);
        }
    }
}

void entitys_animate() {
    component_animation_t *animation;
    component_view_t *view;
    int id;
    for (id = 0; id < MAX_ENTITYS; id++) {
        if ((world->mask[id] & COMPONENT_ANIMATION) == COMPONENT_ANIMATION) {
            animation = &(world->animation[id]);
            view = &(world->view[id]);
            printf("Hi\n");

            long delta = entity_read_timer(id, animation->timerId);
            if (delta >= animation->speed) {
                printf("%ld %d\n", delta, animation->speed);
                entity_cancel_timer(id, animation->timerId);
                animation->frame = (animation->frame + 1) % 4;

                int i, vars[25];
                for (i = 0; i < (view->spr->w_spr * view->spr->h_spr); i++) {
                    vars[i] = animation->anim_cycle[animation->frame];
                }
                sprite_change_variations(view->spr, &vars[0]);

                entity_set_timer(id, -1, NULL);
            } else if (delta == 0) {
                entity_set_timer(id, -1, NULL);
            }
        }
    }
}

void entitys_collide(world_t *world) {

}
