#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

namespace hash {

    float random() {
        return rand() / float(RAND_MAX);
    }

    float clamp(float x, float min, float max) {
        return std::min(max, std::max(min, x));
    }

    int hash(int x, int y) {
        return x * 73856093 ^ y * 19349663;
    }

    int id(int x, int y, float width) {
        int x_id = floor(x / width);
        int y_id = floor(y / width);
        return hash(x_id, y_id);
    }
}
