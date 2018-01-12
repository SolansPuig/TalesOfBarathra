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

void graphics_render_texture_modded(const img_t *img, int src_x, int src_y, int dst_x, int dst_y, int y, int z, int rotation, flip_t flip, int r, int g, int b, int a) {
    rect_t src = { src_x * img->w_tile, src_y * img->h_tile, img->w_tile, img->h_tile};
    rect_t dst = { dst_x, dst_y, img->w_tile, img->h_tile};

    to_render_t ren;
    ren.texture = img->texture;
    memcpy(&ren.src, &src, sizeof(rect_t));
    memcpy(&ren.dst, &dst, sizeof(rect_t));
    ren.y = y;
    ren.z = z;
    ren.rotation = rotation;
    ren.flip = flip;
    ren.r = r;
    ren.g = g;
    ren.b = b;
    ren.a = a;

    renderBuffer[renderBufferIndex] = ren;
    renderBufferIndex ++;
}

void graphics_draw_rect(int x0, int y0, int x1, int y1, int z, int r, int g, int b, int a) {
    rect_t dst = { x0, y0, x1-x0, y1-y0 };
    to_render_t ren;
    memcpy(&ren.dst, &dst, sizeof(rect_t));
    ren.texture = NULL;
    ren.y = y0;
    ren.z = z;
    ren.r = r;
    ren.g = g;
    ren.b = b;
    ren.a = a;

    renderBuffer[renderBufferIndex] = ren;
    renderBufferIndex ++;
}

void graphics_draw_point(int x, int y, int z, int r, int g, int b, int a) {
    graphics_draw_rect(x, y, x+1, y+1, z, r, g, b, a);
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
        to_render_t ren = renderBuffer[i];
        if (ren.texture == NULL) {
            SDL_SetRenderDrawColor(screen->renderer, ren.r, ren.g, ren.b, ren.a );
            SDL_RenderDrawRect(screen->renderer, &ren.dst);
            SDL_SetRenderDrawColor(screen->renderer, BLACK );
        }
        if (ren.r || ren.g || ren.b) SDL_SetTextureColorMod(ren.texture, ren.r, ren.g, ren.b);
        if (ren.a) SDL_SetTextureAlphaMod(ren.texture, ren.a);
        SDL_RenderCopyEx(screen->renderer, ren.texture, &ren.src, &ren.dst, ren.rotation, NULL, ren.flip);
        if (ren.r || ren.g || ren.b) SDL_SetTextureColorMod(ren.texture, 0, 0, 0);
        if (ren.a) SDL_SetTextureAlphaMod(ren.texture, 0);
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
