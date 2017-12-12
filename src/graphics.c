#include "graphics.h"

static to_render_t renderBuffer[MAX_ENTITYS];
static int renderBufferIndex;

screen_t *screen;

void graphics_open_window(char *title, int w, int h) {
    screen = malloc(sizeof(screen_t));
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
}

img_t * graphics_load_image(char *filename, int w_tile, int h_tile, int n_sheets_row, int w_sheet, int h_sheet) {
    img_t *img = malloc(sizeof(img_t));

    img->image = IMG_Load(filename);
    img->texture = SDL_CreateTextureFromSurface(screen->renderer, img->image);
    img->w_tile = w_tile;
    img->h_tile = h_tile;
    img->n_sheets_row = n_sheets_row;
    img->w_sheet = w_sheet;
    img->h_sheet = h_sheet;

    return img;
}

void graphics_render_texture(const img_t *img, int src_x, int src_y, int dst_x, int dst_y, int y, int z) {
    rect_t src = { src_x * img->w_tile, src_y * img->h_tile, img->w_tile, img->h_tile};
    rect_t dst = { dst_x, dst_y, img->w_tile, img->h_tile};

    to_render_t r;
    r.texture = img->texture;
    memcpy(&r.src, &src, sizeof(rect_t));
    memcpy(&r.dst, &dst, sizeof(rect_t));
    r.y = y;
    r.z = z;

    renderBuffer[renderBufferIndex] = r;
    renderBufferIndex ++;
}

void graphics_draw_point(int x, int y) {
    SDL_SetRenderDrawColor(screen->renderer, 255, 0, 0, 255 );
    SDL_RenderDrawPoint(screen->renderer, x, y);
    SDL_SetRenderDrawColor(screen->renderer, 0, 0, 0, 255 );
}

int order (const void * elem1, const void * elem2) {
    float z1 = (*(to_render_t*) elem1).z;
    float z2 = (*(to_render_t*) elem2).z;

    if (z1 > z2) return 1;
    if (z1 < z2) return -1;

    float y1 = (*(to_render_t*) elem1).y;
    float y2 = (*(to_render_t*) elem2).y;

    if (y1 > y2) return 1;
    if (y1 < y2) return -1;

    return 0;
}

void graphics_show() {
    qsort(renderBuffer, renderBufferIndex, sizeof(*renderBuffer), order);

    int i;
    for (i = 0; i < renderBufferIndex; i++) {
        to_render_t r = renderBuffer[i];
        SDL_RenderCopy(screen->renderer, r.texture, &r.src, &r.dst);
    }

    SDL_RenderPresent(screen->renderer);
    renderBufferIndex = 0;
}

void graphics_clear() {
    SDL_RenderClear(screen->renderer);
}

void graphics_show_and_clear() {
    graphics_show();
    graphics_clear();
}

void graphics_close_texture(img_t *img) {
    SDL_DestroyTexture(img->texture);
    SDL_FreeSurface(img->image);
}

void graphics_close_window() {
    SDL_DestroyRenderer(screen->renderer);
    SDL_DestroyWindow(screen->window);
    SDL_Quit();
}
