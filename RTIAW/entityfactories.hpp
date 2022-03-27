#pragma once
#include "entt/entity/registry.hpp"
#include "common.hpp"
#include "components.hpp"

template<typename MaterialData>
entt::entity CreateSphere(entt::registry& entityRegistry, const vec3& pos, const real radius, MaterialFunc materialFunc, const MaterialData& materialData)
{
  auto handle = entityRegistry.create();
  entityRegistry.emplace<sphere>(handle, radius);
  entityRegistry.emplace<position>(handle, pos);
  entityRegistry.emplace<material>(handle, materialFunc);
  entityRegistry.emplace<MaterialData>(handle, materialData);

  return handle;
}
