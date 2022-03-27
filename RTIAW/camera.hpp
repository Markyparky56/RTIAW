#pragma once
#include "common.hpp"
#include "components.hpp"
#include "ray.hpp"
#include "glm/vec2.hpp"

class Camera
{
public:
  Camera(
    const real vFov,
    const real aspectRatio,
    const real aperture,
    const real focusDistance,
    const position& pos = position(0),
    const position& lookAt = position(0,0,-1),
    const vec3& vup = vec3(0,1,0)
  );

  Ray GetRay(vec2 uv) const;

protected:
  position Pos;
  position LowerLeftCorner;
  vec3 horizontal;
  vec3 vertical;
  vec3 u, v, w;
  real LensRadius;
};
