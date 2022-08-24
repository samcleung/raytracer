#pragma once

#include <glm/glm.hpp>

// Adapted from https://en.wikipedia.org/wiki/Perlin_noise
class Perlin {
public:
    float interpolate(float a0, float a1, float w) const;
    glm::vec2 get_random_gradient(const glm::ivec2& v) const;
    float operator()(const glm::vec2& v) const ;
};
