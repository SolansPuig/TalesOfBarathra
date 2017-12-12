// All the code related to render and animate tiles and sites on top of the graphic library

#ifndef SPRITE_H
#define SPRITE_H

#include <stdbool.h>
#include "graphics.h"

typedef enum {FORWARD, LEFT, RIGHT, BACKWARD} anim_t;

typedef struct {
    img_t *img;
    uint8_t w;
    uint8_t h;
    anim_t animation;
    uint8_t anim_cycle[4];
    uint16_t anim_count;
    bool animated;
    uint8_t frame;
    uint8_t defaultFrame;
    uint8_t types[25];
    uint16_t speed;
} spr_t;

spr_t * sprite_create(img_t *img, const uint8_t types[25], uint8_t frame, anim_t animation, uint8_t w, uint8_t h); // Creates an sprite, unique for each entity.
// img is a previously loaded image (see) graphics.h, shared between all sprites from the same file.
// type is a number, starting from 0, to select one spritesheet from the file.
// frame is the initial frame.
// animation is the initial pose.
// w and h are the size the sprite to render

void sprite_destroy(spr_t * spr); // Free the memory of the sprite.

void sprite_render(screen_t *screen, spr_t *spr, int16_t x, int16_t y); // Renders an sprite. Must be done every frame and for every entity.
// screen is the window where the sprite must be rendered.
// spr is the previously loaded sprite.
// x and y are the coords in pixels where to draw the top left corner.

void sprite_freeze(spr_t *spr); // Stops the animation at the current frame.

void sprite_stop_animation(spr_t *spr, uint8_t frame); // Stops the animation at the specified frame (recommended: 1).

void sprite_animate(spr_t *spr); // Starts the animation from the current frame.

void sprite_change_animation(spr_t *spr, anim_t anim); // Changes the animation.
// anim is the desidered animation, one of: FORWARD, LEFT, RIGHT, BACKWARD. Use wisely, as not all spritesheets have all these animations.

#endif
