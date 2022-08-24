#pragma once

#include <glm/glm.hpp>

#include "Perlin.hpp"

class Bump;
class AmbientOcclusion;
class Material {
public:
  virtual ~Material();
  void set_bump(Bump* new_bump);
  void set_occlusion(AmbientOcclusion* new_occ);

  glm::vec2 get_bump_at(const glm::vec2& uv) const;
  double get_occlusion_at(const glm::vec2& uv) const;

  Bump* get_bump() const { return bump; }

protected:
  Material();
  Bump* bump;
  AmbientOcclusion* occlusion;
};

class Bump {
public:
  virtual ~Bump() {}
  virtual glm::vec2 operator()(const glm::vec2& uv) const = 0; // perturb normal
};

class Disco : public Bump {
  static const glm::vec2 bevel;
  static const glm::vec2 size;
public:
  virtual ~Disco();
  virtual glm::vec2 operator()(const glm::vec2& uv) const override;
};

class Water : public Bump {
  static Perlin perlin;
public:
  virtual ~Water();
  virtual glm::vec2 operator()(const glm::vec2& uv) const override;
};

class AmbientOcclusion {
public:
  virtual ~AmbientOcclusion();
  virtual double operator()(const glm::vec2& uv) const = 0;
};

class DiscoOcclusion: public AmbientOcclusion {
  static constexpr float bevel = 0.1;
  static constexpr float groove = 0.1;
  static const glm::vec2 size;
public:
  virtual ~DiscoOcclusion();
  virtual double operator()(const glm::vec2& uv) const override;
};