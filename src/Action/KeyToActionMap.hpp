
#pragma once

#include <cassert>
#include <cstddef>
#include <utility>

#include <SFML/Window/Keyboard.hpp>

#include "Action.hpp"

namespace Lumen {
namespace Action {

class KeyToActionMap {
private:
        Lumen::Action::ActionName *m_key_to_action_map_ptr;

        using this_t = KeyToActionMap;
public:

        constexpr KeyToActionMap(void) noexcept : m_key_to_action_map_ptr{nullptr} {}

        constexpr KeyToActionMap(const KeyToActionMap &other) noexcept = delete;

        constexpr KeyToActionMap(KeyToActionMap &&other) noexcept 
        : m_key_to_action_map_ptr{other.m_key_to_action_map_ptr}
        {
                other.m_key_to_action_map_ptr = nullptr;
        }

        constexpr ~KeyToActionMap(void) noexcept 
        {
                if (nullptr != this->m_key_to_action_map_ptr) {
                        this_t::Deallocate(this->m_key_to_action_map_ptr);
                }
        }

        constexpr KeyToActionMap &operator=(const KeyToActionMap &other) noexcept = delete;

        constexpr KeyToActionMap &operator=(KeyToActionMap &&other) noexcept
        {
                if (this == &other) {
                        return *this;
                }
                if (nullptr != this->m_key_to_action_map_ptr) {
                        this_t::Deallocate(this->m_key_to_action_map_ptr);
                }
                this->m_key_to_action_map_ptr = other.m_key_to_action_map_ptr;
                
                other.m_key_to_action_map_ptr = nullptr;
                return *this;
        }
        
        constexpr void Init(void) noexcept
        {
                if (nullptr == this->m_key_to_action_map_ptr) {
                        this->m_key_to_action_map_ptr = this_t::Allocate();
                }

                this->Clear();
        }

        constexpr void Clear(void) noexcept
        {
                if (nullptr == this->m_key_to_action_map_ptr) {
                        return;
                }
                Lumen::Action::ActionName *original_ptr = this_t::MakeOwnerPtrFromReaderPtr(this->m_key_to_action_map_ptr);
                assert(nullptr != original_ptr);
                for (std::size_t i = 0; i < this_t::NumberOfKeys(); ++i) {
                        original_ptr[i] = Lumen::Action::ActionName::NONE;
                }
                assert(nullptr != this->m_key_to_action_map_ptr);
        }

        constexpr Lumen::Action::ActionName operator[](sf::Keyboard::Key keycode) const noexcept
        {
                assert(nullptr != this->m_key_to_action_map_ptr);
                assert(static_cast<int>(keycode) >= -1);
                assert((static_cast<std::size_t>(keycode) < this_t::NumberOfKeys()) ||
                       (-1 == static_cast<int>(keycode)));
                return this->m_key_to_action_map_ptr[static_cast<size_t>(keycode)];
        }

        constexpr Lumen::Action::ActionName &operator[](sf::Keyboard::Key keycode) noexcept
        {
                assert(nullptr != this->m_key_to_action_map_ptr);
                assert(static_cast<int>(keycode) >= -1);
                assert((static_cast<std::size_t>(keycode) < this_t::NumberOfKeys()) ||
                       (-1 == static_cast<int>(keycode)));
                return this->m_key_to_action_map_ptr[static_cast<size_t>(keycode)];
        }

        constexpr Lumen::Action::ActionName GetActionName(sf::Keyboard::Key keycode) const noexcept
        {
                assert(nullptr != this->m_key_to_action_map_ptr);
                assert(static_cast<int>(keycode) >= -1);
                assert((static_cast<std::size_t>(keycode) < this_t::NumberOfKeys()) ||
                       (-1 == static_cast<int>(keycode)));
                return (*this)[keycode];
        }

        constexpr void RegisteAction(sf::Keyboard::Key keycode, Lumen::Action::ActionName action) noexcept
        {
                assert(nullptr != this->m_key_to_action_map_ptr);
                assert(static_cast<int>(keycode) >= -1);
                assert((static_cast<std::size_t>(keycode) < this_t::NumberOfKeys()) ||
                       (-1 == static_cast<int>(keycode)));
                (*this)[keycode] = action;
        }

        constexpr void RemoveAction(sf::Keyboard::Key keycode) noexcept
        {
                assert(nullptr != this->m_key_to_action_map_ptr);
                assert(static_cast<int>(keycode) >= -1);
                assert((static_cast<std::size_t>(keycode) < this_t::NumberOfKeys()) ||
                       (-1 == static_cast<int>(keycode)));
                (*this)[keycode] = Lumen::Action::ActionName::NONE;
        }

        constexpr void RemoveAction(Lumen::Action::ActionName action_name) noexcept
        {
                assert(nullptr != this->m_key_to_action_map_ptr);
                Lumen::Action::ActionName *original_ptr = this_t::MakeOwnerPtrFromReaderPtr(this->m_key_to_action_map_ptr);
                for (std::size_t i = 0; i < this_t::NumberOfKeys(); ++i) {
                        if (action_name == original_ptr[i]) {
                                original_ptr[i] = Lumen::Action::ActionName::NONE;
                        }
                }
        }

        static constexpr std::size_t NumberOfKeys(void) noexcept
        {
                // + 1 Because SFML defined a Unknown key whoes value is -1
                return sf::Keyboard::KeyCount + 1;
        }

        // array size in bytes
        static constexpr std::size_t SizeOf(void) noexcept
        {
                return this_t::NumberOfKeys() * sizeof(Lumen::Action::ActionName);
        }

private:
        static constexpr Lumen::Action::ActionName *MakeReaderPtrFromOwnerPtr(Lumen::Action::ActionName *owner_ptr) noexcept
        {
                assert(nullptr != owner_ptr);
                assert(nullptr != (owner_ptr + 1));
                return owner_ptr + 1;
        }

        static constexpr Lumen::Action::ActionName *MakeOwnerPtrFromReaderPtr(Lumen::Action::ActionName *reader_ptr) noexcept
        {
                assert(nullptr != reader_ptr);
                assert(nullptr != (reader_ptr - 1));
                return reader_ptr - 1;
        }

        static constexpr Lumen::Action::ActionName *Allocate(void) noexcept
        {
                // + 1 Because SFML defined a Unknown key whoes value is -1,
                // so the pointer should point to the second element in the array
                // to avoid accessing invalid memory.
                return (new Lumen::Action::ActionName[this_t::NumberOfKeys()]) + 1;
        }

        static constexpr void Deallocate(Lumen::Action::ActionName *key_to_action_map_ptr) noexcept
        {
                // Because SFML defined a Unknown key whoes value is -1,
                // so the pointer should point to the second element in the array
                // to avoid accessing invalid memory.
                // The pointer should - 1 to get the address of the first element.
                delete[] (key_to_action_map_ptr - 1);
        }
};

} // namespace Action
} // namespace Lumen
