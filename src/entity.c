#include <stdio.h>
#include "entity.h"

int entity_get_empty_id(world_t *world, entity_type_t entity_type) {
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

void entity_mark_destroyed(world_t *world, int id) {
    world->mask[id] = COMPONENT_NONE;
}

int entity_create(world_t *world, float x, float y, float z, entity_type_t entity_type) {
    int id = entity_get_empty_id(world, entity_type);
    world->mask[id] = COMPONENT_POSITION;
    world->position[id].x = x;
    world->position[id].y = y;
    world->position[id].z = z;

    return id;
}

void entity_destroy(world_t *world, int id) {
    entity_destroy_view(world, id);
    entity_mark_destroyed(world, id);
}

world_t * world_create(void) {
    world_t *world = malloc(sizeof(world_t));
    return world;
}

void entity_set_position(world_t *world, int id, float x, float y, float z) {
    assert(world->mask[id] != COMPONENT_NONE);
    component_position_t *pos = &(world->position[id]);
    pos->x = x;
    pos->y = y;
    pos->z = z;

    world->mask[id] |= COMPONENT_POSITION;
}

void entity_set_velocity(world_t *world, int id, directions_t dir, float qty) {
    assert(world->mask[id] != COMPONENT_NONE);
    component_velocity_t *vel = &(world->velocity[id]);
    switch (dir) {
        case UPDIR: {
            vel->y = -qty;
            break;
        }
        case DOWNDIR: {
            vel->y = qty;
            break;
        }
        case LEFTDIR: {
            vel->x = -qty;
            break;
        }
        case RIGHTDIR: {
            vel->x = qty;
            break;
        }
    }
    world->mask[id] |= COMPONENT_VELOCITY;
}

void entity_stop_velocity(world_t *world, int id, directions_t dir) {
    assert(world->mask[id] != COMPONENT_NONE);
    if ((world->mask[id] & COMPONENT_VELOCITY) == COMPONENT_VELOCITY) {
        component_velocity_t *vel = &(world->velocity[id]);
        // Stop the required axis velocity if active
        switch (dir) {
            case UPDIR: {
                if (vel->y < 0) vel->y = 0;
                break;
            }
            case DOWNDIR: {
                if (vel->y > 0) vel->y = 0;
                break;
            }
            case LEFTDIR: {
                if (vel->x < 0) vel->x = 0;
                break;
            }
            case RIGHTDIR: {
                if (vel->x > 0) vel->x = 0;
                break;
            }
        }

        // If all velocitys 0, mark the component as unused
        if (vel->x == 0 && vel->y == 0) {
            world->mask[id] ^= COMPONENT_VELOCITY;
        }
    }
}

void entity_set_view(world_t *world, int id, img_t *img, uint8_t type) {
    assert(world->mask[id] != COMPONENT_NONE);
    component_view_t *view = &(world->view[id]);
    uint8_t types[25] = {type};
    view->spr = sprite_create(img, types, 1, FORWARD, 1, 1);

    world->mask[id] |= COMPONENT_VIEW;
}

void entity_set_view_tile(world_t *world, int id, img_t *img, uint8_t terrain, uint8_t variation) {
    assert(world->mask[id] != COMPONENT_NONE);
    component_view_t *view = &(world->view[id]);
    uint8_t type = (terrain * img->typesPerRow) + variation;
    uint8_t types[25] = {type, type, type, type};
    view->spr = sprite_create(img, types, 0, 0, 2, 2);

    world->mask[id] |= COMPONENT_VIEW;
}

void entity_destroy_view(world_t *world, int id) {
    assert(world->mask[id] != COMPONENT_NONE);
    component_view_t *view = &(world->view[id]);
    sprite_destroy(view->spr);

    world->mask[id] ^= COMPONENT_VIEW;
}

void entity_set_solid(world_t *world, int id, bool solid) {
    assert(world->mask[id] != COMPONENT_NONE);
    component_physics_t *physics = &(world->physics[id]);
    physics->solid = solid;

    world->mask[id] |= COMPONENT_PHYSICS;
}

void entity_set_size(world_t *world, int id, uint8_t w, uint8_t h) {
    assert(world->mask[id] != COMPONENT_NONE);
    component_physics_t *physics = &(world->physics[id]);
    physics->w = w;
    physics->h = h;

    world->mask[id] |= COMPONENT_PHYSICS;
}

bool entity_overlaps(world_t *world, int id1, int id2) {
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

bool entity_can_move(world_t *world, int id) {
    int i;
    for (i = 0; i < MAX_ENTITYS; i++) {
        if (i != id && (world->mask[i] & COMPONENT_PHYSICS) == COMPONENT_PHYSICS) {
            if (entity_overlaps(world, id, i)) {
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

void entitys_move(world_t *world) {
    int id;
    component_position_t *pos;
    component_velocity_t *vel;
    component_view_t *view;

    for (id = 0; id < MAX_ENTITYS; id++) {
        if ((world->mask[id] & COMPONENT_VELOCITY) == COMPONENT_VELOCITY) {
            pos = &(world->position[id]);
            vel = &(world->velocity[id]);
            view = &(world->view[id]);

            // Update the position if can move
            float old_x = pos->x;
            pos->x += vel->x;
            if (!entity_can_move(world, id)) pos->x = old_x;


            float old_y = pos->y;
            pos->y += vel->y;
            if (!entity_can_move(world, id)) pos->y = old_y;

            // Update the animation
            if ((world->mask[id] & COMPONENT_VIEW) == COMPONENT_VIEW) {
                if (vel->x > 0) sprite_change_animation(view->spr, RIGHT);
                if (vel->x < 0) sprite_change_animation(view->spr, LEFT);
                if (vel->y > 0) sprite_change_animation(view->spr, FORWARD);
                if (vel->y < 0) sprite_change_animation(view->spr, BACKWARD);
                sprite_animate(view->spr);
            }
        } else {
            // Stop the animation
            if ((world->mask[id] & COMPONENT_VIEW) == COMPONENT_VIEW) {
                view = &(world->view[id]);
                sprite_stop_animation(view->spr, view->spr->defaultFrame);
            }
        }
    }
}

static world_t *w;

int zorder (const void * elem1, const void * elem2) {
    float z1 = w->position[*((int*)elem1)].z;
    float z2 = w->position[*((int*)elem2)].z;

    if (z1 > z2) return 1;
    if (z1 < z2) return -1;

    float y1 = w->position[*((int*)elem1)].y;
    float y2 = w->position[*((int*)elem2)].y;

    if (y1 > y2) return 1;
    if (y1 < y2) return -1;

    return 0;
}

void entitys_render(world_t *world, screen_t *screen) {
    int id;
    component_position_t *pos;
    component_view_t *view;

    int zbuffer[MAX_ENTITYS];
    int zindex = 0;

    // Set all the entity with view component  ids to an array
    for (id = 0; id < MAX_ENTITYS; id++) {
        if ((world->mask[id] & COMPONENT_VIEW) == COMPONENT_VIEW) {
            zbuffer[zindex] = id;
            zindex++;
        }
    }

    // Sort the array based on the z-layer
    w = world;
    qsort(zbuffer, zindex, sizeof(*zbuffer), zorder);

    // Render every sprite in the array
    int i;
    for (i = 0; i < zindex; i++) {
        id = zbuffer[i];

        pos = &(world->position[id]);
        view = &(world->view[id]);

        sprite_render(screen, view->spr, pos->x, pos->y);
        if (debug) graphics_draw_point(screen, pos->x, pos->y);
    }
}

void entitys_collide(world_t *world) {

}
