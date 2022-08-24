// Termm--Fall 2020

#pragma once

#include <glm/glm.hpp>

#include "Material.hpp"

class PhongMaterial : public Material {
public:
  PhongMaterial(const glm::vec3& kd, const glm::vec3& ks, double shininess,
	double reflectivity = 0.0, double refractivity = 0.0, double refractive_index = 1.0,
	double glossiness = 0.0 );
  virtual ~PhongMaterial();

  glm::vec3 diffuse() { return m_kd; }
  glm::vec3 specular() { return m_ks; }
  
  double shininess() { return m_shininess; }
  double reflectivity() { return m_reflectivity; }
  double refractivity() { return m_refractivity; }
  double refractive_index() { return m_refractive_index; }
  double glossiness() { return m_glossiness; }
private:
  glm::vec3 m_kd;
  glm::vec3 m_ks;

  double m_shininess;
  double m_reflectivity;
  double m_refractivity;
  double m_refractive_index;
  double m_glossiness;
};
