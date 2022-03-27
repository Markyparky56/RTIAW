#pragma once
#include <cstdint>
#include <limits>
#include <random>
#include <array>
#include "glm/fwd.hpp"
#include "entt/fwd.hpp"

using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;

#ifdef RT_USE_DOUBLE
using real = double;
#else
using real = float;
#endif // RT_USE_DOUBLE

constexpr real SmallEpsilon = real(1e-8);
constexpr real KindaSmallEpsilon = real(1e-4);

// glm aliases
#ifdef RT_USE_DOUBLE
using vec2 = glm::dvec2;
using vec3 = glm::dvec3;
using colour = glm::dvec3;
#else
using vec2 = glm::vec2;
using vec3 = glm::vec3;
using colour = glm::vec3;
#endif

// Constants
constexpr real infinity = std::numeric_limits<real>::infinity();
constexpr real pi = real(3.1415926535897932385);

#if RT_USE_DOUBLE
using prng = std::mt19937_64;
extern prng GMTRng;
#else
using prng = std::mt19937;
extern prng GMTRng;
#endif

// Forward declarations
class Ray;
struct HitResult;
