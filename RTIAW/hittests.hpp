#pragma once
#include "ray.hpp"
#include "glm/geometric.hpp"

bool HitSphere(const position& centre, real radius, const Ray& ray)
{
  const vec3 centreToOrigin = ray.GetStart() - centre; // (A - C)

  // (b.b) + 2(b.(A-C)) + ((A-C).(A-C) - r^2)

  const real dotRayDir = glm::dot(ray.GetDirection(), ray.GetDirection()); // b.b
  const real twoDotOCDir = real(2.0) * glm::dot(centreToOrigin, ray.GetDirection()); // 2(b.(A-C))
  const real dotOCMinusRSqrd = glm::dot(centreToOrigin, centreToOrigin) - radius*radius; // (A-C).(A-C) - r^2

  // b^2 - 4ac
  const real discrim = twoDotOCDir*twoDotOCDir - 4*dotRayDir*dotOCMinusRSqrd;

  return discrim > 0;
}
