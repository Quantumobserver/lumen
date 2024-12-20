#pragma once

#include "BasicUIComponent.hpp"
#include "Button.hpp"

#include <cassert>
#include <cstddef>
#include <vector>
#include <memory>
#include <utility>

namespace Lumen {
namespace UI {
namespace Component {

class Menu : public Lumen::UI::Component::BasicUIComponent {
private:
        Lumen::UI::Component::Transform m_transform;
        Lumen::UI::Component::BoundingBox m_bounding_box;
        std::vector<std::unique_ptr<Lumen::UI::Component::Button> > m_buttons;
        //std::vector<std::unique_ptr<Lumen::UI::Component::Label> > m_lebels;

        sf::Color m_outline_color{sf::Color::Green};

public:
        Menu(void) noexcept = default;
        Menu(const Lumen::UI::Component::Transform &transform,
             const Lumen::UI::Component::BoundingBox &bounding_box,
             sf::RenderWindow *window_ptr,
             std::vector<std::unique_ptr<Lumen::UI::Component::Button> > &&buttons =
                std::vector<std::unique_ptr<Lumen::UI::Component::Button> >{}) noexcept
        : Lumen::UI::Component::BasicUIComponent{Lumen::UI::Component::UIComponentTypeTag::MENU, window_ptr},
          m_transform{transform}, m_bounding_box{bounding_box}, m_buttons{std::move(buttons)}
        {
                for (auto &button : this->m_buttons) {
                        button->SetPositionOffset(this->m_transform.position);
                }
        }

        Menu(const Menu &) noexcept = delete;
        Menu(Menu &&other) noexcept
        : Lumen::UI::Component::BasicUIComponent{std::move(other)},
          m_transform{other.m_transform}, m_bounding_box{other.m_bounding_box},
          m_buttons{std::move(other.m_buttons)} {}
        
        Menu &operator=(const Menu &) noexcept = delete;
        Menu &operator=(Menu &&other) noexcept
        {
                Lumen::UI::Component::BasicUIComponent::operator=(std::move(other));
                m_transform = std::move(other.m_transform);
                m_bounding_box = std::move(other.m_bounding_box);
                m_buttons = std::move(other.m_buttons);
                return *this;
        }

        constexpr void Init(void) noexcept override {}
        constexpr void Update(void) noexcept override {}
        constexpr void Render(void) noexcept override
        {
                for (auto &button : this->m_buttons) {
                        button->Render();
                }

                sf::RectangleShape rectangle_shape;
                constexpr const float outline_thickness = 3.0f;
                rectangle_shape.setOutlineThickness(outline_thickness);
                rectangle_shape.setPosition({
                        static_cast<float>(this->m_transform.position.x),
                        static_cast<float>(this->m_transform.position.y)});
                rectangle_shape.setSize({static_cast<float>(this->m_bounding_box.size.x) - (outline_thickness * 2.0f),
                                         static_cast<float>(this->m_bounding_box.size.y) - (outline_thickness * 2.0f)
                                        });
                rectangle_shape.setOutlineColor(this->m_outline_color);
                rectangle_shape.setFillColor(sf::Color::Transparent);
                rectangle_shape.setOrigin(rectangle_shape.getSize() / 2.0f);
                this->m_window_ptr->draw(rectangle_shape);
        }
        constexpr void DoWindowResizeAction(const Lumen::Core::Math::Vec2i &window_new_size) noexcept override
        {
                // TODO
                (void)window_new_size;
        }

        constexpr void DoSelectionAction(const Lumen::Action::SelectionAction &selection_action) noexcept override
        {
                for (auto &button : this->m_buttons) {
                        button->DoSelectionAction(selection_action);
                }
        }

        constexpr void AddComponent(Lumen::UI::Component::BasicUIComponent &&basic_ui_component) noexcept override
        {
                switch (basic_ui_component.GetUIType()) {
                case Lumen::UI::Component::UIComponentTypeTag::BUTTON: {
                        auto &button = *dynamic_cast<Lumen::UI::Component::Button *>(&basic_ui_component);
                        button.SetPositionOffset(this->m_transform.position);
                        std::cout << "Button: " << button.GetPositionOffset().x << ", " << button.GetPositionOffset().y << "\n";
                        this->m_buttons.push_back(std::make_unique<Lumen::UI::Component::Button>(std::move(button)));
                        break;
                }

                default:
                        break;
                }

                std::abort();
        }

        constexpr void SetPosition(const Lumen::Core::Math::Vec2i &position) noexcept override
        {
                this->m_transform.position = position;
        }

        constexpr virtual void SetSize(const Lumen::Core::Math::Vec2i &size) noexcept override
        {
                this->m_bounding_box.size = size;
                this->m_bounding_box.half_size = size / 2;
        }
        //constexpr virtual void SetText([[maybe_unused]]std::string &&text) noexcept {}
        //constexpr virtual void SetIcon([[maybe_unused]]const sf::Sprite &sprite) noexcept {}

        constexpr std::optional<Lumen::Core::Math::Vec2i> GetPosition(void) const noexcept override
        {
                return std::nullopt;
        }

        constexpr std::optional<Lumen::Core::Math::Vec2i> GetSize(void) const noexcept override
        {
                return std::nullopt;
        }

        constexpr std::optional<const char *> GetText(void) const noexcept override
        {
                return std::nullopt;
        }

        constexpr std::optional<sf::Sprite> GetIcon(void) const noexcept override
        {
                return std::nullopt;
        }
};

} // namespace Component
} // namespace UI        
} // namespace Lumen
