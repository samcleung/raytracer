#include <iostream>

#include "Material.hpp"
#include "Time.hpp"

#include "utils.hpp"

Material::Material()
	: bump(nullptr)
	, occlusion(nullptr)
{}

Material::~Material()
{
	delete bump;
	delete occlusion;
}

void Material::set_bump(Bump* new_bump)
{
	bump = new_bump;
}

void Material::set_occlusion(AmbientOcclusion* new_occ)
{
	occlusion = new_occ;
}

glm::vec2 Material::get_bump_at(const glm::vec2& uv) const
{
	if (bump == nullptr) return glm::vec2(0);
	return (*bump)(uv);
}

double Material::get_occlusion_at(const glm::vec2& uv) const
{
	if (occlusion == nullptr) return 0.0;
	return (*occlusion)(uv);
}

const glm::vec2 Disco::bevel(0.1f);
const glm::vec2 Disco::size(30, 10);

Disco::~Disco() {}

glm::vec2 Disco::operator()(const glm::vec2& uv) const
{
	const glm::vec2 scaled_uv(uv * size);
	glm::vec2 tile_position(glm::mod(scaled_uv, glm::vec2(1)));
	if (int(uv.y * size.y) % 2) // offset every other row's disco panels
	{
		tile_position.x = glm::mod(tile_position.x +.5f, 1.f);
	}
	
	glm::vec2 perturbation;
	if (tile_position.x < bevel.x) // left bevel
	{
		perturbation += glm::vec2(-1,0);
	}
	else if (1.f - tile_position.x < bevel.x) // right bevel
	{
		perturbation += glm::vec2(1, 0);
	}
	if (tile_position.y < bevel.y) // bottom bevel
	{
		perturbation += glm::vec2(0,-1);
	}
	else if (1.f - tile_position.y < bevel.y) // top bevel
	{
		perturbation += glm::vec2(0, 1);
	}
	else // on panel
	{
		// const glm::vec2 tile_center_uv((glm::floor(scaled_uv) + glm::vec2(0.5)) / size);
		// const glm::vec3 tile_center_normal(-glm::sin(2*M_PI*tile_center_uv.x), 0, -glm::cos(2*M_PI*tile_center_uv.x));
		// const glm::vec3 geometric_normal(-glm::sin(2*M_PI*uv.x), 0, -glm::cos(2*M_PI*uv.x));
		
		// Backward UV mapping to find the original normal
		// perturbation = tile
		// std::cerr << tile_position << " : " << perturbation << std::endl;
	}
	return perturbation;
}

Perlin Water::perlin;

Water::~Water() {}

glm::vec2 Water::operator()( const glm::vec2& uv ) const
{
	// using perlin noise to vary each component of the normal each normal can vary from -1 to 1, steepest is 45
   	constexpr float delta = 1e-4f;
	const glm::vec2 freq(30.f, 30.f);
	const glm::vec2 wrapped_uv( glm::mod( uv + glm::vec2(Time::get_time() * 0.01f, 0), glm::vec2( 1 ) ) );
	const glm::vec2 scaled_uv(wrapped_uv * freq);
	// const float height = perlin( wrapped_uv * freq );
	// const float dhdv = ( perlin( ( wrapped_uv + glm::vec2( 0, delta ) ) * freq ) - height ) / delta;
	// const float dhdu = ( perlin( ( wrapped_uv + glm::vec2( delta, 0 ) ) * freq ) - height ) / delta;
	// return -glm::vec2( dhdu, dhdv ) * 0.01f;
	return glm::vec2(perlin(scaled_uv), perlin(scaled_uv + glm::vec2(100.f))); // sampling y far away from x to avoid horizontal and vertical correlation of noise
}

AmbientOcclusion::~AmbientOcclusion() {}

const glm::vec2 DiscoOcclusion::size(30, 10);

DiscoOcclusion::~DiscoOcclusion() {}

// Occlude depending on the angle between position on bevel and adjacent panel
double DiscoOcclusion::operator()(const glm::vec2& uv) const
{
	glm::vec2 tile_position(glm::mod(uv * size, glm::vec2(1)));
	if (int(uv.y * size.y) % 2) // offset every other row's disco panels
	{
		tile_position.x = glm::mod(tile_position.x +.5f, 1.f);
	}

	float d = min(tile_position.x, tile_position.y, 1 - tile_position.x, 1 - tile_position.y);
	if ( d <= bevel ) // on bevel
	{
		return 0.25 + glm::atan( bevel - d, bevel + d ) / M_PI;
	}

	return 0.0;
}