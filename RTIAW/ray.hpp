#pragma once
#include "common.hpp"
#include "components.hpp"

class Ray
{
public:
  Ray()
    : Start(real(0.f))
    , Dir(real(1.f))
  {}
  Ray(const position& start, const direction& dir)
    : Start(start)
    , Dir(dir)
  {}

  vec3 GetStart() const { return Start; }
  vec3 GetDirection() const { return Dir; }

  position EvalAt(real t) const
  {
    return { Start + (Dir * t) };
  }

protected:
  position Start;
  direction Dir;
};
