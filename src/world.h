#ifndef WORLD_H
#define WORLD_H

#include "entity.h"

typedef enum {
    GRASS,
    DESERT_GRASS,
    SNOW_GRASS,
    JUNGLE_GRASS,
    DIRT,
    DARK_DIRT,
    JUNGLE_DIRT,
    CURSED_DIRT,
    SAND,
    DARK_SAND,
    SNOW,
    ICE,
    VOID,
    STONE,
    DARK_STONE,
    WATER,
    LAVA,
    DIRT_TILE,
    STONE_TILE,
    RED_TILE,
    STONE_PATH,
    WHITE_TILE,
    DARK_TILE,
    CURSED_TILE
} terrain_type_t;

typedef enum {
    TREE,
    TRUNK,
    SHRUB,
    CAMPFIRE,
    STORAGE
} props_type_t;

int world_create_player(img_t * img, int sheet, int x, int y, int z);

int world_create_terrain(img_t * img, int sheet, int * variations, int * types, int x_relative, int y_relative, int z);

int world_create_specific_terrain(img_t * img, terrain_type_t type, int x_relative, int y_relative, int z);

int world_create_npc(img_t * img, int sheet, int x, int y, int z);

int world_create_fire(img_t * img, int x, int y, int z);

int world_create_prop(img_t * img, int type, int variant, int x, int y, int z);

#endif
