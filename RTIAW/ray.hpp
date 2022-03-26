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

  position GetStart() const { return Start; }
  direction GetDirection() const { return Dir; }

  position EvalAt(real t) const
  {
    return { Start + (Dir * t) };
  }

protected:
  position Start;
  direction Dir;
};
