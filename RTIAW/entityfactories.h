#pragma once
#include "entt/entity/registry.hpp"
#include "common.hpp"
#include "components.hpp"

entt::entity CreateSphere(entt::registry& entityRegistry, const vec3& pos, const real radius);
