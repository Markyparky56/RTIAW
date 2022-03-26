#include "entityfactories.h"

entt::entity CreateSphere(entt::registry& entityRegistry, const vec3& pos, const real radius)
{
  auto handle = entityRegistry.create();
  entityRegistry.emplace<sphere>(handle, radius);
  entityRegistry.emplace<position>(handle, pos);

  return handle;
}
