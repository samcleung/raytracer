#pragma once

#include <iosfwd>
#include <glm/glm.hpp>

#include "Primitive.hpp"

#include "utils.hpp"

// Represents a simple point light.
class Light {
public:
  Light();
  inline virtual glm::vec3 get_sample_position() const { return position; }
  
  glm::vec3 colour;
  glm::vec3 position;
  double falloff[3];
};

// Represent an area light defined by a plane with a corner position, two bases, and a size
class AreaLight : public Light {
public:
  AreaLight(const glm::vec3& corner, const glm::vec3& a, const glm::vec3& b, double sz);

  virtual glm::vec3 get_sample_position() const override;

  const Plane area;
};

std::ostream& operator<<(std::ostream& out, const Light& l);
