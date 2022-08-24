#include "Animator.hpp"

void Spin::update(double delta_time)
{
    node.rotate('y', theta_per_second * delta_time);
}
