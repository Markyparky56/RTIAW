#pragma once
#include "glm/vec3.hpp"
#include "glm/common.hpp"
#include "common.hpp"
#include "MaterialFunctions.hpp"

struct position : public vec3
{
  position() = default;
  position(real val)
    : vec3(val)
  {}
  position(const vec3& vec)
    : vec3(vec)
  {}
  position(real x, real y, real z)
    : vec3(x, y, z)
  {}

  position& operator=(const vec3& vec)
  {
    *this = { vec.x, vec.y, vec.z };
    return *this;
  }
  explicit operator vec3() const
  {
    return *this;
  }
};

struct direction : public vec3
{
  direction() = default;
  direction(real val)
    : vec3(val)
  {}
  direction(const vec3& vec)
    : vec3(vec)
  {}
  direction(real x, real y, real z)
    : vec3(x, y, z)
  {}

  direction& operator=(const vec3& vec)
  {
    *this = { vec.x, vec.y, vec.z };
    return *this;
  }
  explicit operator vec3() const
  {
    return *this;
  }
};

// The assumption being a sphere entity will also have a position component
struct sphere
{
  real radius;
};

struct material
{
  MaterialFunc materialFunc;
};

struct LambertianData
{
  colour albedo;
};

struct MetalData
{
  // Need a constructor to allow us to clamp the fuzz value
  MetalData(const colour& a, const real f)
    : albedo(a)
    , fuzz(glm::clamp(f, real(0), real(1)))
  {}

  colour albedo;
  real fuzz;
};

struct DielectricData
{
  real ir;
};
