#pragma once
#include "ray.hpp"

struct HitResult
{
  position pos;
  vec3 normal;
  real rayT;
  uint8 bFrontFace : 1;
};

void FillHitResult(HitResult& hitResult, const Ray& ray, const real t, const vec3& pos, const vec3& n);

// Returns the value of t if the ray intersects the sphere, otherwise -1
bool HitSphere(const sphere& sph,
  const position& centre,
  const Ray& ray,
  const real minT, const real maxT,
  HitResult& outHitResult
);

