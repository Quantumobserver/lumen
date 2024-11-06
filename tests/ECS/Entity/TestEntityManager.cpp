
#include <ECS/Entity/EntityManager.hpp>

#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>

#define TEST(expr)                                                                      \
        do {                                                                            \
                if (expr) {                                                             \
                        break;                                                          \
                }                                                                       \
                {                                                                       \
                        std::stringstream string_stream{};                              \
                        string_stream << "[Test Fail]: at \"" __FILE__ "\" Line: "      \
                                                        << __LINE__ << "\n";            \
                        std::cout << string_stream.str();                               \
                }                                                                       \
                std::abort();                                                           \
        } while (0)

void DebugEntityManagerTest0(void) noexcept
{
        std::cout << __func__ << " begin\n";

        Lumen::ECS::Entity::EntityManager entity_manager;

        {
                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::PLAYER);
                        TEST(0 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::ENEMY);
                        TEST(0 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::BULLET);
                        TEST(0 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::TILE);
                        TEST(0 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::DECORATION);
                        TEST(0 == entity_vector.size());
                }
        }

        {
                auto &entities = entity_manager.GetAllEntities();
                for (auto &entity_vector : entities) {
                        TEST(0 == entity_vector.size());
                }
        }

        {
                auto &entity = entity_manager.CreateEntity(Lumen::ECS::Entity::Entity::TagType::PLAYER);

                TEST(false == entity.HasComponent<Lumen::ECS::Component::Transform>());
                entity.AddComponent<Lumen::ECS::Component::Transform>(
                        Lumen::Core::Math::Vec2f32{0.0f, 0.0f},
                        Lumen::Core::Math::Vec2f32{0.0f, 0.0f},
                        Lumen::Core::Math::Vec2f32{0.0f, 0.0f});
                TEST(true == entity.HasComponent<Lumen::ECS::Component::Transform>());

                TEST(false == entity.HasComponent<Lumen::ECS::Component::Score>());
                entity.AddComponent<Lumen::ECS::Component::Score>(10ul);
                TEST(true == entity.HasComponent<Lumen::ECS::Component::Score>());
                entity.RemoveComponent<Lumen::ECS::Component::Score>();
                TEST(false == entity.HasComponent<Lumen::ECS::Component::Score>());

                TEST(0 == entity.GetId());
                TEST(Lumen::ECS::Entity::Entity::TagType::PLAYER == entity.GetTag());
                TEST(true == entity.IsAlive());
        }

        {
                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::PLAYER);
                        TEST(0 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::ENEMY);
                        TEST(0 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::BULLET);
                        TEST(0 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::TILE);
                        TEST(0 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::DECORATION);
                        TEST(0 == entity_vector.size());
                }
        }

        {
                auto &entities = entity_manager.GetAllEntities();
                for (auto &entity_vector : entities) {
                        TEST(0 == entity_vector.size());
                }
        }

        entity_manager.Update();

        {
                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::PLAYER);
                        TEST(1 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::ENEMY);
                        TEST(0 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::BULLET);
                        TEST(0 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::TILE);
                        TEST(0 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::DECORATION);
                        TEST(0 == entity_vector.size());
                }
        }

        {
                auto &entity = entity_manager.CreateEntity(Lumen::ECS::Entity::Entity::TagType::ENEMY);

                TEST(false == entity.HasComponent<Lumen::ECS::Component::Transform>());
                entity.AddComponent<Lumen::ECS::Component::Transform>(
                        Lumen::Core::Math::Vec2f32{3464.0f,  473.0f},
                        Lumen::Core::Math::Vec2f32{44.0f,   -264.0f},
                        Lumen::Core::Math::Vec2f32{2464.0f,  0.0f});
                TEST(true == entity.HasComponent<Lumen::ECS::Component::Transform>());

                TEST(false == entity.HasComponent<Lumen::ECS::Component::Score>());
                entity.AddComponent<Lumen::ECS::Component::Score>(10ul);
                TEST(true == entity.HasComponent<Lumen::ECS::Component::Score>());
                entity.RemoveComponent<Lumen::ECS::Component::Score>();
                TEST(false == entity.HasComponent<Lumen::ECS::Component::Score>());

                TEST(1 == entity.GetId());
                TEST(Lumen::ECS::Entity::Entity::TagType::ENEMY == entity.GetTag());
                TEST(true == entity.IsAlive());
        }

        {
                auto &entity = entity_manager.CreateEntity(Lumen::ECS::Entity::Entity::TagType::ENEMY);

                TEST(false == entity.HasComponent<Lumen::ECS::Component::Transform>());
                entity.AddComponent<Lumen::ECS::Component::Transform>(
                        Lumen::Core::Math::Vec2f32{3464.0f,  473.0f},
                        Lumen::Core::Math::Vec2f32{44.0f,   -264.0f},
                        Lumen::Core::Math::Vec2f32{2464.0f,  0.0f});
                TEST(true == entity.HasComponent<Lumen::ECS::Component::Transform>());

                TEST(false == entity.HasComponent<Lumen::ECS::Component::Score>());
                entity.AddComponent<Lumen::ECS::Component::Score>(10ul);
                TEST(true == entity.HasComponent<Lumen::ECS::Component::Score>());
                entity.RemoveComponent<Lumen::ECS::Component::Score>();
                TEST(false == entity.HasComponent<Lumen::ECS::Component::Score>());

                TEST(2 == entity.GetId());
                TEST(Lumen::ECS::Entity::Entity::TagType::ENEMY == entity.GetTag());
                TEST(true == entity.IsAlive());
        }

        {
                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::PLAYER);
                        TEST(1 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::ENEMY);
                        TEST(0 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::BULLET);
                        TEST(0 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::TILE);
                        TEST(0 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::DECORATION);
                        TEST(0 == entity_vector.size());
                }
        }

        entity_manager.Update();

        {
                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::PLAYER);
                        TEST(1 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::ENEMY);
                        TEST(2 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::BULLET);
                        TEST(0 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::TILE);
                        TEST(0 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::DECORATION);
                        TEST(0 == entity_vector.size());
                }
        }

        {
                auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::ENEMY);
                entity_vector[0].Destroy();

                TEST(false == entity_vector[0].IsAlive());
        }

        {
                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::PLAYER);
                        TEST(1 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::ENEMY);
                        TEST(2 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::BULLET);
                        TEST(0 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::TILE);
                        TEST(0 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::DECORATION);
                        TEST(0 == entity_vector.size());
                }
        }

        entity_manager.Update();

        {
                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::PLAYER);
                        TEST(1 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::ENEMY);
                        TEST(1 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::BULLET);
                        TEST(0 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::TILE);
                        TEST(0 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::DECORATION);
                        TEST(0 == entity_vector.size());
                }
        }

        entity_manager.Clear();

        {
                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::PLAYER);
                        TEST(0 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::ENEMY);
                        TEST(0 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::BULLET);
                        TEST(0 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::TILE);
                        TEST(0 == entity_vector.size());
                }

                {
                        auto &entity_vector = entity_manager.GetEntityVector(Lumen::ECS::Entity::Entity::TagType::DECORATION);
                        TEST(0 == entity_vector.size());
                }
        }
        
        std::cout << __func__ << " end\n";
}



int main(void) noexcept
{
        DebugEntityManagerTest0();

        std::cout << "DONE!\n";
        return 0;
}
