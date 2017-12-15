#include "world.h"
#include "math.h"

static int anim_cycle[4] = {0, 1, 2, 1};

int world_create_player(img_t * img, int sheet, int x, int y, int z) {
    int player = entity_create(x, x, z, PLAYER);
    entity_set_view(player, img, sheet, 1, 1);
    entity_set_solid(player, true);
    entity_set_size(player, 12, 6);
    entity_init_animation(player, 1, anim_cycle, 30);

    return player;
}

void world_check_adjacent(int * ts, int variant, int x_relative, int y_relative, int z) {
    // Get the surrounding tiles
    int a = 0;
    int b = 0;
    int c = 0;
    int d = 0;
    int e = 0;
    int f = 0;
    int g = 0;
    int h = 0;

    for (int i = 0; i < MAX_ENTITYS; i++) {
        if (entity_exists(i) && entity_get_type(i) == TILE) {
            if      (x_relative-1 == entity_get_x(i)/(2*TILESIZE) && y_relative-1 == entity_get_y(i)/(2*TILESIZE))  a = entity_get_variant(i) != variant;
            else if (x_relative == entity_get_x(i)/(2*TILESIZE) && y_relative-1 == entity_get_y(i)/(2*TILESIZE))    b = entity_get_variant(i) != variant;
            else if (x_relative+1 == entity_get_x(i)/(2*TILESIZE) && y_relative-1 == entity_get_y(i)/(2*TILESIZE))  c = entity_get_variant(i) != variant;
            else if (x_relative-1 == entity_get_x(i)/(2*TILESIZE) && y_relative == entity_get_y(i)/(2*TILESIZE))    d = entity_get_variant(i) != variant;
            else if (x_relative+1 == entity_get_x(i)/(2*TILESIZE) && y_relative == entity_get_y(i)/(2*TILESIZE))    e = entity_get_variant(i) != variant;
            else if (x_relative-1 == entity_get_x(i)/(2*TILESIZE) && y_relative+1 == entity_get_y(i)/(2*TILESIZE))  f = entity_get_variant(i) != variant;
            else if (x_relative == entity_get_x(i)/(2*TILESIZE) && y_relative+1 == entity_get_y(i)/(2*TILESIZE))    g = entity_get_variant(i) != variant;
            else if (x_relative+1 == entity_get_x(i)/(2*TILESIZE) && y_relative+1 == entity_get_y(i)/(2*TILESIZE))  h = entity_get_variant(i) != variant;
        }
    }

    // Top left
    ts[0] = a*1 + b*2 + d*4;
    if (ts[0] == 6) ts[0] = 7;
    if (ts[0] == 2) ts[0] = 3;
    if (ts[0] == 4) ts[0] = 5;

    // Top right
    ts[1] = b*1 + c*2 + e*8;
    if (ts[1] == 9) ts[1] = 11;
    if (ts[1] == 8) ts[1] = 10;
    if (ts[1] == 1) ts[1] = 3;

    // Bot left
    ts[2] = d*1 + f*4 + g*8;
    if (ts[2] == 9) ts[2] = 13;
    if (ts[2] == 1) ts[2] = 5;
    if (ts[2] == 8) ts[2] = 12;

    // Bot right
    ts[3] = e*2 + g*4 + h*8;
    if (ts[3] == 6) ts[3] = 14;
    if (ts[3] == 4) ts[3] = 12;
    if (ts[3] == 2) ts[3] = 10;
}

int world_create_terrain(img_t * img, int sheet, int * variations, int * types, int x_relative, int y_relative, int z) {
    int x = x_relative * (2 * TILESIZE);
    int y = y_relative * (2 * TILESIZE);
    int tile = entity_create(x, y, z, TILE);
    entity_set_view(tile, img, sheet, 2, 2);
    entity_set_view_variations(tile, variations);
    entity_set_view_types(tile, types);

    return tile;
}

int world_create_specific_terrain(img_t * img, terrain_type_t type, int x_relative, int y_relative, int z) {
    int variations[25];
    int types[25];
    switch (type) {
        case GRASS: {
            for (int i = 0; i < 4; i++) {
                if (math_random_prob(92)) {
                    variations[i] = math_random(2, 6);
                    types[i] = 1;
                } else if (math_random_prob(80)) {
                    variations[i] = math_random(1, 6);
                    types[i] = 2;
                } else {
                    variations[i] = math_random(9, 12);
                    types[i] = 2;
                }
            }
            break;
        }
        case DESERT_GRASS: {
            for (int i = 0; i < 4; i++) {
                if (math_random_prob(92)) {
                    variations[i] = math_random(1, 3);
                    types[i] = 4;
                } else {
                    variations[i] = math_random(1, 6);
                    types[i] = 5;
                }
            }
            break;
        }
        case DIRT: {
            for (int i = 0; i < 4; i++) {
                if (math_random_prob(95)) {
                    if (math_random_prob(60)) variations[i] = 3;
                    else if (math_random_prob(90)) variations[i] = 1;
                    else variations[i] = 2;
                    types[i] = 6;
                } else if (math_random_prob(70)) {
                    variations[i] = math_random(6, 8);
                    types[i] = 7;
                } else {
                    variations[i] = math_random(13, 16);
                    types[i] = 7;
                }
            }
            break;
        }
        case STONE: {
            for (int i = 0; i < 4; i++) {
                if (math_random_prob(96)) {
                    variations[i] = math_random(1, 4);
                    types[i] = 8;
                } else {
                    variations[i] = math_random(10, 12);
                    types[i] = 8;
                }
            }
            break;
        }
        case SAND: {
            for (int i = 0; i < 4; i++) {
                if (math_random_prob(98)) {
                    if (math_random_prob(50)) variations[i] = 1;
                    else variations[i] = math_random(2, 6);
                    types[i] = 9;
                } else if (math_random_prob(87)){
                    variations[i] = math_random(12, 14);
                    types[i] = 9;
                } else {
                    variations[i] = math_random(1, 4);
                    types[i] = 10;
                }
            }
            break;
        }
        case DARK_SAND: {
            for (int i = 0; i < 4; i++) {
                if (math_random_prob(98)) {
                    if (math_random_prob(50)) variations[i] = 1;
                    else if (math_random_prob(5)) variations[i] = 2;
                    else variations[i] = math_random(3, 6);
                    types[i] = 11;
                } else {
                    variations[i] = math_random(8, 10);
                    types[i] = 11;
                }
            }
            break;
        }
    }

    int tile = world_create_terrain(img, 0, variations, types, x_relative, y_relative, 0);
    entity_set_variant(tile, type);

    return tile;
}

void world_update_tile(int id) {
    int type = entity_get_variant(id);
    switch (type) {
        case DIRT: {
            int ts[4];
            int x = entity_get_x(id)/(2 * TILESIZE);
            int y = entity_get_y(id)/(2 * TILESIZE);
            int z = entity_get_z(id)/(2 * TILESIZE);
            world_check_adjacent(ts, type, x, y, z);

            for (int i = 0; i < 4; i++) {
                if (ts[i] != 0) {
                    entity_update_view_variation(id, i, ts[i]+22);
                    entity_update_view_type(id, i, 2);
                }
            }
            break;
        }
        case STONE: {
            for (int i = 0; i < 4; i++) {

            }
            break;
        }
        case SAND: {
            for (int i = 0; i < 4; i++) {

            }
            break;
        }
        case DARK_SAND: {
            for (int i = 0; i < 4; i++) {

            }
            break;
        }
    }
}

int world_create_npc(img_t * img, int sheet, int x, int y, int z) {
    int npc = entity_create(x, y, z, COMPLEX_IA);
    entity_set_view(npc, img, sheet, 1, 1);
    entity_set_solid(npc, true);
    entity_set_size(npc, 12, 6);
    entity_init_animation(npc, 1, anim_cycle, 30);

    return npc;
}
