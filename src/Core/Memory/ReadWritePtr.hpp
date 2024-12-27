
#pragma once

#include <cassert>
#include <cstddef>
#include <type_traits>

namespace Lumen {
namespace Core {
namespace Memory {

template<typename T>
class ReadWritePtr {
private:
        T *m_pointer;
public:

        constexpr ReadWritePtr(void) noexcept : m_pointer{nullptr} {}
        constexpr ReadWritePtr(std::nullptr_t) noexcept : m_pointer{nullptr} {}
        constexpr ReadWritePtr(T *pointer) noexcept : m_pointer{pointer} {}

        constexpr ReadWritePtr(const ReadWritePtr &other) noexcept = default;
        constexpr ReadWritePtr &operator=(const ReadWritePtr &other) noexcept = default;

        constexpr ReadWritePtr(ReadWritePtr &&other) noexcept : m_pointer{other.m_pointer}
        {
                other.m_pointer = nullptr;
        }

        constexpr ReadWritePtr &operator=(ReadWritePtr &&other) noexcept
        {
                if (this == &other) {
                        return *this;
                }

                this->m_pointer = other.m_pointer;
                other.m_pointer = nullptr;
                return *this;
        }

        constexpr ReadWritePtr &operator=(std::nullptr_t) noexcept
        {
                this->m_pointer = nullptr;
                return *this;
        }

        constexpr ReadWritePtr &operator=(T *pointer) noexcept
        {
                this->m_pointer = pointer;
                return *this;
        }

        constexpr T *Get(void) const noexcept
        {
                return m_pointer;
        }

        constexpr operator bool(void) const noexcept
        {
                return nullptr != m_pointer;
        };

        constexpr T &operator*(void) const noexcept
        {
                return *m_pointer;
        }
        constexpr T *operator->(void) const noexcept
        {
                return m_pointer;
        }

        constexpr bool operator==(const ReadWritePtr &other) const noexcept
        {
                return this->m_pointer == other.m_pointer;
        }

        constexpr bool operator!=(const ReadWritePtr &other) const noexcept
        {
                return this->m_pointer != other.m_pointer;
        }

        constexpr bool operator==(std::nullptr_t) const noexcept
        {
                return nullptr == this->m_pointer;
        }

        constexpr bool operator!=(std::nullptr_t) const noexcept
        {
                return nullptr != this->m_pointer;
        }
};

template<typename T>
constexpr bool operator==(std::nullptr_t, const Lumen::Core::Memory::ReadWritePtr<T> &pointer) noexcept
{
        return nullptr == pointer.Get();
}

template<typename T>
constexpr bool operator!=(std::nullptr_t, const Lumen::Core::Memory::ReadWritePtr<T> &pointer) noexcept
{
        return nullptr != pointer.Get();
}

} // namespace Memory
} // namespace Core
} // namespace Lumen