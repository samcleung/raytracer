#include "Perlin.hpp"

/* Function to linearly interpolate between a0 and a1
 * Weight w should be in the range [0.0, 1.0]
 */
float Perlin::interpolate(float a0, float a1, float w) const {
    /* // You may want clamping by inserting:
     * if (0.0 > w) return a0;
     * if (1.0 < w) return a1;
     */

    // Use [[Smootherstep]] for an even smoother result with a second derivative equal to zero on boundaries:
    return (a1 - a0) * ((w * (w * 6.0 - 15.0) + 10.0) * w * w * w) + a0;
}

/* Create pseudorandom direction vector
 */
glm::vec2 Perlin::get_random_gradient(const glm::ivec2& v) const {
    // No precomputed gradients mean this works for any number of grid coordinates
    constexpr unsigned w = 8 * sizeof(unsigned);
    constexpr unsigned s = w / 2; // rotation width
    unsigned a = v.x;
    unsigned b = v.y;
    a *= 3284157443;
    b ^= a << s | a >> w-s;
    b *= 1911520717;
    a ^= b << s | b >> w-s;
    a *= 2048419325;
    float gradient = a * (M_PI / ~(~0u >> 1)); // in [0, 2*Pi]
    glm::vec2 v2(glm::cos(gradient), glm::sin(gradient));
    return v2;
}

// Compute Perlin noise at coordinates x, y
float Perlin::operator()(const glm::vec2& v) const {
    // Determine grid cell coordinates
    const glm::ivec2 v00 = glm::floor(v);
    const glm::ivec2 v11 = v00 + glm::ivec2(1);
    const glm::ivec2 v10 = v00 + glm::ivec2(1,0);
    const glm::ivec2 v01 = v00 + glm::ivec2(0,1);

    // Determine interpolation weights
    // Could also use higher order polynomial/s-curve here
    const glm::vec2 s = v - glm::vec2(v00);

    return interpolate(
        interpolate(
            glm::dot(v - glm::vec2(v00), get_random_gradient(v00)),
            glm::dot(v - glm::vec2(v10), get_random_gradient(v10)),
            s.x
        ),
        interpolate(
            glm::dot(v - glm::vec2(v01), get_random_gradient(v01)),
            glm::dot(v - glm::vec2(v11), get_random_gradient(v11)),
            s.x
        ),
        s.y
    );
}