#pragma once

#include "SceneNode.hpp"
#include "Primitive.hpp"
#include "Material.hpp"

class GeometryNode : public SceneNode {
public:
	GeometryNode( const std::string & name, Primitive *prim, 
		Material *mat = nullptr );

	void setMaterial( Material *material );

	bool hit( Ray& ray, HitRecord& record, float t0, float t1 ) override;

	Material* m_material;
	Primitive* m_primitive;
};
