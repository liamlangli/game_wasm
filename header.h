#ifndef _HEADER_H_
#define _HEADER_H_

#define WIDTH 240
#define HEIGHT 160
#define SPRITE_BUFFER_WIDTH 128
#define SPRITE_BUFFER_SIZE 16384
#define FRAME_BUFFER_SIZE 38400
#define VERSION 1

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;
typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long i64;
typedef float f32;
typedef double f64;

enum Button {
    UP = 1,
    DOWN = 2,
    LEFT = 4,
    RIGHT = 8,
    A = 16,
    B = 32,
    X = 64,
    Y = 128,
    L = 256,
    R = 512,
    START = 1024,
    SELECT = 2048,
    FN = 4096,
};

typedef struct {
    u32 video_memory[FRAME_BUFFER_SIZE]; // [r, g, b, d] 8 bits per channel
    u16 buttons; // bit set
    u8 mouse[2];
    f32 trigger[2];
    f32 left_stick[2];
    f32 right_stick[2];
} game_instance;

/** ----- game api ----- */
void            init(u64 memory_base);

game_instance*  instance_get();

u64             video_memory_address();

void            update_trigger(f32 l, f32 r);
void            update_mouse(u8 x, u8 y);
void            update_button(u16 buttons);

u32             pixel_pack(u8 r, u8 g, u8 b, u8 d);
void            screen_clear(u32 pixel);
void            pixel_color_set(u8 x, u8 y, u32 color);
void            fill_rect(u8 x, u8 y, u8 w, u8 h, u32 pixel);
void            stroke_rect(u8 x, u8 y, u8 w, u8 h, u8 line_width, u32 pixel);

void            sprite_draw(u8 x, u8 y, u8 sprite_index, u8 size, u8 flip);
// void           palette_set(u16* palette, u8 start, u8 count);
// void            palette_copy(u16* palette, u8 start, u8 count);

/** ----- life cycle hook ----- */
void            start();
void            update(f32 delta_time);

#endif