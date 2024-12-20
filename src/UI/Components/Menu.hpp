#pragma once

#include "BasicUIComponent.hpp"

#include <cassert>
#include <cstddef>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <functional>

namespace Lumen {
namespace UI {
namespace Component {

namespace Detail {

class MenuButton {
private:
        std::string m_text;
        sf::Sprite m_icon;
        void *m_do_menu_button_action_data_ptr{nullptr};
        using MenuButtonActionHandler = std::function<void(void *, const Lumen::Action::SelectionAction &)>;
        MenuButtonActionHandler fn_do_menu_button_action;

        sf::Color m_outline_color_menu_button{sf::Color::Yellow};

public:
        constexpr MenuButton(std::string &&text, const sf::Sprite &icon,
                             void *do_menu_button_action_data_ptr = nullptr,
                             MenuButtonActionHandler &&do_menu_button_action = nullptr) noexcept
        : m_text{std::move(text)}, m_icon{icon},
          m_do_menu_button_action_data_ptr{do_menu_button_action_data_ptr},
          fn_do_menu_button_action{std::move(do_menu_button_action)} {}

        constexpr MenuButton(const MenuButton &) noexcept = delete;
        constexpr MenuButton(MenuButton &&other) noexcept
        : m_text{std::move(other.m_text)}, m_icon{std::move(other.m_icon)},
          m_do_menu_button_action_data_ptr{other.m_do_menu_button_action_data_ptr},
          fn_do_menu_button_action{std::move(other.fn_do_menu_button_action)}
        {
                other.m_do_menu_button_action_data_ptr = nullptr;
        }

        constexpr MenuButton &operator=(const MenuButton &) noexcept = delete;
        constexpr MenuButton &operator=(MenuButton &&other) noexcept
        {
                if (this == &other) {
                        return *this;
                }

                this->m_text = std::move(other.m_text);
                this->m_icon = std::move(other.m_icon);
                this->m_do_menu_button_action_data_ptr = other.m_do_menu_button_action_data_ptr;
                this->fn_do_menu_button_action = std::move(other.fn_do_menu_button_action);
                other.m_do_menu_button_action_data_ptr = nullptr;
                return *this;
        }

        constexpr void DoSelectionAction(const Lumen::Action::SelectionAction &selection_action) noexcept
        {
                if (this->fn_do_menu_button_action) {
                        this->fn_do_menu_button_action(this->m_do_menu_button_action_data_ptr, selection_action);
                }
        }

        constexpr void SetColor(const sf::Color &color) noexcept
        {
                this->m_outline_color_menu_button = color;
        }

        constexpr const sf::Color &GetColor(void) const noexcept
        {
                return this->m_outline_color_menu_button;
        }

};
} // namespace Detail

class Menu : public Lumen::UI::Component::BasicUIComponent {
private:
        Lumen::UI::Component::Transform m_transform;
        Lumen::UI::Component::BoundingBox m_bounding_box_menu;
        Lumen::UI::Component::BoundingBox m_bounding_box_menu_button;
        std::vector<Lumen::UI::Component::Detail::MenuButton> m_buttons;
        //std::vector<std::unique_ptr<Lumen::UI::Component::Label> > m_lebels;

        sf::Color m_outline_color_menu{sf::Color::Green};
        sf::RectangleShape m_rectangle_shape;

public:
        Menu(void) noexcept = default;
        Menu(const Lumen::UI::Component::Transform &transform,
             const Lumen::UI::Component::BoundingBox &bounding_box_menu,
             const Lumen::UI::Component::BoundingBox &bounding_box_menu_button,
             sf::RenderWindow *window_ptr,
             std::vector<Lumen::UI::Component::Detail::MenuButton> &&buttons =
                std::vector<Lumen::UI::Component::Detail::MenuButton>{}) noexcept
        : Lumen::UI::Component::BasicUIComponent{Lumen::UI::Component::UIComponentTypeTag::MENU, window_ptr},
          m_transform{transform}, m_bounding_box_menu{bounding_box_menu},
          m_bounding_box_menu_button{bounding_box_menu_button}, m_buttons{std::move(buttons)} {}

        Menu(const Menu &) noexcept = delete;
        Menu(Menu &&other) noexcept = default;
        
        Menu &operator=(const Menu &) noexcept = delete;
        Menu &operator=(Menu &&other) noexcept = default;

        constexpr void Init(void) noexcept override {}
        constexpr void Update(void) noexcept override {}
        constexpr void Render(void) noexcept override
        {
                this->DrawMenuBoundingBox();
                this->DrawMenuButtonBoundingBoxForEach();
        }

        constexpr void DrawMenuBoundingBox(void) noexcept 
        {
                constexpr const float OUTLINE_THICKNESS_MENU = 3.0f;
                this->m_rectangle_shape.setOutlineThickness(OUTLINE_THICKNESS_MENU);
                this->m_rectangle_shape.setPosition({
                        static_cast<float>(this->m_transform.position.x),
                        static_cast<float>(this->m_transform.position.y)});
                this->m_rectangle_shape.setSize({static_cast<float>(this->m_bounding_box_menu.size.x) - (OUTLINE_THICKNESS_MENU * 2.0f),
                                         static_cast<float>(this->m_bounding_box_menu.size.y) - (OUTLINE_THICKNESS_MENU * 2.0f)
                                        });
                this->m_rectangle_shape.setOutlineColor(this->m_outline_color_menu);
                this->m_rectangle_shape.setFillColor(sf::Color::Transparent);
                this->m_rectangle_shape.setOrigin(this->m_rectangle_shape.getSize() / 2.0f);
                this->m_window_ptr->draw(m_rectangle_shape);
        }

        constexpr void  DrawMenuButtonBoundingBoxForEach(void) noexcept
        {
                for (auto iterator = this->m_buttons.begin(); iterator != this->m_buttons.end(); ++iterator) {
                        const auto index = std::distance(this->m_buttons.begin(), iterator);

                        this->DrawMenuButtonBoundingBox(index, *iterator);
                }
        }

        constexpr void DrawMenuButtonBoundingBox(
                const typename std::iterator_traits<decltype(m_buttons)::iterator>::difference_type index,
                const Lumen::UI::Component::Detail::MenuButton &menu_button) noexcept
        {
                constexpr const float outline_thickness_menu_button = 1.0f;
                m_rectangle_shape.setOutlineThickness(outline_thickness_menu_button);
                m_rectangle_shape.setPosition({
                        static_cast<float>(this->m_transform.position.x),
                        static_cast<float>(
                                (this->m_transform.position.y - this->m_bounding_box_menu.half_size.y +
                                        this->m_bounding_box_menu_button.half_size.y) +
                                (this->m_bounding_box_menu_button.size.y * index))});
                m_rectangle_shape.setSize({
                        static_cast<float>(this->m_bounding_box_menu_button.size.x) - (outline_thickness_menu_button * 2.0f),
                        static_cast<float>(this->m_bounding_box_menu_button.size.y) - (outline_thickness_menu_button * 2.0f)
                });
                m_rectangle_shape.setOutlineColor(menu_button.GetColor());
                m_rectangle_shape.setFillColor(sf::Color::Transparent);
                m_rectangle_shape.setOrigin(m_rectangle_shape.getSize() / 2.0f);
                this->m_window_ptr->draw(m_rectangle_shape);
        }


        constexpr void DoWindowResizeAction(const Lumen::Core::Math::Vec2i &window_new_size) noexcept override
        {
                // TODO
                (void)window_new_size;
        }

private:
        constexpr bool IsMenuSelected(const Lumen::Action::SelectionAction &selection_action) const noexcept
        {
                return Lumen::UI::Component::IsSelected(selection_action.position, this->m_bounding_box_menu, this->m_transform);
        }

        constexpr Lumen::UI::Component::Transform GetMenuButtonPosition(
                const typename std::iterator_traits<decltype(m_buttons)::iterator>::difference_type index) const noexcept
        {
                const auto menu_top_position_y = this->m_transform.position.y - this->m_bounding_box_menu.half_size.y;
                const auto menu_button_at_index_zero_position_y = menu_top_position_y + this->m_bounding_box_menu_button.half_size.y;

                return {{(this->m_transform.position.x),
                         (menu_button_at_index_zero_position_y +
                          (this->m_bounding_box_menu_button.size.y * static_cast<int>(index)))}};
        }

        constexpr bool IsMenuButtonSelected(
                const Lumen::Action::SelectionAction &selection_action,
                const typename std::iterator_traits<decltype(m_buttons)::iterator>::difference_type index) const noexcept
        {
                return Lumen::UI::Component::IsSelected(selection_action.position, this->m_bounding_box_menu_button,
                                                        this->GetMenuButtonPosition(index));
        }

public:
        constexpr void DoSelectionAction(const Lumen::Action::SelectionAction &selection_action) noexcept override
        {
                if (!this->IsMenuSelected(selection_action)) {
                        this->m_outline_color_menu = sf::Color::Green;
                        for (auto iterator = this->m_buttons.begin(); iterator != this->m_buttons.end(); ++iterator) {
                                iterator->SetColor(sf::Color::Blue);
                        }
                        return;
                }
                this->m_outline_color_menu = sf::Color::Red;

                for (auto iterator = this->m_buttons.begin(); iterator != this->m_buttons.end(); ++iterator) {
                        const auto index = std::distance(this->m_buttons.begin(), iterator);
        
                        if (!this->IsMenuButtonSelected(selection_action, index)) {
                                iterator->SetColor(sf::Color::Blue);
                                continue;
                        }
                        iterator->SetColor(sf::Color::Yellow);
                        iterator->DoSelectionAction(selection_action);
                }
        }

        constexpr void AddMenuButton(Lumen::UI::Component::Detail::MenuButton &&menu_button) noexcept
        {
                this->m_buttons.push_back(std::move(menu_button));
        }

        constexpr void SetPosition(const Lumen::Core::Math::Vec2i &position) noexcept override
        {
                this->m_transform.position = position;
        }

        constexpr virtual void SetSize(const Lumen::Core::Math::Vec2i &size) noexcept override
        {
                this->m_bounding_box_menu.size = size;
                this->m_bounding_box_menu.half_size = size / 2;
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
