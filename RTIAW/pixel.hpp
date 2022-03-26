#pragma once
#include "common.hpp"
#include "glm/vec3.hpp"

struct Pixel3
{
  Pixel3(uint8 _r, uint8 _g, uint8 _b)
    : r(_r)
    , g(_g)
    , b(_b)
  {}

  Pixel3(const colour& col)
    : r(uint8(col.r))
    , g(uint8(col.g))
    , b(uint8(col.b))
  {
  }
  uint8 r, g, b;
};
