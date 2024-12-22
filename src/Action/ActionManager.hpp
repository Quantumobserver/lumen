
#pragma once

#include "Action.hpp"
#include "KeyToActionMap.hpp"
#include "ActionStatusBuffer.hpp"
#include "ActionKindRecord.hpp"

#include <SFML/Window/Event.hpp>

#include <SFML/Window.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <cstddef>
#include <utility>

#ifdef LUMEN_DEBUG_ACTION_FORMAT_ENABLED
        #include <iostream>
#endif // LUMEN_DEBUG_ACTION_FORMAT_ENABLED

namespace Lumen {
namespace Action {

class ActionManager {
private:
        using this_t = ActionManager;

        Lumen::Action::ActionStatusBuffer m_action_status_buffer;
        Lumen::Action::KeyToActionMap m_key_to_action_map;
        Lumen::Action::ActionKindRecord m_action_kind_record;

public:

        constexpr ActionManager(void) noexcept {}

        constexpr ActionManager(Lumen::Action::KeyToActionMap &&key_to_action_map,
                                Lumen::Action::ActionKindRecord &&action_kind_record) noexcept
        : m_key_to_action_map{std::move(key_to_action_map)}, m_action_kind_record{std::move(action_kind_record)} {}

        constexpr ActionManager(const ActionManager &other) noexcept = delete;

        constexpr ActionManager(ActionManager &&other) noexcept
        : m_action_status_buffer{std::move(other.m_action_status_buffer)},
          m_key_to_action_map{std::move(other.m_key_to_action_map)} {}

        constexpr ActionManager &operator=(const ActionManager &other) noexcept = delete;

        constexpr ActionManager &operator=(ActionManager &&other) noexcept
        {
                this->m_action_status_buffer = std::move(other.m_action_status_buffer);
                this->m_key_to_action_map = std::move(other.m_key_to_action_map);
                return *this;
        }
        
        constexpr void Init(void) noexcept
        {
                this->m_key_to_action_map.Init();
        }

        constexpr void RegisteAction(sf::Keyboard::Key key_code, Lumen::Action::ActionName action_name) noexcept
        {
                this->m_key_to_action_map.RegisteAction(key_code, action_name);
        }

        constexpr void RemoveAction(sf::Keyboard::Key key_code) noexcept
        {
                this->m_key_to_action_map.RemoveAction(key_code);
        }

        constexpr void RemoveAction(Lumen::Action::ActionName action_name) noexcept
        {
                this->m_key_to_action_map.RemoveAction(action_name);
        }

        constexpr void SetActionKind(Lumen::Action::ActionName action_name, Lumen::Action::ActionKind action_kind) noexcept
        {
                this->m_action_kind_record.SetActionKind(action_name, action_kind);
        }

        constexpr void ResetActionBuffer(void) noexcept
        {
                this->m_action_status_buffer.Reset();
        }

        constexpr Lumen::Action::MovementAction GetMovementAction(void) const noexcept
        {
                return this->m_action_status_buffer.GetMovementAction();
        }

        constexpr bool HasWindowResizeAction(void) const noexcept
        {
                return this->m_action_status_buffer.HasWindowResizeAction();
        }

        constexpr sf::Event::Resized GetWindowResizeAction(void) const noexcept
        {
                return this->m_action_status_buffer.GetWindowResizeAction();
        }

        constexpr bool IsWindowCloseActionHappened(void) const noexcept
        {
                return this->m_action_status_buffer.IsWindowCloseActionHappened();
        }

        constexpr void ResetWindowResizeAction(void) noexcept
        {
                this->m_action_status_buffer.ResetWindowResizeAction();
        }

        constexpr const Lumen::Action::SelectionAction &GetSelectionAction(void) const noexcept
        {
                return this->m_action_status_buffer.GetSelectionAction();
        }

        constexpr bool IsSelectionActionHappened(void) const noexcept
        {
                return this->m_action_status_buffer.IsSelectionActionHappened();
        }

private:
        static constexpr Lumen::Action::SelectionAction::ClickTypeTag
        SfmlMouseButtonTypeToClickType(sf::Mouse::Button button_type) noexcept
        {
                switch (button_type) {
                case sf::Mouse::Button::Left:
                        return Lumen::Action::SelectionAction::ClickTypeTag::LEFT_CLICK;
                case sf::Mouse::Button::Right:
                        return Lumen::Action::SelectionAction::ClickTypeTag::RIGHT_CLICK;
                case sf::Mouse::Button::Middle:
                        return Lumen::Action::SelectionAction::ClickTypeTag::MIDDLE_CLICK;
                default:
                        break;
                }
                return Lumen::Action::SelectionAction::ClickTypeTag::UNKNOWN;
        }

        static constexpr Lumen::Action::SelectionAction::WheelScroll::ScrollDirectionTag
        SfmlMouseWheelScrolledToWheelScroll(const sf::Mouse::Wheel &mouse_wheel_scroll_type) noexcept
        {
                switch (mouse_wheel_scroll_type) {
                case sf::Mouse::Wheel::Vertical:
                        return Lumen::Action::SelectionAction::WheelScroll::ScrollDirectionTag::VERTICAL;
                case sf::Mouse::Wheel::Horizontal:
                        return Lumen::Action::SelectionAction::WheelScroll::ScrollDirectionTag::HORIZONTAL;
                }
                std::abort();
        }

public:

        constexpr void CreateActionFromEvent(sf::Event &event) noexcept
        {
                Lumen::Action::Action action{};

                if (event.is<sf::Event::KeyPressed>()) {
                        const auto &key_pressed_data = *event.getIf<sf::Event::KeyPressed>();
                        action.action_name = this->m_key_to_action_map.GetActionName(key_pressed_data.code);
                        action.action_status = this->m_action_kind_record.GetActionStartStatus(action.action_name);

                } else if (event.is<sf::Event::KeyReleased>()) {
                        const auto &key_released_data = *event.getIf<sf::Event::KeyReleased>();
                        action.action_name = this->m_key_to_action_map.GetActionName(key_released_data.code);
                        action.action_status = Lumen::Action::ActionStatus::END;
                } else if (event.is<sf::Event::Resized>()) {
                        this->m_action_status_buffer.SetWindowResizeAction(*event.getIf<sf::Event::Resized>());
                } else if (event.is<sf::Event::Closed>()) {
                        this->m_action_status_buffer.SetWindowCloseAction();
                } else if (event.is<sf::Event::MouseButtonPressed>()) {
                        const auto &mouse_button_pressed_data = *event.getIf<sf::Event::MouseButtonPressed>();
                        
                        const auto click_type = this_t::SfmlMouseButtonTypeToClickType(mouse_button_pressed_data.button);

                        Lumen::Action::SelectionAction selection_action{
                                Lumen::Action::SelectionAction::SelectionActionTypeTag::PRESS,
                                { mouse_button_pressed_data.position.x, mouse_button_pressed_data.position.y},
                                click_type,
                        };

                        this->m_action_status_buffer.SetSelectionAction(selection_action);
                } else if (event.is<sf::Event::MouseButtonReleased>()) {
                        const auto &mouse_button_released_data = *event.getIf<sf::Event::MouseButtonReleased>();

                        const auto click_type = this_t::SfmlMouseButtonTypeToClickType(mouse_button_released_data.button);

                        Lumen::Action::SelectionAction selection_action{
                                Lumen::Action::SelectionAction::SelectionActionTypeTag::RELEASE,
                                { mouse_button_released_data.position.x, mouse_button_released_data.position.y},
                                click_type,
                        };
                        this->m_action_status_buffer.SetSelectionAction(selection_action);
                } else if (event.is<sf::Event::MouseMoved>()) {
                        const auto &mouse_moved_data = *event.getIf<sf::Event::MouseMoved>();

                        Lumen::Action::SelectionAction selection_action{
                            Lumen::Action::SelectionAction::SelectionActionTypeTag::CURSOR_MOVEMENT,
                            { mouse_moved_data.position.x, mouse_moved_data.position.y},
                        };

                        this->m_action_status_buffer.SetSelectionAction(selection_action);
                } else if (event.is<sf::Event::MouseWheelScrolled>()) {
                        const auto &mouse_wheel_scrolled_data = *event.getIf<sf::Event::MouseWheelScrolled>();

                        const auto wheel_scroll_type = this_t::SfmlMouseWheelScrolledToWheelScroll(mouse_wheel_scrolled_data.wheel);

                        Lumen::Action::SelectionAction selection_action{
                            Lumen::Action::SelectionAction::SelectionActionTypeTag::WHEEL_SCROLL,
                            { mouse_wheel_scrolled_data.position.x, mouse_wheel_scrolled_data.position.y},
                            { wheel_scroll_type, mouse_wheel_scrolled_data.delta, },
                        };
                        this->m_action_status_buffer.SetSelectionAction(selection_action);
                }
//std::cout << "[CreateActionFromEvent]: { " << fmt(action.action_name) << ", " << fmt(action.action_status) << " }\n";
                this->m_action_status_buffer.SetActionStatus(action);
        }

        constexpr auto begin(void) noexcept -> decltype(this->m_action_status_buffer.begin())
        {
                return this->m_action_status_buffer.begin();
        }

        constexpr auto end(void) noexcept -> decltype(this->m_action_status_buffer.end())
        {
                return this->m_action_status_buffer.end();
        }
};

} // namespace Action
} // namespace Lumen
