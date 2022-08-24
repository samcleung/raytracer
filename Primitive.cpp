#include <iostream>
#include <mutex>

#include "Primitive.hpp"

#include "polyroots.hpp"
#include "options.hpp"
#include "utils.hpp"

Primitive::~Primitive()
{}

//---------------------------------------------------------------------------------------

Cube::~Cube()
{}

const Plane Cube::sides[6]
{
	Plane( glm::vec3(-1,-1, 1), glm::vec3( 2, 0, 0), glm::vec3(0, 2, 0), 1 ), // front
	Plane( glm::vec3( 1,-1,-1), glm::vec3(-2, 0, 0), glm::vec3(0, 2, 0), 1 ), // back
	Plane( glm::vec3(-1,-1,-1), glm::vec3( 0, 0, 2), glm::vec3(0, 2, 0), 1 ), // left
	Plane( glm::vec3( 1,-1, 1), glm::vec3( 0, 0,-2), glm::vec3(0, 2, 0), 1 ), // right
	Plane( glm::vec3(-1, 1, 1), glm::vec3( 2, 0, 0), glm::vec3(0, 0,-2), 1 ), // top
	Plane( glm::vec3(-1,-1,-1), glm::vec3( 2, 0, 0), glm::vec3(0, 0, 2), 1 )  // bottom
};

bool Cube::hit( Ray& ray, HitRecord& rec, float t0, float t1 ) const
{
	bool is_hit = false;
	for ( const Plane& side : sides )
	{
		HitRecord new_rec;
		if ( !side.hit( ray, new_rec, t0, t1 ) ) continue;
		if ( (!is_hit || new_rec.t < rec.t) && new_rec.t < t1 && new_rec.t > t0 && new_rec.uv == glm::clamp( new_rec.uv, glm::vec2( 0 ), glm::vec2( 1 )))
		{
			rec = new_rec;
			is_hit = true;
		}
	}
	return is_hit;
}

//---------------------------------------------------------------------------------------

Sphere::~Sphere()
{}

// Reference: Fundamentals of Computer Graphics by Marschner and Shirley (4.4.2)
bool Sphere::hit( Ray& ray, HitRecord& rec, float t0, float t1 ) const
{
	glm::vec3 l = ray.origin;

	float a = glm::dot( ray.direction, ray.direction );
	float b = 2 * glm::dot( ray.direction, l );
	float c = glm::dot( l, l ) - 1;

	float t = 0.0f;
	double roots[ 2 ];
	
	size_t num = quadraticRoots( a, b, c, roots );

	bool is_hit = false;
	for (int i = 0; i < num; ++i)
	{
		if (roots[i] > t0 && roots[i] < t1 && (!is_hit || roots[i] < t))
		{
			is_hit = true;
			t = roots[i];
		}
	}

	if (!is_hit) return false;

	rec.t = t;
	rec.point = ray.at( t );
	rec.normal = rec.point;

	// Spherical mapping
	// azimuthal : u, polar : v
	const float azimuthal = glm::atan(-rec.point.x, -rec.point.z);
	const float r_horizontal = sqrt(rec.point.x * rec.point.x + rec.point.z * rec.point.z);
	const float polar = glm::atan(r_horizontal, rec.point.y);
	rec.uv = glm::vec2(
		glm::mod(azimuthal / (2.0 * M_PI), 1.0),
		polar / M_PI);
	
	rec.u = glm::normalize(glm::vec3(rec.point.z, 0.0, -rec.point.x)); // normalize b/c don't want pinching
	rec.v = glm::cross(rec.u, rec.normal);

	return true;
}

//---------------------------------------------------------------------------------------

Cylinder::~Cylinder()
{}

const Plane Cylinder::caps[2]
{
	Plane( glm::vec3(-1, 1, 1), glm::vec3(2, 0, 0), glm::vec3(0, 0,-2), 1 ), // top
	Plane( glm::vec3(-1,-1,-1), glm::vec3(2, 0, 0), glm::vec3(0, 0, 2), 1 ) // bottom
};

bool Cylinder::hit( Ray& ray, HitRecord& rec, float t0, float t1 ) const
{
	bool is_hit = false;

	// Test infinite face hit
	glm::vec2 ray_direction_2d(ray.direction.x, ray.direction.z);
	glm::vec2 ray_origin_2d(ray.origin.x, ray.origin.z);

	float a = glm::dot(ray_direction_2d, ray_direction_2d);
	float b = 2 * glm::dot(ray_origin_2d, ray_direction_2d);
	float c = glm::dot(ray_origin_2d, ray_origin_2d) - 1;

	float t = t1;
	double roots[ 2 ];
	
	size_t num = quadraticRoots( a, b, c, roots );

	for (int i = 0; i < num; ++i)
	{
		if (roots[i] > t0 && roots[i] < t1 && (!is_hit || roots[i] < t) && glm::abs(ray.at(roots[i]).y) <= 1)
		{
			is_hit = true;
			t = roots[i];
		}
	}

	if (is_hit)
	{
		glm::vec3 point = ray.at(t);
		rec.t = t;
		rec.point = point;
		rec.normal = glm::vec3(rec.point.x, 0, rec.point.z);

		// Cylindrical mapping
		// azimuthal : u, height : h
		const float azimuthal = glm::atan(-rec.point.z, rec.point.x);
		rec.uv = glm::vec2(
			glm::mod(double(azimuthal), 2.0 * M_PI) / 2.0 / M_PI,
			(1 - rec.point.y) / 2);
		
		rec.u = glm::vec3(rec.point.z, 0, rec.point.x); // normalize b/c don't want pinching
		rec.v = glm::vec3(0, -1, 0);
	}

	// Test caps hit
	for ( const Plane& cap : caps )
	{
		HitRecord new_rec;
		if ( !cap.hit( ray, new_rec, t0, t1 ) ) continue;
		if ( ( !is_hit || new_rec.t < rec.t )
			&& new_rec.t > t0 && new_rec.t < t1
			&& glm::length(new_rec.uv - glm::vec2(0.5)) <= 0.5f) // Clamp to a circle
		{
			is_hit = true;
			rec = new_rec;
		}
	}

	return is_hit;
}

//---------------------------------------------------------------------------------------

Plane::Plane( const glm::vec3& origin, const glm::vec3& b1, const glm::vec3& b2, double size )
	: m_origin(origin)
	, m_b1(b1)
	, m_b2(b2)
	, m_norm(glm::normalize(glm::cross(b1, b2)))
	, m_size(size) // only used for area lights
{}

Plane::~Plane()
{}

bool Plane::hit( Ray& ray, HitRecord& rec, float t0, float t1 ) const
{
	const double D = -glm::dot( m_norm, m_origin ); // bias away from the origin
	double roots[ 2 ];
	
	size_t num = quadraticRoots( 0, glm::dot( ray.direction, m_norm )
							   , D + glm::dot( ray.origin, m_norm ), roots );

	if ( num > 0 && roots[ 0 ] > t0 && roots[ 0 ] < t1 ) rec.t = roots[ 0 ];
	else return false;
	
	rec.point = ray.at(rec.t);
	rec.normal = m_norm;
	rec.u = glm::normalize(m_b1);
	rec.v = -glm::normalize(m_b2);
	const glm::vec3 p = ray.at( rec.t ) - m_origin;
	const glm::mat3 A( m_b1, m_b2, m_norm );
	const glm::vec3 a = glm::inverse(A) * p;

	rec.uv = glm::vec2( a.x, 1 - a.y );
	return true;
}
