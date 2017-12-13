#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "globals.h"
#include "sprite.h"
#include "entity.h"

static int player;
static bool done;
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
                    entity_set_speed(player, UPDIR, 1);
                    break;
                }
                case SDLK_DOWN: {
                    entity_set_speed(player, DOWNDIR, 1);
                    break;
                }
                case SDLK_LEFT: {
                    entity_set_speed(player, LEFTDIR, 1);
                    break;
                }
                case SDLK_RIGHT: {
                    entity_set_speed(player, RIGHTDIR, 1);
                    break;
                }
            }
        }
        if (event.type == SDL_KEYUP) {
            SDL_Keycode key = event.key.keysym.sym;
            switch (key) {
                case SDLK_UP: {
                    entity_stop_speed(player, UPDIR);
                    break;
                }
                case SDLK_DOWN: {
                    entity_stop_speed(player, DOWNDIR);
                    break;
                }
                case SDLK_LEFT: {
                    entity_stop_speed(player, LEFTDIR);
                    break;
                }
                case SDLK_RIGHT: {
                    entity_stop_speed(player, RIGHTDIR);
                    break;
                }
            }
        }
    }
}

void update() {
    // Update all the timers
    entitys_timer();

    // Move all the entities
    entitys_move();

    // Animate all the entities
    entitys_animate();
}

void render() {
    // Prepare all the sprites to render
    entitys_render();

    // Render and clear
    graphics_show_and_clear();
}

int main (void) {
    debug = false;
    world_create();
    graphics_open_window("Tales of Barathra", 1200, 780);
    img_t *knights = graphics_load_image("assets/knight_factions_1.png", 26, 36, 4, 3, 4);
    img_t *ground = graphics_load_image("assets/grounds.png", 16, 16, 1, 18, 1);
    int anim_cycle[4] = {0, 1, 2, 1};

    player = entity_create(20, 40, 1, PLAYER);
    entity_set_view(player, knights, 0, 1, 1);
    entity_set_solid(player, true);
    entity_set_size(player, 12, 6);
    entity_init_animation(player, 1, anim_cycle, 30);

    int knight = entity_create(300, 200, 1, COMPLEX_IA);
    entity_set_view(knight, knights, 2, 1, 1);
    entity_set_solid(knight, true);
    entity_set_size(knight, 12, 6);
    entity_init_animation(knight, 1, anim_cycle, 30);

    knight = entity_create(300, 150, 1, COMPLEX_IA);
    entity_set_view(knight, knights, 1, 1, 1);
    entity_set_solid(knight, true);
    entity_set_size(knight, 12, 6);
    entity_init_animation(knight, 1, anim_cycle, 30);

    entity_set_speed(1, LEFTDIR, 0.5);

    int variations[25] = {0, 1, 2, 7};
    int tile = entity_create(15, 30, 0, TILE);
    entity_set_view(tile, ground, 0, 2, 2);
    entity_set_view_variations(tile, variations);
    entity_init_animation(tile, 1, anim_cycle, 30);
    entity_set_animation(tile, FORWARD);
    tile = entity_create(47, 30, 0, TILE);
    entity_set_view(tile, ground, 1, 2, 2);
    entity_set_view_variations(tile, variations);
    tile = entity_create(79, 30, 0, TILE);
    entity_set_view(tile, ground, 2, 2, 2);
    entity_set_view_variations(tile, variations);
    tile = entity_create(111, 30, 0, TILE);
    entity_set_view(tile, ground, 3, 2, 2);
    entity_set_view_variations(tile, variations);

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
