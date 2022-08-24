#pragma once

#include <glm/glm.hpp>

#include "Material.hpp"

// Reference: Fundamentals of Computer Graphics by Marschner and Shirley (4.4.3)
class HitRecord {
	public:
		glm::vec3 point; // point of intersection
		glm::vec3 normal; // surface normal at the hit point
		float t; // coordinate of hit point along ray
		Material* material = nullptr;

		glm::vec3 u; // u coordinate in world space
		glm::vec3 v; // v coordinate in world space
		glm::vec2 uv; // uv coordinates in texture/model space
};
