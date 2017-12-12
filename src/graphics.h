// All the code related to open and close windows and render images to them

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "externals.h"

typedef SDL_Window win_t;
typedef SDL_Renderer ren_t;
typedef SDL_Surface sur_t;
typedef SDL_Texture tex_t;

typedef struct {
    char *title;
    win_t  *window;
    ren_t *renderer;
    uint16_t w;
    uint16_t h;
} screen_t;

typedef struct {
    char *filename;
    tex_t *texture;
    sur_t *image;
    uint16_t w;
    uint16_t h;
    uint8_t typesPerRow;
    uint8_t n_frames;
    uint8_t n_animations;
} img_t;

screen_t * graphics_open_window(char *title, uint16_t w, uint16_t h); // Inits all the video-related functions and opens a window.
// title is the string that will be displayed on the header.
// w and h are the width and height, in pixels, of the window.

img_t * graphics_load_image(screen_t *screen, char *filename, uint16_t w, uint16_t h, uint8_t typesPerRow, uint8_t n_frames, uint8_t n_animations); // Loads an image file. Must be done once and only once for each file.
// screen is the window where the image will be rendered.
// filename is the path of the image file.
// w and h are the width and height, in pixels, of every tile in the image.
// typesPerRow is the number of different tile types that the image has in every row.
// n_frames is the number of frames each animation has.
// n_animations is the number of animations.

void graphics_render_texture(screen_t *screen, const img_t *img, uint16_t src_x, uint16_t src_y, int16_t dest_x, int16_t dest_y); // Renders an image or a part of it.
// screen is the window where the image must be rendered.
// img is the image file that must be rendered.
// src_x and src_y are the coords, in tile-sizes, of the tile that must be rendered.
// dest_x and dest_y are the coords, in pixels, where to draw the top left corner.
// w and h are the size of the sprite to render

void graphics_draw_point(screen_t *screen, int16_t x, int16_t y); // Draws a red point at the specified coordinates.
// screen is the window where the image must be rendered.
// x and y are the coords, in pixels, where to draw the point.

void graphics_clear(screen_t *screen); // Clears the screen.

void graphics_show(screen_t *screen); // Prints to the screen.

void graphics_show_and_clear(screen_t *screen); // Prints to the screen and clears it for the next frame.

void graphics_close_texture(img_t *img); // Unload an image file.

void graphics_close_window(screen_t *screen); // Close all the video-related functions and closes the window.

#endif
