#include "MaterialFunctions.hpp"
#include "components.hpp"
#include "hittests.hpp"
#include "utility.hpp"
#include "entt/entity/registry.hpp"
#include "glm/geometric.hpp"

bool LambertianMaterial(const entt::registry& sceneRegistry, const Ray& inRay, const HitResult& impactResult, colour& outColour, Ray& scatteredRay)
{
  if (sceneRegistry.valid(impactResult.entity))
  {
    // Get material data from registry
    const LambertianData& lamData = sceneRegistry.get<LambertianData>(impactResult.entity);

    vec3 scatterDirection = glm::normalize(impactResult.normal + GetRandomUnitVector());

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
  if (sceneRegistry.valid(impactResult.entity))
  {
    // Get material data from registry
    const MetalData& metData = sceneRegistry.get<MetalData>(impactResult.entity);

    const vec3 reflected = glm::reflect(inRay.GetDirection(), impactResult.normal);
    const vec3 fuzzed = glm::normalize(reflected + metData.fuzz * GetRandomInUnitSphere());
    
    scatteredRay = Ray(impactResult.pos, fuzzed);
    outColour = metData.albedo;

    return (glm::dot(reflected, impactResult.normal) > 0);
  }
  return false;
}
