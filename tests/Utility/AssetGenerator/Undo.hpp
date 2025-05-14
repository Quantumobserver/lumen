
#pragma once

#include <cassert>
#include <cstddef>
#include <string>
#include <string_view>
#include <utility>
#include <deque>
#include <optional>

class Undo {
private:
        static constexpr std::size_t DEFAULT_MAX_SIZE = 2048;

        std::size_t m_max_size{DEFAULT_MAX_SIZE};
        std::deque<std::string> m_undo_deque;
        std::size_t m_current_index{0};
        std::size_t m_number_of_commands{0};
        bool m_is_redo_able{false};

public:
        constexpr Undo(void) noexcept = default;
        constexpr Undo(const Undo &) noexcept = delete;
        constexpr Undo(Undo &&) noexcept = default;

        constexpr Undo &operator=(const Undo &) noexcept = delete;
        constexpr Undo &operator=(Undo &&) noexcept = default;

        constexpr void AddCommand(std::string &&command) noexcept
        {
                if (this->m_undo_deque.size() == this->m_max_size) {
                        this->m_undo_deque.pop_front();
                        --this->m_current_index;
                }

                if (this->m_undo_deque.size() > this->m_current_index) {
                        this->m_undo_deque[this->m_current_index] = std::move(command);
                } else {
                        this->m_undo_deque.push_back(std::move(command));
                }
                ++this->m_current_index;
        }

        constexpr std::optional<std::string_view> UndoCommand(void) const noexcept
        {
                if (0 == this->m_current_index) {
                        return std::nullopt;
                }

                --this->m_current_index;
                assert(this->m_current_index < this->m_undo_stack.size());
                return this->m_undo_stack[this->m_current_index];
        }

        constexpr std::optional<std::string_view> RedoCommand(void) const noexcept
        {
                if (this->m_undo_stack.size() == this->m_current_index) {
                        return std::nullopt;
                }

                ++this->m_current_index;
                assert(this->m_current_index < this->m_undo_stack.size());
                return this->m_undo_stack[this->m_current_index];
        }

        constexpr void Clear(void) noexcept
        {
                this->m_undo_deque.clear();
                this->m_current_index = 0;
        }
};
