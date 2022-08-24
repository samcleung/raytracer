#include "PhongMaterial.hpp"

PhongMaterial::PhongMaterial(
	const glm::vec3& kd,
	const glm::vec3& ks,
	double shininess,
	double reflectivity,
	double refractivity,
	double refractive_index,
	double glossiness )
	: m_kd(kd)
	, m_ks(ks)
	, m_shininess(shininess)
	, m_reflectivity(reflectivity)
	, m_refractivity(refractivity)
	, m_refractive_index(refractive_index == 0.0 ? 1.000293 : refractive_index)
	, m_glossiness(glossiness)
{
	if ( m_reflectivity + m_refractivity > 1.0 )
	{
		m_reflectivity = m_refractivity = 0.5;
	}
	m_refractive_index = glm::max(m_refractive_index, 0.0); // Check non-zero
}

PhongMaterial::~PhongMaterial()
{}
