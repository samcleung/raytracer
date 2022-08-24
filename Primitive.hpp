#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>

#include "HitRecord.hpp"
#include "Ray.hpp"

class Plane;
class Primitive {
public:
  virtual ~Primitive();
	virtual bool hit( Ray& ray, HitRecord& rec, float t0, float t1 ) const = 0;

  std::vector<glm::vec3> m_vertices;
};

class Cube : public Primitive {
public:
  Cube() {}
  
  virtual ~Cube();
  virtual bool hit( Ray& ray, HitRecord& rec, float t0, float t1 ) const override;

private:
  static const Plane sides[6];
};

class Sphere : public Primitive {
public:
  Sphere() {}
  virtual ~Sphere();
  virtual bool hit( Ray& ray, HitRecord& rec, float t0, float t1 ) const override;
};

// Unit cylinder: radius 1 of x and z axes, faces at +1/-1 on y-axis
class Cylinder : public Primitive {
public:
  Cylinder() {}
  virtual ~Cylinder();
  virtual bool hit( Ray& ray, HitRecord& rec, float t0, float t1 ) const override;

private:
  static const Plane caps[2];
};

class Plane : public Primitive {
public:
  Plane( const glm::vec3& origin, const glm::vec3& b1, const glm::vec3& b2, double size = 1 );
  
  virtual ~Plane();
  virtual bool hit( Ray& ray, HitRecord& rec, float t0, float t1 ) const override;

  const glm::vec3& get_origin() const { return m_origin; }
  const glm::vec3& get_b1() const { return m_b1; }
  const glm::vec3& get_b2() const { return m_b2; }
  const double get_size() const { return m_size; }

private:
  glm::vec3 m_origin;
  glm::vec3 m_b1;
  glm::vec3 m_b2;
  glm::vec3 m_norm;
  double m_size;
};

