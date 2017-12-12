#include "graphics.h"

screen_t * graphics_open_window(char *title, uint16_t w, uint16_t h) {
    screen_t *screen = malloc(sizeof(screen_t));
    screen->title = title;
    screen->w = w;
    screen->h = h;

    SDL_Init(SDL_INIT_VIDEO);

    screen->window = SDL_CreateWindow(screen->title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen->w, screen->h, SDL_WINDOW_SHOWN);
    screen->renderer = SDL_CreateRenderer(screen->window, -1, 0);
    SDL_SetRenderDrawColor(screen->renderer, 0, 0, 0, 255);
    SDL_RenderSetScale(screen->renderer, 3, 3);
    SDL_RenderClear(screen->renderer);
    SDL_RenderPresent(screen->renderer);

    return screen;
}

img_t * graphics_load_image(screen_t *screen, char *filename, uint16_t w, uint16_t h, uint8_t typesPerRow, uint8_t n_frames, uint8_t n_animations) {
    img_t *img = malloc(sizeof(img_t));

    img->filename = filename;
    img->image = IMG_Load(img->filename);
    img->texture = SDL_CreateTextureFromSurface(screen->renderer, img->image);
    img->w = w;
    img->h = h;
    img->typesPerRow = typesPerRow;
    img->n_frames = n_frames;
    img->n_animations = n_animations;
    return img;
}

void graphics_render_texture(screen_t *screen, const img_t *img, uint16_t src_x, uint16_t src_y, int16_t dest_x, int16_t dest_y) {
    SDL_Rect srcrect = { src_x * img->w, src_y * img->h, img->w, img->h};
    SDL_Rect dstrect = { dest_x, dest_y, img->w, img->h};
    SDL_RenderCopy(screen->renderer, img->texture, &srcrect, &dstrect);
}

void graphics_draw_point(screen_t *screen, int16_t x, int16_t y) {
    SDL_SetRenderDrawColor(screen->renderer, 255, 0, 0, 255 );
    SDL_RenderDrawPoint(screen->renderer, x, y);
    SDL_SetRenderDrawColor(screen->renderer, 0, 0, 0, 255 );
}

void graphics_show(screen_t *screen) {
    int i;
    SDL_RenderPresent(screen->renderer);
}

void graphics_clear(screen_t *screen) {
    SDL_RenderClear(screen->renderer);
}

void graphics_show_and_clear(screen_t *screen) {
    SDL_RenderPresent(screen->renderer);
    SDL_RenderClear(screen->renderer);
}

void graphics_close_texture(img_t *img) {
    SDL_DestroyTexture(img->texture);
    SDL_FreeSurface(img->image);
}

void graphics_close_window(screen_t *screen) {
    SDL_DestroyRenderer(screen->renderer);
    SDL_DestroyWindow(screen->window);
    SDL_Quit();
}
