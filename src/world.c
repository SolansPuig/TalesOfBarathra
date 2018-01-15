#include "world.h"
#include "math.h"
#include "frozen.h"

static int anim_cycle[4] = {0, 1, 2, 1};

static img_t *knights;;
static img_t *fire;
static img_t *outside;
static img_t *terrain;


int load(int type) {
    switch (type) {
        case PLAYER: {
            if (knights == NULL) knights = graphics_load_image("assets/knight_factions_1.png", 26, 36, 4, 3, 4);
            return world_create_player(knights, 0, 40, 50, 2);
            break;
        }
        case PROP: {
            if (fire == NULL) fire = graphics_load_image("assets/fire.png", 16, 16, 1, 4, 1);
            if (outside == NULL) outside = graphics_load_image("assets/outside.png", 16, 16, 1, 52, 24);
            world_create_prop(fire, "fire", 101, 202, 2);
            world_create_prop(fire, "fire", 95, 200, 2);
            world_create_prop(fire, "fire", 103, 196, 2);
            world_create_prop(outside, "campfire", 100, 200, 1);

            world_create_prop(outside, "tree0", 120, 150, 2);
            world_create_prop(outside, "abba", 220, 100, 2);
            break;
        }
        case COMPLEX_IA: {
            if (knights == NULL) knights = graphics_load_image("assets/knight_factions_1.png", 26, 36, 4, 3, 4);
            int Steve = world_create_npc(knights, 2, 300, 200, 2);
            int Charlie = world_create_npc(knights, 1, 300, 150, 2);
            entity_change_colors(Steve, 100, 255, 218);
            entity_change_alpha(Charlie, 80);
            break;
        }
        case TILE: {
            if (terrain == NULL) terrain = graphics_load_image("assets/terrain.png", 16, 16, 1, 39, 38);
            for (int x = 0; x < 50; x++) {
                for (int y = 0; y < 50; y++) {
                    int a = world_create_specific_terrain(terrain, GRASS, x, y, 0);
                    entity_change_colors(a, 220, 220, 220);
                }
            }
        }
    }

    return 0;
}

void unload(int type) {
    for (int i = 0; i < MAX_ENTITYS; i++) {
        if (entity_exists(i) && entity_get_type(i) == type) entity_destroy(i);
    }
}

void reload(int type) {
    unload(type);
    load(type);
}

void scan_int_array(const char *str, int len, void *user_data) {
    struct json_token t;
    for (int i = 0; json_scanf_array_elem(str, len, "", i, &t) > 0; i++) {
        char s[10];
        snprintf(s, sizeof(s), "%.*s\n", t.len, t.ptr);
        int n = atoi(s);
        ((int *)user_data)[i] = n;
    }
}

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

int world_create_prop(img_t * img, char * type, int x, int y, int z) {
    int prop = entity_create(x, y, z, PROP);

    char * str = json_fread("assets/props.json");
    char template[] = "{img: %s, spr_w: %d, spr_h:%d, variations: %M, types: %M, w: %d, h: %d, solid: %B, h_off: %d, anim_cycle: %M, anim_speed: %d, anim_frame: %d}";
    char filter[256];
    snprintf(filter, sizeof(filter), "{%s: %s}", type, template);

    char img_name[32];
    int spr_w, spr_h, w, h, h_off, anim_speed, anim_frame;
    int variations[25];
    int types[25];
    int anim_cycle[] = {-1, -1, -1, -1};
    bool solid;

    json_scanf(str, strlen(str), filter, &img_name, &spr_w, &spr_h, scan_int_array, variations, scan_int_array, types, &w, &h, &solid, &h_off, scan_int_array, anim_cycle, &anim_speed, &anim_frame);
    free(str);

    entity_set_view(prop, img, 0, spr_w, spr_h);
    entity_set_view_variations(prop, variations);
    entity_set_view_types(prop, types);
    entity_set_size(prop, w, h);
    entity_set_solid(prop, solid);
    entity_set_height_offset(prop, h_off);

    if (anim_cycle[0] != -1) {
        if (anim_frame == -1) anim_frame = math_random(0, 4);
        entity_init_animation(prop, anim_frame, anim_cycle, anim_speed);
        entity_set_animation(prop, FORWARD);
    }

    return prop;
}
