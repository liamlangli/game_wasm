// union game chip is
#include "header.h"

static game_instance instance;
static u8 heap[65535];
static u16 sprites[16384];
static u16 tiles[16384];

int width() { return WIDTH; }
int height() { return HEIGHT; }
int version() { return VERSION; }

void init() {}

game_instance* instance_get() {
    return &instance;
}

u64 video_memory_address() {
    return (u64)&instance.video_memory;
}

u16* video_memory_pointer() {
    return (u16*)&instance.video_memory;
}

u64 heap_memory_address() {
    return (u64)&heap;
}

void* heap_memory_pointer() {
    return heap;
}

void screen_clear(u16 color) {
    for (int i = 0; i < FRAME_BUFFER_SIZE; i++) {
        instance.video_memory[i] = color;
    }
}

void pixel_color_set(u8 x, u8 y, u16 color) {
    instance.video_memory[y * WIDTH + x] = color;
}

void rect_color_set(u8 x, u8 y, u8 w, u8 h, u16 color) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            instance.video_memory[(y + i) * WIDTH + x + j] = color;
        }
    }
}

void rect_color_stroke(u8 x, u8 y, u8 w, u8 h, u8 line_width, u16 color) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (i < line_width || i >= h - line_width || j < line_width || j >= w - line_width) {
                instance.video_memory[(y + i) * WIDTH + x + j] = color;
            }
        }
    }
}

// void sprite_draw(u8 x, u8 y, u8 sprite_index, u8 size, u8 flip) {
//     u16* sprite = &sprites[sprite_index * size * size];
//     for (int i = 0; i < size; i++) {
//         for (int j = 0; j < size; j++) {
//             if (flip) {
//                 instance.video_memory[(y + i) * WIDTH + x + size - j - 1] = sprite[i * size + j];
//             } else {
//                 instance.video_memory[(y + i) * WIDTH + x + j] = sprite[i * size + j];
//             }
//         }
//     }
    
// }

// 5bit red, 5bit green, 5bit blue
u16 color_pack(u8 r, u8 g, u8 b) {
    return (r << 10) | (g << 5) | b;
}

void update_trigger(f32 l, f32 r) {
    instance.trigger[0] = l;
    instance.trigger[1] = r;
}

void update_mouse(u8 x, u8 y) {
    instance.mouse[0] = x;
    instance.mouse[1] = y;
}

void update_button(u16 buttons) {
    instance.buttons = buttons;
}

