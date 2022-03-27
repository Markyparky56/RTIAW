#pragma once
#include "common.hpp"
#include "glm/gtx/norm.hpp"

inline void SeedRNG()
{
  // std::random_device has it's issues but I don't feel like implementing a multi-platform cryptographically secure alternative right now
  // If that changes, check out: https://stackoverflow.com/questions/45069219/how-to-succinctly-portably-and-thoroughly-seed-the-mt19937-prng

  std::random_device rd;
  std::array<prng::result_type, prng::state_size> state;
  for (uint32 i = 0; i < state.size(); ++i)
  {
#if RT_USE_DOUBLE
    state[i] = (rd() << 31) | rd();
#else
    state[i] = rd();
#endif
  }
  std::seed_seq seedSeq(state.begin(), state.end());

  GMTRng.seed(seedSeq);
}

inline real GetRandomReal(prng& rng = GMTRng)
{
  static std::uniform_real_distribution<real> dist(real(0.0), real(1.0));
  return dist(rng);
}

inline real GetRandomReal(const real min, const real max, prng& rng = GMTRng)
{
  return min + (max - min) * GetRandomReal(rng);
}

inline vec3 GetRandomVec3(prng& rng = GMTRng)
{
  return { GetRandomReal(rng), GetRandomReal(rng), GetRandomReal(rng) };
}

inline vec3 GetRandomVec3(const real min, const real max, prng& rng = GMTRng)
{
  return { GetRandomReal(min, max, rng), GetRandomReal(min, max, rng), GetRandomReal(min, max, rng) };
}

// Sus function
inline vec3 GetRandomInUnitSphere()
{
  while (true)
  {
    const vec3 v = GetRandomVec3(real(-1), real(1));
    if (glm::length2(v) >= 1)
    {
      // Try again
      continue;
    }
    return v;
  }
}

inline vec3 GetRandomUnitVector()
{
  return glm::normalize(GetRandomInUnitSphere());
}

inline bool IsNearlyZero(const vec3& v)
{
  return glm::epsilonEqual(v, vec3(0), SmallEpsilon) == glm::bvec3(true);
}
