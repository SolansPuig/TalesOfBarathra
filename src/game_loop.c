#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "globals.h"
#include "sprite.h"
#include "entity.h"
#include "world.h"
#include "math.h"
#include "message.h"

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
                case SDLK_F4: {
                    reload(PROP);
                    break;
                }
                case SDLK_F5: {
                    reload(TILE);
                    break;
                }
                case SDLK_F6: {
                    reload(COMPLEX_IA);
                    break;
                }
                case SDLK_UP: {
                    message_send(-1, 6, MOVE, "UPDIR, 1");
                    break;
                }
                case SDLK_DOWN: {
                    message_send(-1, 6, MOVE, "DOWNDIR, 1");
                    break;
                }
                case SDLK_LEFT: {
                    message_send(-1, 6, MOVE, "LEFTDIR, 1");
                    break;
                }
                case SDLK_RIGHT: {
                    message_send(-1, 6, MOVE, "RIGHTDIR, 1");
                    break;
                }
            }
        }
        if (event.type == SDL_KEYUP) {
            SDL_Keycode key = event.key.keysym.sym;
            switch (key) {
                case SDLK_UP: {
                    message_send(-1, 6, STOP, "UPDIR");
                    break;
                }
                case SDLK_DOWN: {
                    message_send(-1, 6, STOP, "DOWNDIR");
                    break;
                }
                case SDLK_LEFT: {
                    message_send(-1, 6, STOP, "LEFTDIR");
                    break;
                }
                case SDLK_RIGHT: {
                    message_send(-1, 6, STOP, "RIGHTDIR");
                    break;
                }
            }
        }
    }
}

void update() {
    // Update all the timers
    entitys_timer();

    // Read all the messages
    entitys_read_messages();

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
    math_random_seed();
    debug = false;
    graphics_open_window("Tales of Barathra", 1200, 780);

    load(COMPLEX_IA);
    load(PROP);
    load(TILE);

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

    //graphics_close_texture(knights);
    graphics_close_window(screen);
    return 0;
}
