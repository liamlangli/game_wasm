#include "../header.h"

void fill_round__rect(u8 x, u8 y, u8 w, u8 h, u8 r, u32 pixel) {
    u32* video_memory = instance_get()->video_memory;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (i < r || i >= h - r || j < r || j >= w - r) {
                video_memory[(y + i) * WIDTH + x + j] = pixel;
            }
        }
    }
}

void stroke_round_rect(u8 x, u8 y, u8 w, u8 h, u8 r, u8 line_width, u32 pixel) {
    u8 lw = line_width;
    u32* video_memory = instance_get()->video_memory;
    u8 hori = w - r * 2;
    for (u8 i = 0; i < hori; i++) {
        for (u8 j = 0; j < lw; ++j) {
            video_memory[(y + j) * WIDTH + x + r + i] = pixel;
            video_memory[(y + h - j - 1) * WIDTH + x + r + i] = pixel;
        }
    }

}

void start() {

}

enum MenuTab {
    SPRITE,
    MAP,
    AUDIO
};

const u8 MENU_BAR_HEIGHT = HEIGHT / 3;
const u8 MENU_BAR_WIDTH = 20;

void render_menu_bar() {
    // rect_color_set(0, 0, MENU_BAR_WIDTH, MENU_BAR_HEIGHT, pixel_pack(255, 255, 255, 255));
    stroke_round_rect(0, 0, MENU_BAR_WIDTH, MENU_BAR_HEIGHT, 5, 1, pixel_pack(0, 0, 0, 255));
    rect_color_set(0, MENU_BAR_HEIGHT, MENU_BAR_WIDTH, MENU_BAR_HEIGHT, pixel_pack(210, 210, 210, 255));
    rect_color_set(0, MENU_BAR_HEIGHT * 2, MENU_BAR_WIDTH, MENU_BAR_HEIGHT, pixel_pack(140, 140, 140, 255));
}

void render_sprite_editor() {

}

void update(float dt) {
    u32 pixel = pixel_pack(128, 130, 142, 255);
    screen_clear(pixel);

    render_menu_bar();

    rect_color_set(WIDTH - 128 - 10, (HEIGHT - 128) >> 1, 128, 128, pixel_pack(255, 255, 255, 255));

    game_instance* ins = instance_get();
    rect_color_set(ins->mouse[0], ins->mouse[1], 2, 2, pixel_pack(255, 0, 0, 255));
}