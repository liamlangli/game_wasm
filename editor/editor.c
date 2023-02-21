#include "../header.h"
#include "../math.h"

const u8 MENU_BAR_HEIGHT = HEIGHT / 3;
const u8 MENU_BAR_WIDTH = 20;
const u8 GRID_SIZE = 4;
const u8 BORDER_WIDTH = 1;
const u8 SPRITE_VIEW_SIZE = 128;
const u8 SLIDER_HEIGHT = 24;
const u8 SLIDER_WIDTH = 4;

u8 rect_size_shift = 3;

static u32 gray;
static u32 white;
static u32 lighter;
static u32 darker;
static u32 black;

void start()
{
    gray = pixel_pack(168, 168, 168, 255);
    white = pixel_pack(220, 220, 220, 255);
    lighter = pixel_pack(230, 230, 230, 255);
    darker = pixel_pack(108, 108, 108, 255);
    black = pixel_pack(60, 60, 60, 255);
}

enum MenuTab
{
    SPRITE,
    MAP,
    AUDIO
};

void render_sprite_view() {
    u8 grid_x = WIDTH - SPRITE_VIEW_SIZE - 10;
    u8 grid_y = HEIGHT - SPRITE_VIEW_SIZE - 10;
    fill_rect(grid_x, grid_y, SPRITE_VIEW_SIZE, SPRITE_VIEW_SIZE, white);
    // render gray grid
    for (u8 j = 0; j < SPRITE_VIEW_SIZE; j += GRID_SIZE)
    {
        for (u8 i = ((j / GRID_SIZE) & 1) ? GRID_SIZE : 0; i < SPRITE_VIEW_SIZE; i += GRID_SIZE << 1)
        {
            fill_rect(grid_x + i, grid_y + j, GRID_SIZE, GRID_SIZE, gray);
        }
    }

    game_instance* ins = instance_get();
    u8 mouse_x = ins->mouse[0];
    u8 mouse_y = ins->mouse[1];

    u8 rect_shift = rect_size_shift + 2;
    if (mouse_x > grid_x && mouse_x < grid_x + SPRITE_VIEW_SIZE && mouse_y > grid_y && mouse_y < grid_y + SPRITE_VIEW_SIZE) {
        u8 rect_x = (mouse_x - grid_x) >> rect_shift;
        u8 rect_y = (mouse_y - grid_y) >> rect_shift;
        stroke_rect(grid_x + (rect_x << rect_shift), grid_y + (rect_y << rect_shift), 1 << rect_shift, 1 << rect_shift, 1, black);

    }

    // up border
    fill_rect(grid_x, grid_y - BORDER_WIDTH, SPRITE_VIEW_SIZE, BORDER_WIDTH, darker);
    // left border
    fill_rect(grid_x - BORDER_WIDTH, grid_y, BORDER_WIDTH, SPRITE_VIEW_SIZE, darker);
    // right border
    fill_rect(grid_x + SPRITE_VIEW_SIZE, grid_y, BORDER_WIDTH, SPRITE_VIEW_SIZE, lighter);
    // bottom border
    fill_rect(grid_x, grid_y + SPRITE_VIEW_SIZE, SPRITE_VIEW_SIZE, BORDER_WIDTH, lighter);
}


void render_size_slider() {
    u8 mouse_x = instance_get()->mouse[0];
    u8 mouse_y = instance_get()->mouse[1];

    u8 slider_x = WIDTH - SPRITE_VIEW_SIZE - 20;
    u8 slider_y = (HEIGHT - SLIDER_HEIGHT) >> 1;

    fill_rect(slider_x, slider_y, SLIDER_WIDTH, SLIDER_HEIGHT, gray);

    // up border
    fill_rect(slider_x, slider_y - BORDER_WIDTH, SLIDER_WIDTH, BORDER_WIDTH, darker);
    // left border
    fill_rect(slider_x - BORDER_WIDTH, slider_y, BORDER_WIDTH, SLIDER_HEIGHT, darker);
    // right border
    fill_rect(slider_x + SLIDER_WIDTH, slider_y, BORDER_WIDTH, SLIDER_HEIGHT, lighter);
    // bottom border
    fill_rect(slider_x, slider_y + SLIDER_HEIGHT, SLIDER_WIDTH, BORDER_WIDTH, lighter);

    fill_rect(slider_x, slider_y + rect_size_shift * SLIDER_WIDTH, SLIDER_WIDTH, SLIDER_WIDTH, black);

    rect r = {slider_x, slider_y, SLIDER_WIDTH, SLIDER_HEIGHT};

    game_instance* ins = instance_get();
    if (rect_contains(r, mouse_x, mouse_y) && ins->mouse_btn & MOUSE_LEFT) {
        rect_size_shift = (mouse_y - slider_y) >> 2;
    }
}

void render_tab(u8 x, u8 y, u8 w, u8 h, u32 color) {
}

void update(float dt)
{
    u32 pixel = pixel_pack(52, 62, 74, 255);
    screen_clear(pixel);

    render_sprite_view();
    render_size_slider();

    game_instance *ins = instance_get();
    fill_rect(ins->mouse[0], ins->mouse[1], 2, 2, pixel_pack(120, 120, 220, 255));
}