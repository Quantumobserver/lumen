
#pragma once

#include <ECS/Entity/EntityManager.hpp>

namespace Lumen {
namespace ECS {
namespace System {

constexpr void Movement(Lumen::ECS::Entity::Entity &entity, const float delta_time) noexcept
{
        if (!entity.HasComponent<Lumen::ECS::Component::Transform>()) {
                return;
        }
        auto &transform = entity.GetComponent<Lumen::ECS::Component::Transform>();
        transform.previous_position = transform.position;
        transform.position += delta_time * transform.velocity;
}

constexpr void MovementForEach(Lumen::ECS::Entity::EntityManager &entity_manager, const float delta_time) noexcept
{
        auto &all_entities = entity_manager.GetAllEntities();

        for (auto &entity_vector : all_entities) {
                for (auto &entity : entity_vector) {
                        Movement(entity, delta_time);
                }
        }
}

} // namespace System
} // namespace ECS
} // namespace Lumen