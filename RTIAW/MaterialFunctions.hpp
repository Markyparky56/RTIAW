#pragma once
#include "common.hpp"

//typedef bool(*MaterialFunc)(const Ray&, const HitResult&, colour&, Ray&);

using MaterialFunc = bool(*)(const entt::registry& /*sceneRegistry*/, const Ray& /*inRay*/, const HitResult& /*impactResult*/, colour& /*outColour*/, Ray& /*scatteredRay*/);
//typedef bool(*MaterialFunc)(const entt::registry& /*sceneRegistry*/, const Ray& /*inRay*/, const HitResult& /*impactResult*/, colour& /*outColour*/, Ray& /*scatteredRay*/);

bool LambertianMaterial(const entt::registry& sceneRegistry, const Ray& inRay, const HitResult& impactResult, colour& outColour, Ray& scatteredRay);

bool MetalMaterial(const entt::registry& sceneRegistry, const Ray& inRay, const HitResult& impactResult, colour& outColour, Ray& scatteredRay);
