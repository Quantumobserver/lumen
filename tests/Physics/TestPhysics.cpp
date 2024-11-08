#include <Physics/AABB.hpp>
#include <ECS/Entity/EntityManager.hpp>

#include <cassert>
#include <iostream>
#include <vector>

struct EntityInfo {
        Lumen::ECS::Entity::Entity::TagType tag;
        Lumen::Core::Math::Vec2f32 position;
        Lumen::Core::Math::Vec2f32 previous_position;
        Lumen::ECS::Component::BoundingBox boundingbox;
};

void DebugPhysicsTest0HelperCreateEntityImpl(
        Lumen::ECS::Entity::EntityManager &entity_manager,
        const EntityInfo &entity_info) noexcept
{
        auto &entity = entity_manager.CreateEntity(entity_info.tag);
        entity.AddComponent<Lumen::ECS::Component::Transform>(
                entity_info.position, Lumen::Core::Math::Vec2f32{ 1.0f, 1.0f },
                Lumen::Core::Math::Vec2f32{ 0.0f, 0.0f });
        entity.AddComponent<Lumen::ECS::Component::BoundingBox>(entity_info.boundingbox);
}

void DebugPhysicsTest0HelperCreateEntities(
        Lumen::ECS::Entity::EntityManager &entity_manager,
        const std::vector<EntityInfo> &entity_info_vector) noexcept
{
        for (const auto &entityInfo : entity_info_vector) {
                DebugPhysicsTest0HelperCreateEntityImpl(entity_manager, entityInfo);
        }
        entity_manager.Update();
}

void DebugPhysicsTest0(void) noexcept
{
        Lumen::ECS::Entity::EntityManager entity_manager;
        const std::vector<EntityInfo> entity_info_vector{
                EntityInfo{
                        Lumen::ECS::Entity::Entity::TagType::BULLET,
                        Lumen::Core::Math::Vec2f32{ 10.0f, 15.0f },
                        Lumen::Core::Math::Vec2f32{ 10.0f, 15.0f },
                        Lumen::ECS::Component::BoundingBox{ Lumen::Core::Math::Vec2f32{ 100.0f, 25.0f } }
                },
                EntityInfo{
                        Lumen::ECS::Entity::Entity::TagType::BULLET,
                        Lumen::Core::Math::Vec2f32{ 90.0f, 35.0f },
                        Lumen::Core::Math::Vec2f32{ 60.0f, 105.0f },
                        Lumen::ECS::Component::BoundingBox{ Lumen::Core::Math::Vec2f32{ 100.0f, 20.0f } }
                }
        };
        DebugPhysicsTest0HelperCreateEntities(entity_manager, entity_info_vector);
        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::BULLET);
        assert(entity_info_vector.size() == entity_vector.size());

        Lumen::Core::Math::Vec2f32 ovelap = Lumen::Physics::AABB::GetOverlap<
                                                Lumen::ECS::Component::Transform,
                                                Lumen::ECS::Component::BoundingBox>(entity_vector[0], entity_vector[1]);
        assert(Lumen::Physics::AABB::IsCollide(ovelap));
}

int main(void) noexcept
{
        DebugPhysicsTest0();

        std::cout << "DONE!\n";
        return 0;
}
