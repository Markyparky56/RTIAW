#pragma once
#include "common.hpp"
#include "components.hpp"
#include "ray.hpp"
#include "glm/vec2.hpp"

class Camera
{
public:
  Camera(
    const real aspectRatio=(real(16)/real(9)), 
    const real focalLength=real(1.0), 
    const position& pos=position(0))
    : Pos(pos)
  {
    constexpr real viewportHeight = 2.0;
    const real viewportWidth = aspectRatio * viewportHeight;
    
    horizontal = vec3(viewportWidth, 0, 0);
    vertical = vec3(0, viewportHeight, 0);
    LowerLeftCorner = Pos - (horizontal / real(2)) - (vertical / real(2)) - vec3(0, 0, focalLength);
  }

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
