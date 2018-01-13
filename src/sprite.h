// All the code related to render and animate tiles and sites on top of the graphic library
// Note on the notation:
//    An image is a file that holds multiple sheets.
//    A sheet is a group of tiles that are grouped for some reason, for example being different animations of the same character; or different variations of similar terrain,
//         for example grass is one sheet, dirt another... A sheet holds multiple types.
//    A type is a group of tiles that are in the same sheet, but pretty different, for example they belong to the same animation, like walking forward is one type, walking
//         backward another...; or grass surrounded by grass is one type, grass surrounded by dirt another... A type holds multiple variations. Types must be full-width
//         single-height rows of the same sheet.
//    A variation is a single tile contained in a type, for example one frame of one animation, or the same grass but slightly different to look more natural.
//
//              A FILE:
//          |-----------------------|-----------------------|-----------------------|
//          |                       |        A TYPE         |                       |
//          |                       |-----------------------|                       |
//          |       A SHEET         |     ANOTHER TYPE      |                       |
//          |                       |-----------------------|                       |
//          |                       | VARI. | VARI. | VARI. |                       |
//          |-----------------------|-----------------------|-----------------------|
//          |                       |                       |                       |
//          |     ANOTHER SHEET     |                       |                       |
//          |                       |                       |                       |
//          |                       |                       |                       |
//          |-----------------------|-----------------------|-----------------------|


#ifndef SPRITE_H
#define SPRITE_H

#include <stdbool.h>
#include "graphics.h"

typedef struct {
    img_t *img;
    int w_spr;
    int h_spr;
    int sheet;
    int types[25];
    int variations[25];
    int rotation;
    flip_t flip;
    int r;
    int g;
    int b;
    int a;
} spr_t;

spr_t * sprite_create(img_t *img, int w_spr, int h_spr, int sheet, const int * types, const int * variations, int rotation, flip_t flip, int r, int g, int b, int a);
// Creates an sprite, unique for each entity.
//     img is the file containing this sprite.
//     w_spr and h_spr are the size of the sprite, in tile-sizes.
//     sheet is the group of similar tiles containing this sprite.
//     types is the list of tile types that build this sprite.
//     variations is the list of tile variations that build this sprite.
//      rotation is a number, in degrees, to rotate the image.
//      flip is one of { NO_FLIP, FLIP_HORIZONTAL, FLIP_VERTICAL, FLIP_DIAGONAL }.
//      r is the red value used to mod the original image.
//      g is the green value used to mod the original image.
//      b the blue value used to mod the original image.
//      a the alpha value used to mod the original image.

void sprite_destroy(spr_t * spr); // Free the memory of the sprite.

void sprite_render(spr_t *spr, int x, int y, int y2, int z);
// Renders an sprite. Must be done every frame and for every entity.
//     spr is the previously loaded sprite.
//     x and y are the coords in pixels where to draw the top left corner.
//     y2 and z are for depth buffering.

void sprite_change_sheet(spr_t *spr, int sheet);
// Changes the sheet (AKA, in most cases, changes the animation frame of a tile).
//     spr is the previously loaded sprite.
//     sheet is the sheet we want to change to.

void sprite_change_types(spr_t *spr, const int * types);
// Changes the types list (AKA, in most cases, changes the animation).
//     spr is the previously loaded sprite.
//     types is the list of tile types to change.

void sprite_update_type(spr_t *spr, int typeId, int type);
// Changes a single type from the list.
//     spr is the previously loaded sprite.
//     typeId is the index in the type list.
//     type is the new value for that index.

void sprite_change_variations(spr_t *spr, const int * variations);
// Changes the variations list (AKA, in most cases, changes the animation frame).
//     spr is the previously loaded sprite.
//     variations is the list of tile variations to change.

void sprite_update_variation(spr_t *spr, int variationId, int variation);
// Changes a single variation from the list.
//     spr is the previously loaded sprite.
//     variationId is the index in the variation list.
//     variation is the new value for that index.

void sprite_change_color(spr_t *spr, int r, int g, int b, int a);
// Change the color of the sprite.
//      r, g, and b are the red, green and blue values. Use -1 to leave it unaltered.
//      a is the alpha. Use -1 to leave it unaltered.

void sprite_flip(spr_t *spr, flip_t flip);
// Flips the sprite.
//      flip is one of { NO_FLIP, FLIP_HORIZONTAL, FLIP_VERTICAL, FLIP_DIAGONAL }.

void sprite_rotate(spr_t *spr, int rotation);
// Rotates a sprite.
//      rotation is a number, in degrees, to rotate the image.


#endif
