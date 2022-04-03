#pragma once
#include "common.hpp"
#include "glm/vec3.hpp"

struct Pixel3
{
  Pixel3() = default;
  Pixel3(uint8 _r, uint8 _g, uint8 _b)
    : r(_r)
    , g(_g)
    , b(_b)
  {}

  Pixel3(const colour& col)
    : r(uint8(realToByte * glm::clamp(col.r, real(0.0), real(1) - KindaSmallEpsilon)))
    , g(uint8(realToByte * glm::clamp(col.g, real(0.0), real(1) - KindaSmallEpsilon)))
    , b(uint8(realToByte * glm::clamp(col.b, real(0.0), real(1) - KindaSmallEpsilon)))
  {
  }
  uint8 r, g, b;

private:
  static constexpr real realToByte = 256 - KindaSmallEpsilon;
};
