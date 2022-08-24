#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "SceneNode.hpp"

class Animator {
public:
    Animator(SceneNode& node)
    : node(node) {}
    virtual ~Animator() {}
    virtual void update(double delta_time) = 0;

    SceneNode& node;
};

class Spin : public Animator {
static constexpr double theta_per_second = -12;
public:
    Spin(SceneNode& node)
    : Animator(node) {}
    virtual ~Spin() {}
    virtual void update(double delta_time) override;
};
