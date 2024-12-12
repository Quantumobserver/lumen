
#include <ECS/Entity/EntityManager.hpp>
#include <ECS/System/Debug/DrawBoundingBox.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

#include <iostream>

void DebugDrawBoundingBoxTest0(void) noexcept
{
        std::cout << __func__ << " begin\n";

        Lumen::ECS::Entity::EntityManager entity_manager;

        {
                auto &entity = entity_manager.CreateEntity(Lumen::ECS::Entity::Entity::TagType::PLAYER);

                entity.AddComponent<Lumen::ECS::Component::Transform>(
                        Lumen::Core::Math::Vec2f32{0.0f, 0.0f},
                        Lumen::Core::Math::Vec2f32{0.0f, 0.0f},
                        Lumen::Core::Math::Vec2f32{0.0f, 0.0f});

                entity.AddComponent<Lumen::ECS::Component::BoundingBox>(
                        Lumen::Core::Math::Vec2f32{30.0f, 30.0f}
                );
        }

        {
                auto &entity = entity_manager.CreateEntity(Lumen::ECS::Entity::Entity::TagType::ENEMY);

                entity.AddComponent<Lumen::ECS::Component::Transform>(
                        Lumen::Core::Math::Vec2f32{100.0f,  50.0f},
                        Lumen::Core::Math::Vec2f32{44.0f,   -264.0f},
                        Lumen::Core::Math::Vec2f32{2464.0f,  0.0f});

                entity.AddComponent<Lumen::ECS::Component::BoundingBox>(
                        Lumen::Core::Math::Vec2f32{40.0f, 20.0f},
                        Lumen::ECS::Component::BoundingBox::BlockVision{false},
                        Lumen::ECS::Component::BoundingBox::BlockMovement{false}
                );
        }

        {
                auto &entity = entity_manager.CreateEntity(Lumen::ECS::Entity::Entity::TagType::ENEMY);

                entity.AddComponent<Lumen::ECS::Component::Transform>(
                        Lumen::Core::Math::Vec2f32{250.0f,  400.0f},
                        Lumen::Core::Math::Vec2f32{44.0f,   -264.0f},
                        Lumen::Core::Math::Vec2f32{2464.0f,  0.0f});

                entity.AddComponent<Lumen::ECS::Component::BoundingBox>(
                        Lumen::Core::Math::Vec2f32{20.0f, 40.0f},
                        Lumen::ECS::Component::BoundingBox::BlockVision{true},
                        Lumen::ECS::Component::BoundingBox::BlockMovement{false}
                );
        }

        {
                auto &entity = entity_manager.CreateEntity(Lumen::ECS::Entity::Entity::TagType::ENEMY);

                entity.AddComponent<Lumen::ECS::Component::Transform>(
                        Lumen::Core::Math::Vec2f32{110.0f,  225.0f},
                        Lumen::Core::Math::Vec2f32{44.0f,   -264.0f},
                        Lumen::Core::Math::Vec2f32{2464.0f,  0.0f});

                entity.AddComponent<Lumen::ECS::Component::BoundingBox>(
                        Lumen::Core::Math::Vec2f32{40.0f, 40.0f},
                        Lumen::ECS::Component::BoundingBox::BlockVision{false}
                );
        }

        entity_manager.Update();

        sf::RenderWindow window{sf::VideoMode{{800, 600}}, {"My window"}};

        while (window.isOpen()) {
                for (std::optional<sf::Event> optional_event = window.pollEvent(); optional_event.has_value(); optional_event = window.pollEvent()) {
                        sf::Event &event = optional_event.value();

                        if (event.is<sf::Event::KeyPressed>()) {
                                const sf::Event::KeyPressed &key_pressed_data = *event.getIf<sf::Event::KeyPressed>();
                                if (sf::Keyboard::Key::Escape == key_pressed_data.code) {
                                        window.close();
                                }
                        } else if (event.is<sf::Event::Closed>()) {
                                window.close();
                        }
                }
                window.clear(sf::Color::Yellow);
                Lumen::ECS::System::Debug::DrawBoundingBoxForEach(window, entity_manager);
                window.display();

                entity_manager.Update();
        }

        entity_manager.Clear();

        std::cout << __func__ << " end\n";
}



int main(void) noexcept
{
        DebugDrawBoundingBoxTest0();

        std::cout << "DONE!\n";
        return 0;
}
