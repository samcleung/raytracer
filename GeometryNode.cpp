#include "GeometryNode.hpp"
#include <glm/gtx/string_cast.hpp>

//---------------------------------------------------------------------------------------
GeometryNode::GeometryNode(
	const std::string & name, Primitive *prim, Material *mat )
	: SceneNode( name )
	, m_material( mat )
	, m_primitive( prim )
{
	m_nodeType = NodeType::GeometryNode;
}

void GeometryNode::setMaterial( Material *mat )
{
	// Obviously, there's a potential memory leak here.  A good solution
	// would be to use some kind of reference counting, as in the 
	// C++ shared_ptr.  But I'm going to punt on that problem here.
	// Why?  Two reasons:
	// (a) In practice we expect the scene to be constructed exactly
	//	 once.  There's no reason to believe that materials will be
	//	 repeatedly overwritten in a GeometryNode.
	// (b) A ray tracer is a program in which you compute once, and 
	//	 throw away all your data.  A memory leak won't build up and
	//	 crash the program.

	m_material = mat;
}

//---------------------------------------------------------------------------------------

bool GeometryNode::hit(Ray& ray, HitRecord& rec, float t0, float t1)
{
	bool is_hit = false;
	HitRecord new_rec;
	float new_t1 = t1;
	// Re-express primary ray in local MCS to perform object intersection
	Ray model_ray = Ray(glm::vec3(invtrans * glm::vec4(ray.origin, 1.0)),
						glm::vec3(invtrans * glm::vec4(ray.direction, 0.0)));
	
	if ( m_primitive->hit( model_ray, new_rec, t0, t1 ) )
	{
		is_hit = true;
		if ( new_rec.material == nullptr ) new_rec.material = m_material; // this
		new_t1 = new_rec.t;
		rec = new_rec;
	}

	if ( is_hit )
	{
		// Perturb normals in primitive model space
		const glm::vec2 perturbation = m_material->get_bump_at(rec.uv);
		rec.normal += rec.u * perturbation.x + rec.v * perturbation.y;
		// Transform location, normal vector back to WCS
		rec.point = glm::vec3( trans * glm::vec4( rec.point, 1.0 ) );
		glm::mat3 mat_transpose( glm::transpose( invtrans ) );
		rec.normal = mat_transpose * rec.normal;
		rec.u = mat_transpose * rec.u;
		rec.v = mat_transpose * rec.v;
	}

	return is_hit;
}
