
#pragma once

#include <ECS/Entity/EntityManager.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <iostream>

namespace Lumen {

namespace ECS {

namespace System {

namespace Debug {

constexpr sf::Color GetBoundingBoxColor(const Lumen::ECS::Component::BoundingBox &bounding_box) noexcept
{
        if (bounding_box.block_movement) {
                if (bounding_box.block_vision) {
                        return sf::Color::Black;
                }
                return sf::Color::Blue;
        }
        if (bounding_box.block_vision) {
                return sf::Color::Red;
        }
        return sf::Color::White;
}

constexpr void DrawBoundingBox(sf::RenderWindow &window, const Lumen::ECS::Entity::Entity &entity) noexcept
{
        if (!entity.HasComponent<Lumen::ECS::Component::Transform>() ||
            !entity.HasComponent<Lumen::ECS::Component::BoundingBox>()) {
                return;
        }
        const auto &transform = entity.GetComponent<Lumen::ECS::Component::Transform>();
        const auto &bounding_box = entity.GetComponent<Lumen::ECS::Component::BoundingBox>();
        sf::RectangleShape rectangle_shape{};
        rectangle_shape.setSize({bounding_box.size.x, bounding_box.size.y});
        rectangle_shape.setFillColor(sf::Color::Transparent);
        rectangle_shape.setOutlineColor(GetBoundingBoxColor(bounding_box));
        rectangle_shape.setOutlineThickness(5.0f);
        rectangle_shape.setOrigin({bounding_box.half_size.x, bounding_box.half_size.y});
        rectangle_shape.setPosition({transform.position.x, transform.position.y});

        window.draw(rectangle_shape);
}

constexpr void DrawBoundingBoxForEach(sf::RenderWindow &window, const Lumen::ECS::Entity::EntityManager &entity_manager) noexcept
{
        const auto &all_entities = entity_manager.GetAllEntities();
        for (const auto &entity_vector : all_entities) {
                for (const auto &entity : entity_vector) {
                        DrawBoundingBox(window, entity);
                }
        }
}

} // namespace Debug

} // namespace System

} // namespace ECS
} // namespace Lumen

