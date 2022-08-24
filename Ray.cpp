#include "Ray.hpp"

using namespace glm;

int Ray::next_id = 0;

Ray::Ray( const glm::vec3& eye, const glm::vec3& eye_pixel )
	: origin( eye ), direction( eye_pixel ), id(next_id++) {
}

glm::vec3 Ray::at( float t )
{
	return origin + t * direction;
}