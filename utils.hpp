// Global utility functions to use throughout code

#pragma once

#include <random>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

inline std::ostream& operator<<(std::ostream& out, const glm::vec2& v)
{
	return std::cout << '(' << v.x << ", " << v.y << ')';
}

inline std::ostream& operator<<(std::ostream& out, const glm::vec3& v)
{
	return std::cout << '(' << v.x << ", " << v.y << ", " << v.z << ')';
}

inline float rand_float( float min, float max )
{
	static std::uniform_real_distribution< float > dist( 0.0, 1.0 );
	static std::mt19937 gen;
	float rand_real = dist( gen );
	return min + ( max - min )*rand_real;
}

inline double rand_double( double min, double max )
{
	static std::uniform_real_distribution< double > dist( 0.0, 1.0 );
	static std::mt19937 gen;
	double rand_real = dist( gen );
	return min + ( max - min )*rand_real;
}

inline int rand_int( double min, double max )
{
	return static_cast< int >( rand_double( min, max+1 ) );
}

inline glm::vec3 random_in_unit_disk()
{
	while (true) {
		const glm::vec3 p( rand_double( -1, 1 ), rand_double( -1, 1 ), 0 );
		if ( glm::length2(p) >= 1 ) continue;
		return p;
	}
}

// Schlick approximation for reflectivity
inline float get_reflectivity(double n1, double n2, glm::vec3 incident, glm::vec3 normal)
{
	float r0 = glm::pow( (n1 - n2)/(n1 + n2), 2 );
	float cos_theta = -glm::dot(normal, incident);
	if ( n1 > n2 )
	{
		float n = n1 / n2;
	}
	return r0 + (1 - r0) * glm::pow(1 - cos_theta, 5);
}

template <typename... Rest>
inline double min(double first, Rest... rest)
{
	return glm::min(first, min(rest...));
}

template <>
inline double min(double single)
{
	return single;
}