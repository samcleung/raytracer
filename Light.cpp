#include <iostream>
#include <glm/ext.hpp>

#include "Light.hpp"

Light::Light()
  : colour(0.0, 0.0, 0.0),
	position(0.0, 0.0, 0.0)
{
  falloff[0] = 1.0;
  falloff[1] = 0.0;
  falloff[2] = 0.0;
}

AreaLight::AreaLight(
  const glm::vec3& corner,
  const glm::vec3& a, const glm::vec3& b,
  const double sz)
  : Light(),
	area(corner, a, b, sz)
{
  position = corner;
}

// Get center point of the cell
glm::vec3 AreaLight::get_sample_position() const
{
  auto sample_position = position
		+ area.get_b1() * (area.get_size() * rand_double(0.0, 1.0))
		+ area.get_b2() * (area.get_size() * rand_double(0.0, 1.0));
  // std::cout << sample_position << std::endl;
  return sample_position;
}

std::ostream& operator<<(std::ostream& out, const Light& l)
{
  out << "L[" << glm::to_string(l.colour) 
  	  << ", " << glm::to_string(l.position) << ", ";
  for (int i = 0; i < 3; i++) {
	if (i > 0) out << ", ";
	out << l.falloff[i];
  }
  out << "]";
  return out;
}
