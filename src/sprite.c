#include <stdbool.h>
#include "sprite.h"

spr_t * sprite_create(img_t *img, int w_spr, int h_spr, int sheet, const int * types, const int * variations, int rotation, flip_t flip, int r, int g, int b, int a) {
    spr_t *spr = malloc(sizeof(spr_t));

    spr->img = img;
    spr->w_spr = w_spr;
    spr->h_spr = h_spr;
    spr->sheet = sheet;
    memcpy(spr->types, types, sizeof(int) * 25);
    memcpy(spr->variations, variations, sizeof(int) * 25);
    spr->rotation = rotation;
    spr->flip = flip;
    spr->r = r;
    spr->g = g;
    spr->b = b;
    spr->a = a;

    return spr;
}

void sprite_destroy(spr_t * spr) {
    free(spr);
}

void sprite_render(spr_t *spr, int x, int y, int z) {
    int i, j;
    for (i = 0; i < spr->w_spr; i++) {
        for (j = 0; j < spr->h_spr; j++) {
            img_t *img = spr->img;

            int t = j*spr->w_spr + i; // Get the inline index corresponding to that i and j

            // Select the tile corresponding to the sheet, type and variation.
            int sheet_x = (spr->sheet % img->n_sheets_row) * img->w_sheet;
            int sheet_y = (spr->sheet / img->n_sheets_row) * img->h_sheet;
            int variation = spr->variations[t];
            int type = spr->types[t];
            int src_x = (sheet_x + variation);
            int src_y = (sheet_y + type);

            // Set the draw offset so the x, y are in the middle of the hitbox
            int16_t dest_x = (x - (spr->w_spr * img->w_tile / 2) + 1) + (i * img->w_tile);
            int16_t dest_y = (y - (spr->h_spr * img->h_tile / 2) + 1) + (j * img->h_tile);

            graphics_render_texture_modded(img, src_x, src_y, dest_x, dest_y, y, z, spr->rotation, spr->flip, spr->r, spr->g, spr->b, spr->a);
        }
    }
}

void sprite_change_sheet(spr_t *spr, int sheet) {
    spr->sheet = sheet;
}

void sprite_change_types(spr_t *spr, const int * types) {
    memcpy(spr->types, types, sizeof(int)*25);
}

void sprite_change_variations(spr_t *spr, const int * variations) {
    memcpy(spr->variations, variations, sizeof(int)*25);
}

void sprite_update_type(spr_t *spr, int typeId, int type) {
    spr->types[typeId] = type;
}

void sprite_update_variation(spr_t *spr, int variationId, int variation) {
    spr->variations[variationId] = variation;
}

void sprite_change_color(spr_t *spr, int r, int g, int b, int a) {
    if (spr->r != -1) spr->r = r;
    if (spr->g != -1) spr->g = g;
    if (spr->b != -1) spr->b = b;
    if (spr->a != -1) spr->a = a * 2.55;
}

void sprite_flip(spr_t *spr, flip_t flip) {
    spr->flip = flip;
}

void sprite_rotate(spr_t *spr, int rotation) {
    spr->rotation;
}
