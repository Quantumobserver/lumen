
#pragma once

#include <ECS/Entity/EntityManager.hpp>

namespace Lumen {
namespace ECS {
namespace System {

constexpr void LifespanUpdate(Lumen::ECS::Entity::Entity &entity, const float delta_time) noexcept
{
        if (!entity.HasComponent<Lumen::ECS::Component::Lifespan>()) {
                return;
        }
        auto &entity_lifespan = entity.GetComponent<Lumen::ECS::Component::Lifespan>();
        if (entity_lifespan.ShouldBeDestroyed()) {
                entity.Destroy();
        }
        entity_lifespan.remaining_lifespan -= delta_time;
}

constexpr void LifespanUpdateForEach(Lumen::ECS::Entity::EntityManager &entity_manager, const float delta_time) noexcept
{
        auto &all_entities = entity_manager.GetAllEntities();

        for (auto &entity_vector : all_entities) {
                for (auto &entity : entity_vector) {
                        LifespanUpdate(entity, delta_time);
                }
        }
}

} // namespace System
} // namespace ECS
} // namespace Lumen