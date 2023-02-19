#include "../header.h"

void start() {

}

const u8 MENU_BAR_HEIGHT = 12;

void render_menu_bar() {
    rect_color_set(0, 0, WIDTH, MENU_BAR_HEIGHT, pixel_pack(234, 255, 234, 255));
}

void update(float dt) {
    u32 pixel = pixel_pack(128, 130, 142, 255);
    screen_clear(pixel);

    render_menu_bar();

    rect_color_set(WIDTH - 128 - 10, (HEIGHT - 128) >> 1, 128, 128, pixel_pack(255, 255, 255, 255));

    game_instance* ins = instance_get();
    rect_color_set(ins->mouse[0], ins->mouse[1], 2, 2, pixel_pack(255, 0, 0, 255));
}