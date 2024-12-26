
#pragma once

#include "BasicUIComponent.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

#include <utility>
#include <iostream>
#include <functional>

namespace Lumen {
namespace UI {
namespace Component {

class Button : public Lumen::UI::Component::BasicUIComponent {
private:
        std::string m_text;
        sf::Sprite m_sprite;
        Lumen::UI::Component::BoundingBox m_bounding_box;
        Lumen::UI::Component::TransformRectangleArea m_transform_rectangle_area;
        void *m_do_button_action_data_ptr;
        std::function<void(void *)> fn_do_button_action;

        sf::Color m_outline_color{sf::Color::Green};

public:
        Button(std::string &&text, const sf::Sprite &sprite,
               const Lumen::UI::Component::BoundingBox &bounding_box,
               const Lumen::UI::Component::TransformTopLeft &transform_top_left,
               sf::RenderWindow *window_ptr,
               void *do_button_action_data_ptr = nullptr,
               std::function<void(void *)> &&do_button_action = nullptr
               ) noexcept
        : Lumen::UI::Component::BasicUIComponent{Lumen::UI::Component::UIComponentTypeTag::BUTTON, window_ptr},
          m_text{std::move(text)}, m_sprite{sprite}, m_bounding_box{bounding_box},
          m_transform_rectangle_area{transform_top_left, bounding_box},
          m_do_button_action_data_ptr{do_button_action_data_ptr},
          fn_do_button_action{std::move(do_button_action)}
          {}

        Button(const Button &other) noexcept = delete;
        Button(Button &&other) noexcept
        : Lumen::UI::Component::BasicUIComponent{std::move(other)},
          m_text{std::move(other.m_text)}, m_sprite{std::move(other.m_sprite)},
          m_bounding_box{std::move(other.m_bounding_box)},
          m_transform_rectangle_area{std::move(other.m_transform_rectangle_area)},
          m_do_button_action_data_ptr{other.m_do_button_action_data_ptr},
          fn_do_button_action{std::move(other.fn_do_button_action)}
        {
                other.m_do_button_action_data_ptr = nullptr;
        }

        Button &operator=(const Button &other) noexcept = delete;
        Button &operator=(Button &&other) noexcept
        {
                if (this == &other) {
                        return *this;
                }

                Lumen::UI::Component::BasicUIComponent::operator=(std::move(other));

                this->m_text = std::move(other.m_text);
                this->m_sprite = std::move(other.m_sprite);
                this->m_bounding_box = std::move(other.m_bounding_box);
                this->m_transform_rectangle_area = std::move(other.m_transform_rectangle_area);

                this->m_do_button_action_data_ptr = other.m_do_button_action_data_ptr;
                this->fn_do_button_action = std::move(other.fn_do_button_action);
                other.m_do_button_action_data_ptr = nullptr;
                //this->m_outline_color = (other.m_outline_color);

                return *this;
        }

        constexpr void Init(void) noexcept override {}
        constexpr void Update(const Lumen::Core::Math::Vec2i &) noexcept override
        {
                // TODO
        }

        constexpr void Render(void) noexcept override
        {
                sf::RectangleShape rectangle_shape;
                constexpr const float outline_thickness = 3.0f;
                rectangle_shape.setOutlineThickness(outline_thickness);
                rectangle_shape.setPosition({
                        static_cast<float>(this->m_transform_rectangle_area.top_left_position.top_left_position.x),
                        static_cast<float>(this->m_transform_rectangle_area.top_left_position.top_left_position.y)});
                rectangle_shape.setSize({static_cast<float>(this->m_bounding_box.size.x) - (outline_thickness * 2.0f),
                                         static_cast<float>(this->m_bounding_box.size.y) - (outline_thickness * 2.0f)
                                        });
                rectangle_shape.setOutlineColor(this->m_outline_color);
                rectangle_shape.setFillColor(sf::Color::Transparent);
                //rectangle_shape.setOrigin(rectangle_shape.getSize() / 2.0f);
                this->m_window_ptr->draw(rectangle_shape);
        }

        constexpr void DoWindowResizeAction(const Lumen::Core::Math::Vec2i &window_new_size) noexcept override
        {
                // TODO
                (void)window_new_size;
        }

        constexpr virtual void DoSelectionAction(
                const Lumen::UI::Component::RelativeSelectionAction &relative_selection_action) noexcept override
        {
                if (!Lumen::UI::Component::IsIncluded({relative_selection_action.relative_position_to_the_parent_ui_component},
                                                      this->m_transform_rectangle_area)) {
                        this->m_outline_color = sf::Color::Green;
                        return;
                }
                switch (relative_selection_action.selection_action.selection_action_type) {
                case Lumen::Action::SelectionAction::SelectionActionTypeTag::NONE:
                        //std::cout << "Button None\n";
                        break;
                case Lumen::Action::SelectionAction::SelectionActionTypeTag::PRESS:
                        std::cout << "Button Pressed\n";
                        if (this->fn_do_button_action) {
                                this->fn_do_button_action(this->m_do_button_action_data_ptr);
                        }
                        break;
                case Lumen::Action::SelectionAction::SelectionActionTypeTag::RELEASE:
                        std::cout << "Button Released\n";
                        break;
                case Lumen::Action::SelectionAction::SelectionActionTypeTag::CURSOR_MOVEMENT:
                        this->m_outline_color = sf::Color::Red;
                        break;
                case Lumen::Action::SelectionAction::SelectionActionTypeTag::WHEEL_SCROLL:
                        std::cout << "Button Wheel Scroll\n";
                        break;
                }
        }

        constexpr void SetPosition(const Lumen::Core::Math::Vec2i &position) noexcept override
        {
                this->m_transform_rectangle_area = {position, this->m_bounding_box};
        }

        constexpr void SetSize(const Lumen::Core::Math::Vec2i &size) noexcept override
        {
                this->m_bounding_box.size = size;
        }

        constexpr void SetText(std::string &&text) noexcept override
        {
                this->m_text = std::move(text);
        }

        constexpr void SetIcon(const sf::Sprite &sprite) noexcept override
        {
                this->m_sprite = sprite;
        }

        constexpr std::optional<Lumen::Core::Math::Vec2i> GetPosition(void) const noexcept override
        {
                return std::optional<Lumen::Core::Math::Vec2i>{
                        this->m_transform_rectangle_area.top_left_position.top_left_position
                };
        }

        constexpr std::optional<Lumen::Core::Math::Vec2i> GetSize(void) const noexcept override
        {
                return std::optional<Lumen::Core::Math::Vec2i>{
                        this->m_bounding_box.size
                };
        }

        constexpr std::optional<std::string_view> GetText(void) const noexcept override
        {
                return std::optional<std::string_view>{
                        this->m_text
                };
        }

        constexpr std::optional<sf::Sprite> GetIcon(void) const noexcept override
        {
                return std::optional<sf::Sprite>{this->m_sprite};
        }

};

} // namespace Component
} // namespace UI
} // namespace Lumen