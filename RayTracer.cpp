#include <glm/ext.hpp>

#include "RayTracer.hpp"

using namespace std;
using namespace glm;

// Multi-threading report
void report()
{
	static int n = 0;
	static std::mutex mx;
	mx.lock();
	n++;
	std::cout << "jobs completed: " << n << std::endl;
	mx.unlock();
}

// Trace rays through hierarchy of SceneNodes
glm::vec3 get_ray_color(
	Ray& ray,
	SceneNode* root,
	const glm::vec3& ambient,
	const std::list< Light* >& lights,
	const unsigned int max_recursions = rtc::max_recursions, // number of reflections/refractions through materials
	const bool is_inside_obj = false // for refraction
) {
	HitRecord rec;
	glm::vec3 color( 0.0 );

	// Check ray intersections
	if ( root->hit( ray, rec, rtc::epsilon, std::numeric_limits< float >::infinity() ) )
	{
		rec.normal = glm::normalize( rec.normal );

		PhongMaterial* material2 = dynamic_cast< PhongMaterial* >( rec.material );
		if (material2 == nullptr) {
			std::cerr << "missing material!" << std::endl;
			return glm::vec3(1,0,1);
		}

		// Add colour from ambient
		color += material2->diffuse() * ambient;

		// Shadows
		for ( Light* light : lights ) {
			Ray ray_shadow( rec.point, light->position - rec.point );
			HitRecord rec_shadow;

			if ( root->hit( ray_shadow, rec_shadow, rtc::epsilon, 1 ) ) continue;

			float soft_shadow_coef = 1;
#ifdef SOFT_SHADOWS
			// Sample random points on area light
			AreaLight* area_light = dynamic_cast<AreaLight*>(light);
			if (area_light != nullptr)
			{
				unsigned int hits = 0;
				for (uint i = 1; i < rtc::scatter_soft; ++i) {
					glm::vec3 sample_light_position(area_light->get_sample_position());
					Ray ray_soft_shadow(rec.point, sample_light_position - rec.point);
					HitRecord rec_soft_shadow;
					if ( root->hit( ray_soft_shadow, rec_soft_shadow, rtc::epsilon, 1) ) ++hits; 
				}
				soft_shadow_coef = (rtc::scatter_soft - hits) / rtc::scatter_soft;
			}
#endif
			if ( !is_inside_obj ) // don't produce shadows for rays inside objects
			{
				glm::vec3 l = glm::normalize( ray_shadow.direction );
				glm::vec3 r = glm::normalize( 2 * rec.normal * glm::dot( rec.normal, l ) - l );
				glm::vec3 v = -glm::normalize( ray.direction );
				double rad = glm::length( ray_shadow.direction );

				double attenuation = 1.0 / ( light->falloff[ 0 ] + light->falloff[ 1 ] * rad +
					light->falloff[ 2 ] * rad * rad );
				
				color += glm::dot( rec.normal, l ) * attenuation * material2->diffuse() * light->colour * soft_shadow_coef;
				color += glm::pow( glm::max(0.0, (double)dot( r, v )), material2->shininess() )
						* attenuation * material2->specular() * light->colour * soft_shadow_coef;
			}
		}
			
		if (max_recursions < 1) return color; // No more reflections/refractions to account for

		float reflectivity = 0.0f;
		float refractivity = 0.0f;
		
		double glossiness = material2->glossiness();
		ray.direction = glm::normalize(ray.direction);
		glm::vec3 reflect_color(0.0);
		glm::vec3 refract_color(0.0);
#ifdef REFLECTIONS
		reflectivity = material2->reflectivity();
		glm::vec3 reflection = glm::reflect(ray.direction, rec.normal); // ray.direction - 2*rec.normal * glm::dot( ray.direction, rec.normal );
		Ray reflect_ray( rec.point, reflection );
#ifdef GLOSSY_REFLECTIONS // No glossy reflections without specular reflections
		reflection = glm::normalize( reflection );
		glm::vec3 u_v(glm::cross(reflection, rec.normal));
		glm::vec3 v_v(glm::cross(u_v, reflection));
		for (uint i = 0; i < rtc::scatter_glossy; ++i) {
			float u = -glossiness/2 + rand_float(0, 1)*glossiness;
			float v = -glossiness/2 + rand_float(0, 1)*glossiness;
			glm::vec3 glossy_reflection = reflection + u*u_v + v*v_v;
			glossy_reflection = glm::normalize( glossy_reflection );
			Ray ray_glossy_reflection(rec.point, glossy_reflection);
			reflect_color += get_ray_color(ray_glossy_reflection, root, ambient, lights, 0, is_inside_obj);
		}
		reflect_color /= rtc::scatter_glossy; // Average the colour of the glossy reflections
#else
		reflect_color = get_ray_color( reflect_ray, root, ambient, lights, max_recursions - 1, is_inside_obj );
#endif
#endif
#ifdef REFRACTIONS
		refractivity = material2->refractivity();
		float eta = 1 / material2->refractive_index();
		if ( is_inside_obj ) eta = 1/eta; // Check inside or outside
		glm::vec3 refraction = glm::refract(ray.direction, rec.normal * -glm::sign(glm::dot(ray.direction, rec.normal)), eta);
		if (refraction == glm::vec3(0.0) || glm::any(isnan(refraction))) // check for total internal reflection
		{
			reflectivity += refractivity;
			refractivity = 0.0f;
		}
		else
		{
			glm::vec3 refraction_origin(rec.point);
			Ray ray_refraction(refraction_origin, refraction);
#ifdef GLOSSY_REFRACTIONS
			refraction = glm::normalize(refraction);
			glm::vec3 u_v(glm::cross(refraction, -rec.normal));
			glm::vec3 v_v(glm::cross(u_v, refraction));
			for (uint i = 0; i < rtc::scatter_glossy; ++i) {
				float u = -glossiness/2 + rand_float(0, 1)*glossiness;
				float v = -glossiness/2 + rand_float(0, 1)*glossiness;
				glm::vec3 glossy_refraction = refraction + u*u_v + v*v_v;
				glossy_refraction = glm::normalize( glossy_refraction );
				Ray ray_glossy_refraction(refraction_origin, glossy_refraction);
				refract_color += get_ray_color(ray_glossy_refraction, root, ambient, lights, max_recursions - 1, is_inside_obj);
			}
			refract_color /= rtc::scatter_glossy;
#else
			refract_color = get_ray_color(ray_refraction, root, ambient, lights, max_recursions - 1, !is_inside_obj);
#endif
		}
#endif
		color = (1 - reflectivity - refractivity) * color
				+ reflectivity * reflect_color
				+ refractivity * refract_color;

		// Occlude
		color *= (1 - material2->get_occlusion_at(rec.uv));
	}
	else // Show BG gradient
	{
		float t( 0.5 * ( glm::normalize( ray.direction ).y + 1.0 ));
		color += ( 1.0 - t ) * glm::vec3( 1.0, 1.0, 1.0 )
				+ t * glm::vec3( 0.7, 0.5, 0.9 );
	}

	return color;
}

void Job::operator()() {
	size_t h = resources->image.height();
	size_t w = resources->image.width();
	auto theta = glm::radians( resources->fovy );

	const glm::vec3 v = glm::normalize( resources->view ); // w
	const glm::vec3 u = glm::normalize( resources->up ); // v
	const glm::vec3 right = glm::cross( v, u ); // u
	const double aspect = w / h;
	const double img_plane_h = 2 * glm::tan( theta / 2);
	const double img_plane_w = aspect * img_plane_h;
	const double pixel_w = img_plane_h / h;

	const glm::vec3 top_left = resources->eye + v -
					( img_plane_w/2 - pixel_w/2 )*right +
					( img_plane_h/2 - pixel_w/2 )*u;

	for (uint j = start.y; j < std::min(size+start.y, uint(h)); ++j) {
		for (uint i = start.x; i < std::min(size+start.x, uint(w)); ++i) {
			glm::vec3 color( 0, 0, 0 );
			for (uint k = 0; k < resources->ss; ++k) {
				for (uint l = 0; l < resources->ss; ++l) {
					// std::cerr << "\rScanlines remaining: " << h-j << ' ' << std::flush;
					// if ( j != 256 || i != 512 ) continue;
					glm::vec3 top_left_pixel = top_left - pixel_w * ( j*u - i*right );
					glm::vec2 off_subpixel( ( k - 0.5 )/resources->ss, ( l - 0.5 )/resources->ss );
					glm::vec3 ray_origin = resources->eye;
					glm::vec3 ray_direction = top_left_pixel
						- pixel_w / resources->ss * ( off_subpixel.x*u - off_subpixel.y*right )
						- resources->eye;

					Ray ray( ray_origin, ray_direction );
					color += get_ray_color( ray, resources->root, resources->ambient, resources->lights );
				}
			}
			color /= ( resources->ss * resources->ss );
			// Red: 
			resources->image(i, j, 0) = (double)color.r;
			// Green: 
			resources->image(i, j, 1) = (double)color.g;
			// Blue: 
			resources->image(i, j, 2) = (double)color.b;
		}
	}

	// report();
}

void RayTracer::disco_update()
{
	if (m_shared.eye.x != 0 || m_shared.eye.y != 0) return;
	if (m_shared.eye.z < 0) // not at origin yet
	{
		m_shared.eye.z += glm::smoothstep(0.f, m_end_time, Time::get_time());
	}
}

RayTracer::RayTracer(
	// What to render
	SceneNode* root,

	// Image to write to, set to a given width and height
	Image& image,

	// Viewing parameters
	glm::vec3& eye,
	const glm::vec3& view,
	const glm::vec3& up,
	double fovy,

	// Lighting parameters
	const glm::vec3& ambient,
	const std::list<Light*>& lights,

	float end_time // for animation
)

	: m_shared{root, image, eye, view, up, fovy, ambient, lights}
{
	m_end_time = end_time;
	uint num_threads = std::thread::hardware_concurrency();
	if (!num_threads) num_threads = 4;
	for (uint i = 0; i < num_threads; ++i) {
		m_threads.emplace_back(std::thread(WorkerThread(m_jobs)));
	}
	std::cout << num_threads << " threads running" << std::endl;

	size_t h = m_shared.image.height();
	size_t w = m_shared.image.width();

	m_chunk_size = std::min(h, w)/m_threads.size();
	std::cout << m_chunk_size << " chunk size" << std::endl;

#ifdef REFLECTIONS
	std::cout << "REFLECTIONS: ON\n" << "NUMBER OF REFLECTIONS: " << rtc::max_recursions << std::endl;
#ifdef GLOSSY_REFLECTIONS
	std::cout << "GLOSSY_REFLECTIONS: ON" << std::endl;
#endif
#endif
#ifdef REFRACTIONS
	std::cout << "REFRACTIONS: ON" << std::endl;
#ifdef GLOSSY_REFRACTIONS
	std::cout << "GLOSSY_REFRACTIONS: ON" << std::endl;
#endif
#endif
#ifdef SOFT_SHADOWS
	std::cout << "SOFT SHADOWS: ON" << std::endl;
#endif
#ifdef ANTI_ALIASING
	std::cout << "ANTI ALIASING: ON\n" << "# SUPERSAMPLES: " << m_shared.ss << std::endl;
#endif
#ifdef BUMP_MAPPING
	std::cout << "BUMP MAPPING: ON" << std::endl;
#endif
#ifdef AMBIENT_OCCLUSION
	std::cout << "AMBIENT OCCLUSION: ON" << std::endl;
#endif
}

RayTracer::~RayTracer()
{
	m_jobs.release_worker_threads();
	for (auto& t: m_threads) {
		t.join();
	}
}

void RayTracer::render()
{
	size_t h = m_shared.image.height();
	size_t w = m_shared.image.width();

	// Populate the job queue
	for (uint j = 0; j < h; j += m_chunk_size) {
		for (uint i = 0; i < w; i += m_chunk_size) {
			m_jobs.push(Job(i, j, m_chunk_size, &m_shared));
		}
	}

	m_jobs.wait(); // Main thread wait until no jobs
}
