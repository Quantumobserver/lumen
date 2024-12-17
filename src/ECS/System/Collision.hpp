#pragma once

#include <ECS/Entity/EntityManager.hpp>
#include <Physics/AABB.hpp>

#include <cassert>
#include <vector>

namespace Lumen {
namespace ECS{
namespace System{

constexpr Lumen::Core::Math::Vec2f32 GetOverlap(Lumen::ECS::Entity::Entity &entity_1,
                                                Lumen::ECS::Entity::Entity &entity_2) noexcept
{
        assert(entity_1.HasComponent<Lumen::ECS::Component::Transform>());
        assert(entity_1.HasComponent<Lumen::ECS::Component::BoundingBox>());
        assert(entity_2.HasComponent<Lumen::ECS::Component::Transform>());
        assert(entity_2.HasComponent<Lumen::ECS::Component::BoundingBox>());

        return Lumen::Physics::AABB::GetOverlap<Lumen::ECS::Component::Transform,
                                                Lumen::ECS::Component::BoundingBox>(entity_1, entity_2);
}

constexpr Lumen::Core::Math::Vec2f32 GetPrevOverlap(Lumen::ECS::Entity::Entity &entity_1,
                                                    Lumen::ECS::Entity::Entity &entity_2) noexcept
{
        assert(entity_1.HasComponent<Lumen::ECS::Component::Transform>());
        assert(entity_1.HasComponent<Lumen::ECS::Component::BoundingBox>());
        assert(entity_2.HasComponent<Lumen::ECS::Component::Transform>());
        assert(entity_2.HasComponent<Lumen::ECS::Component::BoundingBox>());

        return Lumen::Physics::AABB::GetPrevOverlap<Lumen::ECS::Component::Transform,
                                                    Lumen::ECS::Component::BoundingBox>(entity_1, entity_2);
}
constexpr void PlayerCollideWithTile(Lumen::ECS::Entity::Entity &player, Lumen::ECS::Entity::Entity &tile) noexcept
{
        assert(player.HasComponent<Lumen::ECS::Component::Transform>());
        assert(player.HasComponent<Lumen::ECS::Component::BoundingBox>());
        assert(tile.HasComponent<Lumen::ECS::Component::Transform>());
        assert(tile.HasComponent<Lumen::ECS::Component::BoundingBox>());
        
        const auto overlap = Lumen::ECS::System::GetOverlap(player, tile);
        if (!Lumen::Physics::AABB::IsCollide(overlap)) {
                assert(!Lumen::Physics::AABB::IsCollide(Lumen::ECS::System::GetPrevOverlap(player, tile)));
                return;
        }

        const auto prev_overlap = Lumen::ECS::System::GetPrevOverlap(player, tile);
        assert(!Lumen::Physics::AABB::IsCollide(prev_overlap));
        auto &player_transform = player.GetComponent<Lumen::ECS::Component::Transform>();
        const auto &tile_transform = player.GetComponent<Lumen::ECS::Component::Transform>();
        if (prev_overlap.y > 0.0f) {
                if (player_transform.previous_position.x < tile_transform.position.x) {
                        player_transform.position.x -= overlap.x;
                } else {
                        player_transform.position.x += overlap.x;
                        //std::cout << "[Collison] overlap.x: " << overlap.x << "\n";
                }

        } else {
                if (player_transform.previous_position.y < tile_transform.position.y) {
                        player_transform.position.y -= overlap.y;
                } else {
                        player_transform.position.y += overlap.y;
                }
        }
}

constexpr void PlayerCollideWithTilesForEach(std::vector<Lumen::ECS::Entity::Entity> &players,
                                             std::vector<Lumen::ECS::Entity::Entity> &tiles) noexcept
{
        for (auto &player : players) {
                for (auto &tile : tiles) {
                        Lumen::ECS::System::PlayerCollideWithTile(player, tile);
                }
        }
}

constexpr void CollisionForEach(Lumen::ECS::Entity::EntityManager &entity_manager) noexcept
{
        auto &players = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::PLAYER);
        auto &tiles = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::TILE);
        Lumen::ECS::System::PlayerCollideWithTilesForEach(players, tiles);
}

} // namespace System
} // namespace ECS
} // namespace Lumen
