#pragma once
#include "ray.hpp"

struct HitResult
{
  position pos;
  vec3 normal;
  real rayT;
  entt::entity entity;
  uint8 bFrontFace : 1;
};

void FillHitResult(HitResult& outhitResult, entt::entity entity, const Ray& ray, const real t, const vec3& pos, const vec3& n);

// Returns the value of t if the ray intersects the sphere, otherwise -1
bool HitSphere(entt::entity entity, const sphere& sph,
  const position& centre,
  const Ray& ray,
  const real minT, const real maxT,
  HitResult& outHitResult
);

