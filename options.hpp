#pragma once

// #define REFLECTIONS
// #define GLOSSY_REFLECTIONS
// #define REFRACTIONS
// #define GLOSSY_REFRACTIONS
// #define SOFT_SHADOWS
// #define ANTI_ALIASING
// #define BUMP_MAPPING
// #define AMBIENT_OCCLUSION

namespace rtc {
	constexpr double epsilon = 1e-4;
	constexpr unsigned int max_recursions = 4;
	constexpr unsigned int scatter_glossy = 1;
	constexpr unsigned int scatter_soft = 10;
	constexpr unsigned int supersamples = 2;
	constexpr int seed = 0;
}
