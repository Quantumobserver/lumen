
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

        constexpr void CreateActionFromEvent(sf::Event &event) noexcept
        {
                Lumen::Action::Action action{};

                if (event.is<sf::Event::KeyPressed>()) {
                        const sf::Event::KeyPressed &key_pressed_data = *event.getIf<sf::Event::KeyPressed>();
                        action.action_name = this->m_key_to_action_map.GetActionName(key_pressed_data.code);
                        action.action_status = this->m_action_kind_record.GetActionStartStatus(action.action_name);

                } else if (event.is<sf::Event::KeyReleased>()) {
                        const sf::Event::KeyReleased &key_released_data = *event.getIf<sf::Event::KeyReleased>();
                        action.action_name = this->m_key_to_action_map.GetActionName(key_released_data.code);
                        action.action_status = Lumen::Action::ActionStatus::END;
                } // TODO: Mouse Input
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
