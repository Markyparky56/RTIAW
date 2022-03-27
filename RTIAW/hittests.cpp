#include "hittests.hpp"
#include "glm/geometric.hpp"
#include "glm/gtx/norm.hpp"

void FillHitResult(HitResult& hitResult, const Ray& ray, const real t, const vec3& pos, const vec3& n)
{
  hitResult.rayT = t;
  hitResult.pos = ray.EvalAt(t);
  hitResult.bFrontFace = glm::dot(ray.GetDirection(), n) < 0;
  hitResult.normal = hitResult.bFrontFace ? n : -n;
}

bool HitSphere(const sphere& sph, const position& centre, const Ray& ray, const real minT, const real maxT, HitResult& outHitResult)
{
#if 0
  //const vec3 oc = ray.GetStart() - centre;
  const vec3 centreToOrigin = ray.GetStart() - centre; // (A - C)

  // (b.b) + 2(b.(A-C)) + ((A-C).(A-C) - r^2)

//const real a         = glm::dot(ray.GetDirection(), ray.GetDirection());
  const real dotRayDir = glm::dot(ray.GetDirection(), ray.GetDirection()); // b.b

//const real b            = real(2.0) * glm::dot(oc             , ray.GetDirection());
  const real twoDotOCDir = real(2.0) * glm::dot(centreToOrigin, ray.GetDirection()); // 2(b.(A-C))

//const real c                = glm::dot(oc             , oc            ) - radius * radius;
  const real dotOCMinusRSqrd = glm::dot(centreToOrigin, centreToOrigin) - radius * radius; // (A-C).(A-C) - r^2

  // b^2 - 4ac
//const real discrim  = b*b                     - 4*a*c;
  const real discrim = twoDotOCDir * twoDotOCDir - real(4.0) * dotRayDir * dotOCMinusRSqrd;

  if (discrim < 0)
  {
    return real(-1.0);
  }
  else
  {
    return (-twoDotOCDir - glm::sqrt(discrim)) / (real(2.0) * dotRayDir);
  }
#else
  const real radius = sph.radius;

  const vec3 oc = ray.GetStart() - centre;
  const real a = glm::length2(ray.GetDirection());
  const real halfB = glm::dot(oc, ray.GetDirection());
  const real c = glm::length2(oc) - radius * radius;

  const real discrim = halfB * halfB - a * c;
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
  FillHitResult(outHitResult, ray, root, pos, normal);

  return true;
#endif
}
