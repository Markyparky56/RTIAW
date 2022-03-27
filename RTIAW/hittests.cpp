#include "hittests.hpp"
#include "glm/geometric.hpp"
#include "glm/gtx/norm.hpp"

void FillHitResult(HitResult& outhitResult, entt::entity entity, const Ray& ray, const real t, const vec3& pos, const vec3& n)
{
  outhitResult.entity = entity;
  outhitResult.rayT = t;
  outhitResult.pos = ray.EvalAt(t);
  outhitResult.bFrontFace = glm::dot(ray.GetDirection(), n) < 0;
  outhitResult.normal = outhitResult.bFrontFace ? n : -n;
}

bool HitSphere(entt::entity entity, const sphere& sph, const position& centre, const Ray& ray, const real minT, const real maxT, HitResult& outHitResult)
{
  const real radius = sph.radius;

  // (b.b) + 2(b.(A-C)) + ((A-C).(A-C) - r^2)
  //   a         b                 c

  const vec3 oc = ray.GetStart() - centre;
  const real a = glm::length2(ray.GetDirection());
  const real halfB = glm::dot(oc, ray.GetDirection());
  const real c = glm::length2(oc) - radius * radius;

  const real discrim = halfB*halfB - a*c;
  if (discrim < 0)
  {
    // No roots
    return false;
  }

  const real sqrtDiscrim = glm::sqrt(discrim);

  // Find nearest root within given range
  real root = (-halfB - sqrtDiscrim) / a;
  if (root < minT || root > maxT)
  {
    root = (-halfB + sqrtDiscrim) / a;
    if (root < minT || root > maxT)
    {
      return false;
    }
  }

  const vec3 pos = ray.EvalAt(root);
  const vec3 normal = (pos - centre) / radius;
  FillHitResult(outHitResult, entity, ray, root, pos, normal);

  return true;
}
