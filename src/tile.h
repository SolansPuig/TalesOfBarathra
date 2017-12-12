#ifndef TILE_H
#define TILE_H

typedef struct {
    uint8_t tile;
    uint8_t variation;
    uint8_t object1;
    uint16_t object2;
} tile_t;

void tile_create(world_t *world, tile_t * t, float x, float y, float z) {
    int id = entity_create(world, x, y, z TILE);
    entity_set_view(world, id, "assets/tiles.png", uint8_t type);
}

#endif
