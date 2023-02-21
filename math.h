#ifndef _MATH_H_
#define _MATH_H_

#include "header.h"

#define PI 3.14159265358979323846
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

typedef struct
{
    u8 x, y, w, h;
} rect;

typedef struct 
{
    u8 x, y;
} point;

inline u8 rect_contains(rect r, u8 x, u8 y) {
    return x >= r.x && x < r.x + r.w && y >= r.y && y < r.y + r.h;
}

inline u8 rect_contains_point(rect r, point p) {
    return rect_contains(r, p.x, p.y);
}

inline point point_make(u8 x, u8 y) { return (point){.x = x, .y = y}; }

inline u8 clamp_u8(u8 x, u8 b, u8 t) {
    return max(b, min(x, t));
}

inline f32 clamp_f32(f32 x, f32 b, f32 t) {
    return max(b, min(x, t));
}

#endif