#include "../header.h"

u32 white;
u32 gray;

void start()
{
    white = pixel_pack(220, 220, 220, 255);
    gray = pixel_pack(128, 130, 142, 255);
}

enum MenuTab
{
    SPRITE,
    MAP,
    AUDIO
};

const u8 MENU_BAR_HEIGHT = HEIGHT / 3;
const u8 MENU_BAR_WIDTH = 20;
const u8 GRID_SIZE = 4;

void render_menu_bar()
{
}

void render_sprite_editor()
{
    fill_rect(WIDTH - 128 - 10, (HEIGHT - 128) >> 1, 128, 128, white);
    // render gray grid
    for (u8 i = 0; i < 128; i += GRID_SIZE)
    {
        for (u8 j = 0; j < 128; j += GRID_SIZE)
        {
            fill_rect(WIDTH - 128 - 10 + i, ((HEIGHT - 128) >> 1) + j, GRID_SIZE, GRID_SIZE, gray);
        }
    }
}

void update(float dt)
{
    u32 pixel = pixel_pack(128, 130, 142, 255);
    screen_clear(pixel);

    render_menu_bar();
    render_sprite_editor();

    game_instance *ins = instance_get();
    fill_rect(ins->mouse[0], ins->mouse[1], 2, 2, pixel_pack(120, 120, 220, 255));
}