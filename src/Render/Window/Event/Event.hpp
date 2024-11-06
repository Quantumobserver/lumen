
#pragma once

#include <cassert>
#include <optional>
#include <utility>

#include <SFML/Window/Event.hpp>

namespace Lumen {

class RenderWindow;

class Event {
private:
        friend RenderWindow;

        std::optional<sf::Event> m_event;

        //constexpr Event(void) noexcept = default;
        constexpr Event(std::optional<sf::Event> &&event) noexcept : m_event{std::move(event)} {}

public:

        constexpr Event(const Event &) noexcept = delete;
        constexpr Event(Event &&other) noexcept = default;

        constexpr Event &operator=(const Event &) noexcept = delete;
        constexpr Event &operator=(Event &&other) noexcept = default;

        constexpr bool HasEvent(void) const noexcept
        {
                return this->m_event.has_value();
        }

        template<typename T>
        constexpr bool IsThisEventType(void) noexcept
        {
                return this->m_event.value().is<T>();
        }

        /*template<typename T>
        constexpr auto GetEventData(void) noexcept -> decltype(this->m_event.value().visit<T>())
        {
                return this->m_event.value().visit<T>();
        }*/

        template<typename T>
        constexpr T GetEventData(void) const noexcept
        {
                assert(nullptr != this->m_event.value().getIf<T>());
                return *this->m_event.value().getIf<T>();
        }
};

} // namespace Lumen
