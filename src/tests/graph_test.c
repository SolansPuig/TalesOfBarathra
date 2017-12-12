int main() {
    screen_t *screen = graphics_open_window("Tales of Barathra", 800, 600);

    img_t *img = graphics_load_image(screen, "assets/knight_factions_1.png", 26, 36);
    graphics_render_texture(screen, img, 0, 1, 100, 50);
    graphics_render_texture(screen, img, 5, 1, 200, 50);
    SDL_Delay(3000);
    graphics_close_texture(img);
    graphics_close_window(screen);
}
