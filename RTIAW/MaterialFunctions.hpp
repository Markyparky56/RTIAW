#pragma once
#include "common.hpp"

using MaterialFunc = bool(*)(const entt::registry& /*sceneRegistry*/, const Ray& /*inRay*/, const HitResult& /*impactResult*/, colour& /*outColour*/, Ray& /*scatteredRay*/);

bool LambertianMaterial(const entt::registry& sceneRegistry, const Ray& inRay, const HitResult& impactResult, colour& outColour, Ray& scatteredRay);

bool MetalMaterial(const entt::registry& sceneRegistry, const Ray& inRay, const HitResult& impactResult, colour& outColour, Ray& scatteredRay);

bool DielectricMaterial(const entt::registry& sceneRegistry, const Ray& inRay, const HitResult& impactResult, colour& outColour, Ray& scatteredRay);
