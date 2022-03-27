#include "MaterialFunctions.hpp"
#include "components.hpp"
#include "hittests.hpp"
#include "utility.hpp"
#include "entt/entity/registry.hpp"
#include "glm/geometric.hpp"

bool LambertianMaterial(const entt::registry& sceneRegistry, const Ray& inRay, const HitResult& impactResult, colour& outColour, Ray& scatteredRay)
{
  //if (sceneRegistry.valid(impactResult.entity))
  {
    // Get material data from registry
    const LambertianData& lamData = sceneRegistry.get<LambertianData>(impactResult.entity);

    vec3 scatterDirection = impactResult.normal + GetRandomUnitVector();

    // Catch degenerate scatter direction (where random unit vector is equal (or close) to normal )
    if (IsNearlyZero(scatterDirection))
    {
      scatterDirection = impactResult.normal;
    }

    scatteredRay = Ray(impactResult.pos, scatterDirection);
    outColour = lamData.albedo;

    return true;
  }
  return false;
}

bool MetalMaterial(const entt::registry& sceneRegistry, const Ray& inRay, const HitResult& impactResult, colour& outColour, Ray& scatteredRay)
{
  //if (sceneRegistry.valid(impactResult.entity))
  {
    // Get material data from registry
    const MetalData& metData = sceneRegistry.get<MetalData>(impactResult.entity);

    const vec3 unitDir = glm::normalize(inRay.GetDirection());
    const vec3 reflected = glm::reflect(unitDir, impactResult.normal);
    const vec3 fuzzed = reflected + metData.fuzz * GetRandomInUnitSphere();
    
    scatteredRay = Ray(impactResult.pos, fuzzed);
    outColour = metData.albedo;

    return (glm::dot(reflected, impactResult.normal) > 0);
  }
  return false;
}

// Schlick Approximation for reflectance
real reflectance(real cosine, real refractIndex)
{
  const real r0 = (real(1.0) - refractIndex) / (real(1.0) + refractIndex);
  const real r0Sq = r0*r0;
  return r0Sq + (real(1.0) - r0Sq) * glm::pow(real(1.0) - cosine, real(5.0));
}

bool DielectricMaterial(const entt::registry& sceneRegistry, const Ray& inRay, const HitResult& impactResult, colour& outColour, Ray& scatteredRay)
{
  //if (sceneRegistry.valid(impactResult.entity))
  {
    // Get material data from registry
    const DielectricData& diData = sceneRegistry.get<DielectricData>(impactResult.entity);

    outColour = colour(1);
    const real refractionRatio = impactResult.bFrontFace ? (real(1.0) / diData.ir) : diData.ir;

    const vec3 unitDir = glm::normalize(inRay.GetDirection());
    const real cosTheta = glm::min(glm::dot(-unitDir, impactResult.normal), 1.0);
    const real sinTheta = glm::sqrt(real(1.0) - cosTheta*cosTheta);

    vec3 dir;
    const bool cannotRefract = refractionRatio*sinTheta > real(1.0);
    if (cannotRefract || reflectance(cosTheta, refractionRatio) > GetRandomReal())
    {
      dir = glm::reflect(unitDir, impactResult.normal);
    }
    else
    {
      dir = glm::refract(unitDir, impactResult.normal, refractionRatio);
    }

    scatteredRay = Ray(impactResult.pos, dir);
    return true;
  }
  return false;
}
