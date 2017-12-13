#include <stdbool.h>
#include "sprite.h"

spr_t * sprite_create(img_t *img, int w_spr, int h_spr, int sheet, const int * types, const int * variations) {
    spr_t *spr = malloc(sizeof(spr_t));

    spr->img = img;
    spr->w_spr = w_spr;
    spr->h_spr = h_spr;
    spr->sheet = sheet;
    memcpy(spr->types, types, sizeof(int) * 25);
    memcpy(spr->variations, variations, sizeof(int) * 25);

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

            graphics_render_texture(img, src_x, src_y, dest_x, dest_y, y, z);
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
