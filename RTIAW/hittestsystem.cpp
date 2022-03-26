#include "hittestsystem.hpp"

bool HitTestSystem::TestScene(const entt::registry& registry, const Ray& ray, real minT, real maxT, HitResult& outHitResult) const
{
  bool hitAnything = false;

  hitAnything |= TestSpheres(registry, ray, minT, maxT, outHitResult);

  return hitAnything;
}

bool HitTestSystem::TestSpheres(const entt::registry& registry, const Ray& ray, real minT, real maxT, HitResult& outHitResult) const
{
  bool hitAnything = false;
  HitResult tempHitResult;
  real closestT = maxT;

  // Iterate over all spheres, find closest
  for (const auto& [entity, sphere, position] : registry.view<sphere, position>().each())
  {
    // Note the use of closestT in place of maxT
    if (HitSphere(sphere, position, ray, minT, closestT, tempHitResult))
    {
      hitAnything = true;
      closestT = tempHitResult.rayT;
      outHitResult = tempHitResult;
    }
  }

  return hitAnything;
}
