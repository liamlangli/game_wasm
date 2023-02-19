// union game chip is
#include "header.h"

int width() { return WIDTH; }
int height() { return HEIGHT; }
int version() { return VERSION; }

static game_instance* instance = 0;

void init(u64 memory_base) {
    instance = (game_instance*)memory_base;
}

game_instance* instance_get() {
    return instance;
}

u64 video_memory_address() {
    return (u64)&instance->video_memory;
}

void screen_clear(u32 pixel) {
    u32* video_memory = instance->video_memory;
    for (int i = 0; i < FRAME_BUFFER_SIZE; i++) {
        video_memory[i] = pixel;
    }
}

void pixel_color_set(u8 x, u8 y, u32 color) {
    instance->video_memory[y * WIDTH + x] = color;
}

void fill_rect(u8 x, u8 y, u8 w, u8 h, u32 pixel) {
    u32* video_memory = instance->video_memory;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            video_memory[(y + i) * WIDTH + x + j] = pixel;
        }
    }
}

void stroke_rect(u8 x, u8 y, u8 w, u8 h, u8 line_width, u32 pixel) {
    u32* video_memory = instance->video_memory;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (i < line_width || i >= h - line_width || j < line_width || j >= w - line_width) {
                video_memory[(y + i) * WIDTH + x + j] = pixel;
            }
        }
    }
}

u32 pixel_pack(u8 r, u8 g, u8 b, u8 d) {
    return (r << 24) | (g << 16) | (b << 8) | d;
}

void update_trigger(f32 l, f32 r) {
    instance->trigger[0] = l;
    instance->trigger[1] = r;
}

void update_mouse(u8 x, u8 y) {
    instance->mouse[0] = x;
    instance->mouse[1] = y;
}

void update_button(u16 buttons) {
    instance->buttons = buttons;
}

