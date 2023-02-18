#include "../header.h"

u8 pos_x = 0;
u8 pos_y = 0;
u8 size_x = 0;
u8 size_y = 0;

void start() {
    game_instance* ins = instance_get();
    pos_x = WIDTH >> 1;
    pos_y = HEIGHT - 8;
    size_x = 8;
    size_y = 8;
}

#define TILE_SIZE 16
#define TILE_COUNT_X 15
#define TILE_COUNT_Y 10
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

u8 tiles[TILE_COUNT_Y][TILE_COUNT_X];

u8 clamp(u8 i, u8 b, u8 t) {
    return min(max(i, b), t);
}

f32 clampf(f32 i, f32 b, f32 t) {
    return min(max(i, b), t);
}

const u8 speed = 2;
const f32 gravity = 980.f;
f32 velocity = 0.f;

void update(f32 dt) {
    screen_clear(color_pack(2, 3, 2));
    for (u8 y = 0; y < TILE_COUNT_Y; y++) {
        for (u8 x = 0; x < TILE_COUNT_X; x++) {
            u8 tile = tiles[y][x];
            rect_color_stroke(x << 4, y << 4, TILE_SIZE, TILE_SIZE, 1, color_pack(8, 8, 8));
        }
    }

    game_instance* ins = instance_get();
    if ((ins->buttons & UP) && (pos_y == HEIGHT - 8)) {
        velocity = -980.f;
        size_x = 6;
        size_y = 10;
    }
    if (ins->buttons & LEFT) {
        pos_x -= speed;
    }
    if (ins->buttons & RIGHT) {
        pos_x += speed;
    }

    velocity += gravity * dt;
    velocity = clampf(velocity, -400.f, 400.f);
    pos_y += velocity * dt;

    pos_x = clamp(pos_x, 2, WIDTH - 8);
    pos_y = clamp(pos_y, 2, HEIGHT - 8);

    if (pos_y == HEIGHT - 8) {
        size_x = 8;
        size_y = 8;
    }

    rect_color_set(pos_x + ((8 - size_x)), pos_y, size_x, size_y, color_pack(31, 31, 31));
}