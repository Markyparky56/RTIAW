#pragma once
#include "common.hpp"
#include "components.hpp"
#include "ray.hpp"
#include "glm/vec2.hpp"

class Camera
{
public:
  Camera(
    const real vFov = real(90),
    const real aspectRatio = (real(16) / real(9)),
    const real focalLength = real(1.0),
    const position& pos = position(0),
    const position& lookAt = position(0,0,-1),
    const vec3& vup = vec3(0,1,0)
  );

  Ray GetRay(vec2 uv) const
  {
    return Ray(Pos, LowerLeftCorner + uv.s * horizontal + uv.t * vertical - Pos);
  }

protected:
  position Pos;
  position LowerLeftCorner;
  vec3 horizontal;
  vec3 vertical;
};
