#include <SDL.h>
#pragma once

struct Color {
    uint8_t r, g, b, a;

    Color(Uint8 red = 0, Uint8 green = 0, Uint8 blue = 0, Uint8 alpha = 255)
            : r(red), g(green), b(blue), a(alpha) {}

    Color operator*(float scalar) const {
        Color result;
        result.r = static_cast<uint8_t>(r * scalar);
        result.g = static_cast<uint8_t>(g * scalar);
        result.b = static_cast<uint8_t>(b * scalar);
        result.a = static_cast<uint8_t>(a * scalar);
        return result;
    }


};