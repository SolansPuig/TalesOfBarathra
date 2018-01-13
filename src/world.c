#include "world.h"
#include "math.h"

static int anim_cycle[4] = {0, 1, 2, 1};

int world_create_player(img_t * img, int sheet, int x, int y, int z) {
    int player = entity_create(x, x, z, PLAYER);
    entity_set_view(player, img, sheet, 1, 1);
    entity_set_solid(player, true);
    entity_set_size(player, 12, 6);
    entity_set_height_offset(player, 15);
    entity_init_animation(player, 1, anim_cycle, 30);

    return player;
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

int world_create_npc(img_t * img, int sheet, int x, int y, int z) {
    int npc = entity_create(x, y, z, COMPLEX_IA);
    entity_set_view(npc, img, sheet, 1, 1);
    entity_set_solid(npc, true);
    entity_set_size(npc, 12, 6);
    entity_set_height_offset(npc, 15);
    entity_init_animation(npc, 1, anim_cycle, 30);

    return npc;
}

int world_create_fire(img_t * img, int x, int y, int z) {
    int fire = entity_create(x, y, z, PROP);
    entity_set_view(fire, img, 0, 1, 1);
    entity_set_solid(fire, true);
    entity_set_size(fire, 12, 6);
    entity_set_height_offset(fire, 5);
    int fire_cycle[4] = {0, 1, 2, 3};
    entity_init_animation(fire, math_random(0, 4), fire_cycle, 30);
    entity_set_animation(fire, FORWARD);

    return fire;
}

int world_create_prop(img_t * img, int type, int variant, int x, int y, int z) {
    int prop = entity_create(x, y, z, PROP);

    switch (type) {
        case TREE: {
            if (variant == 0) {
                entity_set_view(prop, img, 0, 4, 4);
                int variations[16] = {17, 18, 19, 20, 17, 18, 19, 20, 17, 18, 19, 20, 17, 18, 19, 20};
                int types[16] = {3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6};
                entity_set_view_variations(prop, variations);
                entity_set_view_types(prop, types);

                entity_set_size(prop, 18, 6);
                entity_set_solid(prop, true);
                entity_set_height_offset(prop, 25);
                break;
            }
        }

        case CAMPFIRE: {
            if (variant == 0) {
                entity_set_view(prop, img, 0, 3, 2);
                int variations[6] = {1, 2, 3, 1, 2, 3};
                int types[6] = {18, 18, 18, 19, 19, 19};
                entity_set_view_variations(prop, variations);
                entity_set_view_types(prop, types);

                entity_set_size(prop, 26, 22);
                entity_set_height_offset(prop, -3);
                break;
            }
        }
    }

    entity_set_variant(prop, type);
    return prop;
}
