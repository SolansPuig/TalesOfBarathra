#include <stdbool.h>
#include "sprite.h"

spr_t * sprite_create(img_t *img, const uint8_t types[25], uint8_t frame, anim_t animation, uint8_t w, uint8_t h) {
    spr_t *spr = malloc(sizeof(spr_t));

    // Important
    spr->img = img;
    memcpy(spr->types, types, 25);
    spr->w = w;
    spr->h = h;

    // Animation
    spr->animation = animation;
    spr->anim_count = 0;
    spr->animated = false;
    spr->frame = frame;
    spr->defaultFrame = frame;
    spr->speed = 350;
    uint8_t anim_cycle_default[4] = {0, 1, 2, 1};
    memcpy(spr->anim_cycle, anim_cycle_default, sizeof(anim_cycle_default));

    return spr;
}

void sprite_destroy(spr_t * spr) {
    free(spr);
}

void sprite_render(screen_t *screen, spr_t *spr, int16_t x, int16_t y) {
    if (spr->animated) {
        if (SDL_GetTicks() - spr->anim_count >= (500 - spr->speed)) {
            spr->anim_count = SDL_GetTicks();
            spr->frame = (spr->frame + 1) % 4;
        }
    }

    int i, j;
    for (i = 0; i < spr->w; i++) {
        for (j = 0; j < spr->h; j++) {
            uint16_t t = j*spr->w + i;
            uint16_t src_x = spr->anim_cycle[spr->frame] + spr->img->n_frames * (spr->types[t] % spr->img->typesPerRow);
            uint16_t src_y = spr->animation + spr->img->n_animations * (spr->types[t] / spr->img->typesPerRow);

            // Set the draw offset so the x, y are in the middle of the hitbox
            int16_t dest_x = (x - (spr->w * spr->img->w / 2) + 1) + (i * spr->img->w);
            int16_t dest_y = (y - (spr->h * spr->img->h / 2) + 1) + (j * spr->img->h);
            graphics_render_texture(screen, spr->img, src_x, src_y, dest_x, dest_y);
        }
    }
}

void sprite_freeze(spr_t *spr) {
    spr->animated = false;
}

void sprite_stop_animation(spr_t *spr, uint8_t frame) {
    spr->animated = false;
    spr->frame = frame;
}

void sprite_animate(spr_t *spr) {
    spr->animated = true;
}

void sprite_change_animation(spr_t *spr, anim_t anim) {
    spr->animation = anim;
}
