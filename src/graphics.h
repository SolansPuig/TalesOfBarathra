// All the code related to open and close windows and render images to them

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "globals.h"

typedef SDL_Window      win_t;
typedef SDL_Renderer    ren_t;
typedef SDL_Surface     sur_t;
typedef SDL_Texture     tex_t;
typedef SDL_Rect        rect_t;
typedef SDL_Color       color_t;

typedef struct {
    char *title;
    win_t  *window;
    ren_t *renderer;
    int w;
    int h;
} screen_t;

typedef struct {
    sur_t *image;
    tex_t *texture;
    int w_tile;
    int h_tile;
    int n_sheets_row;
    int w_sheet;
    int h_sheet;
} img_t;

typedef struct {
    tex_t *texture;
    rect_t src;
    rect_t dst;
    int y;
    int z;
    int r;
    int g;
    int b;
    int a;
} to_render_t;

extern screen_t *screen;

void graphics_open_window(char *title, int w, int h);
// Initializes all the functions related to video and opens a window.
//      title is the string that will be displayed in the header.
//      w and h are the size, in pixels, of the window.

img_t * graphics_load_image(char *filename, int w_tile, int h_tile, int n_sheets_row, int w_sheet, int h_sheet);
// Loads an image file. It must be done once and only once for each file.
//      filename is the path to the image file.
//      w_tile and h_tile are the size, in pixels, of every tile.
//      n_sheets_row are the number of sheets (groups of similar images) that the file contains in each row.
//      w_sheet and h_sheet are the size, in tile-sizes, of every sheet;

void graphics_render_texture(const img_t *img, int src_x, int src_y, int dst_x, int dst_y, int y, int z);
// Adds the image (or the part of it) to the rendering buffer.
//      img is the image file that must be rendered.
//      src_x and src_y are the coords, in tile-sizes, of the tile that must be rendered.
//      dst_x and dst_y are the coords, in pixels, where to draw the top left corner.
//      y and z are the height and depth of the entity calling this, to calculate which image has to render on top.

void graphics_draw_rect(int x0, int y0, int x1, int y1, int z, int r, int g, int b, int a);
// Add a rectangle at the specified coordinates to the rendering buffer.
//      x0 and y0 are the coords, in pixels, where to draw the top left corner of the rectangle.
//      x1 and y1 are the coords, in pixels, where to draw the bottom right corner of the rectangle.
//      y1 and z are also the height and depth of the entity calling this, to calculate which image has to render on top.
//      color is the color we want to draw.
//      r is the red value used to draw on the rendering target
//      g is the green value used to draw on the rendering target
//      b the blue value used to draw on the rendering target
//      a the alpha value used to draw on the rendering target

void graphics_draw_point(int x, int y, int z, int r, int g, int b, int a);
// Add a red point at the specified coordinates to the rendering buffer.
//      x and y are the coords, in pixels, where to draw the point.
//      y and z are also the height and depth of the entity calling this, to calculate which image has to render on top.
//      color is the color we want to draw.
//      r is the red value used to draw on the rendering target
//      g is the green value used to draw on the rendering target
//      b the blue value used to draw on the rendering target
//      a the alpha value used to draw on the rendering target

void graphics_clear();
// Clears the screen.

void graphics_show();
// Prints to the screen.

void graphics_show_and_clear();
// Prints to the screen and clears it for the next frame.

void graphics_close_texture(img_t *img);
// Unloads an image file.

void graphics_close_window();
// Closes all the video-related functions and closes the window.

#endif
