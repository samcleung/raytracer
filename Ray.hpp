#pragma once

#include <glm/glm.hpp>

// A ray object.
class Ray {
  static int next_id;
public:
  Ray( const glm::vec3& eye, const glm::vec3& eye_pixel );

  glm::vec3 at( float t );

	glm::vec3 origin;
	glm::vec3 direction;
  const int id;
};
