
#pragma once

#include <cstddef>
#include <array>
#include <utility>

#include "Action.hpp"

#include <SFML/Window/Event.hpp>

#include <iostream>
#ifdef LUMEN_DEBUG_ACTION_FORMAT_ENABLED
        #include <iostream>
#endif // LUMEN_DEBUG_ACTION_FORMAT_ENABLED

namespace Lumen {
namespace Action {

class ActionStatusBuffer {
private:
        static constexpr const std::size_t NUMBER_OF_ACTIONS = static_cast<std::size_t>(Lumen::Action::ActionName::NUMBER_OF_ACTIONS);

        std::array<Lumen::Action::ActionStatus,
                   NUMBER_OF_ACTIONS
                   > m_action_status_buffer;

        struct WindowResizeAction {
                bool has_resize_action;
                sf::Event::Resized data;
        } m_window_resize_action;

        bool m_window_close_action{false};

        class ActionStatusBufferIterator {
        private:
                friend ActionStatusBuffer;
                
                Lumen::Action::ActionStatus *m_current_pointer;
                Lumen::Action::ActionStatus *m_end_pointer;
                Lumen::Action::ActionName m_action_name;

                constexpr ActionStatusBufferIterator(Lumen::Action::ActionStatus *pointer, std::size_t number_of_elements) noexcept
                : m_current_pointer{pointer}, m_end_pointer{pointer + number_of_elements}, m_action_name{Lumen::Action::ActionName::NONE}
                {}
        public:

                constexpr void operator++(void) noexcept
                {
                        // The 0 index is reserved for ActionName::NONE,
                        // so we can do ++ at first step
                        do {
                                ++this->m_current_pointer;
                                this->m_action_name = static_cast<Lumen::Action::ActionName>(static_cast<int>(this->m_action_name) + 1);
                        } while ((this->m_current_pointer < this->m_end_pointer) &&
                                 ((Lumen::Action::ActionStatus::END == (*this->m_current_pointer)) ||
                                  (Lumen::Action::ActionStatus::TRIGGER_END == (*this->m_current_pointer))));
                }

                constexpr bool operator!=(const ActionStatusBufferIterator &other) const noexcept
                {
                        return this->m_current_pointer != other.m_current_pointer;
                }

                constexpr Lumen::Action::Action operator*(void) noexcept
                {
                        Lumen::Action::ActionStatus return_value = *this->m_current_pointer;
                        if (Lumen::Action::ActionStatus::TRIGGER == (*this->m_current_pointer)) {
                                *this->m_current_pointer = Lumen::Action::ActionStatus::TRIGGER_END;
                        }
                        return {this->m_action_name, return_value};
                }
        };

        using this_t = ActionStatusBuffer;

public:
        using iterator = ActionStatusBufferIterator;

        constexpr ActionStatusBuffer(void) noexcept : m_action_status_buffer{}, m_window_resize_action{}
        {
                for (std::size_t i = 0; i < this->m_action_status_buffer.size(); ++i) {
                        this->m_action_status_buffer[i] = Lumen::Action::ActionStatus::END;
                }
        }

        constexpr ActionStatusBuffer(const ActionStatusBuffer &) noexcept = delete;
        constexpr ActionStatusBuffer(ActionStatusBuffer &&other) noexcept
        : m_action_status_buffer{std::move(other.m_action_status_buffer)},
          m_window_resize_action{std::move(other.m_window_resize_action)} {}

        constexpr ActionStatusBuffer &operator=(const ActionStatusBuffer &) noexcept = delete;
        constexpr ActionStatusBuffer &operator=(ActionStatusBuffer &&other) noexcept
        {
                if (this == &other) {
                        return *this;
                }
                this->m_action_status_buffer = std::move(other.m_action_status_buffer);
                return *this;
        }

        constexpr void SetWindowResizeAction(const sf::Event::Resized &window_resize_data) noexcept
        {//std::cout << "SetWindowResizeAction\n";
                this->m_window_resize_action.has_resize_action = true;
                this->m_window_resize_action.data = window_resize_data;
        }

        constexpr void ResetWindowResizeAction(void) noexcept
        {
                this->m_window_resize_action.has_resize_action = false;
        }

        constexpr bool HasWindowResizeAction(void) const noexcept
        {
                return this->m_window_resize_action.has_resize_action;
        }

        constexpr sf::Event::Resized GetWindowResizeAction(void) const noexcept
        {
                return this->m_window_resize_action.data;
        }

        constexpr void SetActionStatus(Lumen::Action::Action action) noexcept
        {
                if ((Lumen::Action::ActionStatus::TRIGGER == action.action_status) &&
                    (Lumen::Action::ActionStatus::TRIGGER_END == this->m_action_status_buffer[static_cast<std::size_t>(action.action_name)])) {
                       return;
                }
                //std::cout << "[SetActionStatus]: " << fmt(action.action_status) << "\n";
                this->m_action_status_buffer[static_cast<std::size_t>(action.action_name)] = action.action_status;
        }

        constexpr void Reset(void) noexcept
        {
                for (auto &action_status : this->m_action_status_buffer) {
                        action_status = Lumen::Action::ActionStatus::END;
                }
                this->m_window_resize_action.has_resize_action = false;
                this->m_window_close_action = false;
        }

        static constexpr bool IsActionHappened(Lumen::Action::ActionStatus action_status) noexcept
        {
                constexpr const std::array<bool, static_cast<std::size_t>(Lumen::Action::ActionStatus::END) + 1> action_status_map{
                        true, true, false, false
                };
                return action_status_map[static_cast<std::size_t>(action_status)];
        }

private:
        constexpr bool IsMoveUp(void) const noexcept
        {
                const auto action_status = this->m_action_status_buffer[static_cast<std::size_t>(Lumen::Action::ActionName::MOVE_UP)];
                return this_t::IsActionHappened(action_status);
        }

        constexpr bool IsMoveDown(void) const noexcept
        {
                const auto action_status = this->m_action_status_buffer[static_cast<std::size_t>(Lumen::Action::ActionName::MOVE_DOWN)];
                return this_t::IsActionHappened(action_status);
        }

        constexpr bool IsMoveLeft(void) const noexcept
        {
                const auto action_status = this->m_action_status_buffer[static_cast<std::size_t>(Lumen::Action::ActionName::MOVE_LEFT)];
                return this_t::IsActionHappened(action_status);
        }

        constexpr bool IsMoveRight(void) const noexcept
        {
                const auto action_status = this->m_action_status_buffer[static_cast<std::size_t>(Lumen::Action::ActionName::MOVE_RIGHT)];
                return this_t::IsActionHappened(action_status);
        }
public:
        constexpr Lumen::Action::MovementAction GetMovementAction(void) const noexcept
        {
                Lumen::Action::MovementAction movement_action{
                        this->IsMoveUp(),
                        this->IsMoveDown(),
                        this->IsMoveLeft(),
                        this->IsMoveRight(),
                        false,
                };
                movement_action.has_movement = movement_action.is_move_up || movement_action.is_move_down ||
                                               movement_action.is_move_left || movement_action.is_move_right;
                return movement_action;
        }

        constexpr void SetWindowCloseAction(void) noexcept
        {
                this->m_window_close_action = true;
        }

        constexpr bool IsWindowCloseActionHappened(void) const noexcept
        {
                return this->m_window_close_action;
        }

        constexpr ActionStatusBufferIterator begin(void) noexcept
        {
                return ActionStatusBufferIterator{&this->m_action_status_buffer[0], this->m_action_status_buffer.size()};
        }

        constexpr ActionStatusBufferIterator end(void) noexcept
        {
                return ActionStatusBufferIterator{(&this->m_action_status_buffer[0] + this->m_action_status_buffer.size()),
                                                  this->m_action_status_buffer.size()};
        }
private:

        constexpr Lumen::Action::ActionStatus GetActionStatus(Lumen::Action::ActionName action_name) const noexcept
        {
                return this->m_action_status_buffer[static_cast<std::size_t>(action_name)];
        }
};

} // namespace Lumen
} // namespace Action
