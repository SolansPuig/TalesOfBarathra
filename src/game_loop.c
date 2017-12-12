#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "globals.h"
#include "sprite.h"
#include "entity.h"

#define MS_PER_UPDATE 16

static int player;
static bool done;
static world_t *world;
bool debug;

void get_input() {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            done = true;
            break;
        }
        if (event.type == SDL_KEYDOWN) {
            SDL_Keycode key = event.key.keysym.sym;
            switch (key) {
                case SDLK_ESCAPE: {
                    done = true;
                    break;
                }
                case SDLK_F3: {
                    debug = !debug;
                    break;
                }
                case SDLK_UP: {
                    entity_set_velocity(world, player, UPDIR, 1);
                    break;
                }
                case SDLK_DOWN: {
                    entity_set_velocity(world, player, DOWNDIR, 1);
                    break;
                }
                case SDLK_LEFT: {
                    entity_set_velocity(world, player, LEFTDIR, 1);
                    break;
                }
                case SDLK_RIGHT: {
                    entity_set_velocity(world, player, RIGHTDIR, 1);
                    break;
                }
            }
        }
        if (event.type == SDL_KEYUP) {
            SDL_Keycode key = event.key.keysym.sym;
            switch (key) {
                case SDLK_UP: {
                    entity_stop_velocity(world, player, UPDIR);
                    break;
                }
                case SDLK_DOWN: {
                    entity_stop_velocity(world, player, DOWNDIR);
                    break;
                }
                case SDLK_LEFT: {
                    entity_stop_velocity(world, player, LEFTDIR);
                    break;
                }
                case SDLK_RIGHT: {
                    entity_stop_velocity(world, player, RIGHTDIR);
                    break;
                }
            }
        }
    }
}

void update() {
    entitys_move(world);
}

void render() {
    // Prepare all the sprites to render
    entitys_render(world, screen);

    // Render and clear
    graphics_show_and_clear(screen);
}

int main (void) {
    debug = false;
    world = world_create();
    graphics_open_window("Tales of Barathra", 1200, 780);
    img_t *knights = graphics_load_image("assets/knight_factions_1.png", 26, 36, 4, 3, 4);
    img_t *ground = graphics_load_image("assets/grounds.png", 16, 16, 1, 18, 2);

    player = entity_create(world, 20, 40, 1, PLAYER);
    entity_set_view(world, player, knights, 0);
    entity_set_solid(world, player, true);
    entity_set_size(world, player, 12, 6);

    int knight = entity_create(world, 300, 200, 1, COMPLEX_IA);
    entity_set_view(world, knight, knights, 3);
    entity_set_solid(world, knight, true);
    entity_set_size(world, knight, 12, 6);

    knight = entity_create(world, 300, 150, 1, COMPLEX_IA);
    entity_set_view(world, knight, knights, 1);
    entity_set_solid(world, knight, true);
    entity_set_size(world, knight, 12, 6);

    entity_set_velocity(world, 1, LEFTDIR, 0.5);

    int tile = entity_create(world, 15, 30, 0, TILE);
    entity_set_view_tile(world, tile, ground, 0, 0);
    tile = entity_create(world, 47, 30, 0, TILE);
    entity_set_view_tile(world, tile, ground, 1, 0);
    tile = entity_create(world, 79, 30, 0, TILE);
    entity_set_view_tile(world, tile, ground, 2, 0);
    tile = entity_create(world, 111, 30, 0, TILE);
    entity_set_view_tile(world, tile, ground, 3, 0);

    double lag = 0.0;
    done = false;
    double previous = SDL_GetTicks();
    while (!done) {
        double current = SDL_GetTicks();
        double elapsed = current - previous;
        previous = current;
        lag += elapsed;

        get_input();

        while (lag >= MS_PER_UPDATE) {
            update();
            lag -= MS_PER_UPDATE;
        }

        render();
    }

    graphics_close_texture(knights);
    graphics_close_window(screen);
    return 0;
}
