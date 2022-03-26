#pragma once
#include "hittests.hpp"
#include "entt/entity/registry.hpp"

class HitTestSystem
{
public:
  bool TestScene(const entt::registry& registry, const Ray& ray, real minT, real maxT, HitResult& outHitResult) const;
  bool TestSpheres(const entt::registry& registry, const Ray& ray, real minT, real maxT, HitResult& outHitResult) const;
};
