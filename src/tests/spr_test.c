int main(void) {
    screen_t *screen = graphics_open_window("Tales of Barathra", 800, 600);
    img_t *knights = graphics_load_image(screen, "assets/knight_factions_1.png", 26, 36, 3);

    spr_t *knight1 = sprite_create(knights, 1);
    spr_t *knight2 = sprite_create(knights, 2);
    sprite_animate(knight2);

    bool done = false;
	while (!done) {
		SDL_Event event;
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				done = true;
				break;
			}
            if (event.type == SDL_KEYDOWN) {
                SDL_Keycode key = event.key.keysym.sym;
                if (key == SDLK_ESCAPE) {
                    done = true;
                    break;
                }
                if (key == SDLK_UP) {
                    sprite_change_animation(knight1, BACKWARD);
                    sprite_animate(knight1);
                }

                if (key == SDLK_DOWN) {
                    sprite_change_animation(knight1, FORWARD);
                    sprite_animate(knight1);
                }

                if (key == SDLK_LEFT) {
                    sprite_change_animation(knight1, LEFT);
                    sprite_animate(knight1);
                }

                if (key == SDLK_RIGHT) {
                    sprite_change_animation(knight1, RIGHT);
                    sprite_animate(knight1);
                }
            } if (event.type == SDL_KEYUP) {
                sprite_stop_animation(knight1, 1);
                sprite_stop_animation(knight2, 1);
            }
        }

        sprite_render(screen, knight1, 100, 200);
        sprite_render(screen, knight1, 200, 200);
        sprite_render(screen, knight1, 300, 200);
        sprite_render(screen, knight2, 400, 200);
        sprite_render(screen, knight2, 500, 200);
        graphics_show_and_clear(screen);
    }

    graphics_close_texture(knights);
    graphics_close_window(screen);
    return 0;
}
