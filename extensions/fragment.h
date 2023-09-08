#include "glm/glm.hpp"
#pragma once

struct Fragment {
    glm::ivec2 position; // X and Y coordinates of the pixel (in screen space)
    Color color;
    float z;
};