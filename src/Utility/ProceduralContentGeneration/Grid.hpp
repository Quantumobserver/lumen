
#pragma once

#include <utility>

namespace Lumen {
namespace Utility {
namespace ProceduralContentGeneration {

template<typename T>
class Grid {
private:

        class GridIterator {
        private:
                Lumen::Core::Memory::ReadWritePtr<T> m_data_ptr;
        public:
                constexpr GridIterator(Lumen::Core::Memory::ReadWritePtr<T> data_ptr) noexcept
                : m_data_ptr{std::move(data_ptr)} {}

                constexpr GridIterator(const GridIterator &) noexcept = default;
                constexpr GridIterator(GridIterator &&) noexcept = default;

                constexpr GridIterator &operator=(const GridIterator &) noexcept = default;
                constexpr GridIterator &operator=(GridIterator &&) noexcept = default;

                constexpr T &operator*(void) noexcept
                {
                        return *this->m_data_ptr.Get();
                }

                constexpr const T &operator*(void) const noexcept
                {
                        return *this->m_data_ptr.Get();
                }

                constexpr T *operator->(void) noexcept
                {
                        return this->m_data_ptr.Get();
                }

                constexpr const T *operator->(void) const noexcept
                {
                        return this->m_data_ptr.Get();
                }

                constexpr GridIterator &operator++(void) noexcept
                {
                        this->m_data_ptr = this->m_data_ptr.Get() + 1;
                        return *this;
                }

                friend constexpr bool operator==(const GridIterator &lhs,
                                                 const GridIterator &rhs) noexcept
                {
                        return lhs.m_data_ptr == rhs.m_data_ptr;
                }

                friend constexpr bool operator!=(const GridIterator &lhs,
                                                 const GridIterator &rhs) noexcept
                {
                        return lhs.m_data_ptr != rhs.m_data_ptr;
                }
        };

        Lumen::Core::Memory::ReadWritePtr<T> m_data_ptr;
        std::size_t m_width;
        std::size_t m_height;

public:
        constexpr Grid(Lumen::Core::Memory::ReadWritePtr<T> data_ptr,
                                       std::size_t width, std::size_t height) noexcept
        : m_data_ptr{std::move(data_ptr)}, m_width{width}, m_height{height} {}

        template<std::size_t WIDTH, std::size_t HEIGHT>
        constexpr Grid(T (&array_2d)[WIDTH][HEIGHT]) noexcept
        : m_data_ptr{&(array_2d[0][0])}, m_width{WIDTH}, m_height{HEIGHT} {}

        constexpr Grid(const Grid &) noexcept = default;
        constexpr Grid(Grid &&) noexcept = default;

        constexpr Grid &operator=(const Grid &) noexcept = default;
        constexpr Grid &operator=(Grid &&) noexcept = default;

        constexpr T &At(std::size_t x, std::size_t y) noexcept
        {
                assert(x < this->m_width && y < this->m_height);
                return this->m_data_ptr.Get()[y * this->m_width + x];
        }

        constexpr const T &At(std::size_t x, std::size_t y) const noexcept
        {
                assert(x < this->m_width && y < this->m_height);
                return this->m_data_ptr.Get()[y * this->m_width + x];
        }

        constexpr T &At(std::size_t index) noexcept
        {
                assert(index < this->m_width * this->m_height);
                return this->m_data_ptr.Get()[index];
        }

        constexpr const T &At(std::size_t index) const noexcept
        {
                assert(index < this->m_width * this->m_height);
                return this->m_data_ptr.Get()[index];
        }

        constexpr std::size_t GetWidth(void) const noexcept
        {
                return this->m_width;
        }

        constexpr std::size_t GetHeight(void) const noexcept
        {
                return this->m_height;
        }

        constexpr std::size_t GetSize(void) const noexcept
        {
                return this->m_width * this->m_height;
        }

        constexpr T *GetData(void) noexcept
        {
                return this->m_data_ptr.Get();
        }

        constexpr const T *GetData(void) const noexcept
        {
                return this->m_data_ptr.Get();
        }

        constexpr void SetData(T *data_ptr, std::size_t width, std::size_t height) noexcept
        {
                this->m_data_ptr = data_ptr;
                this->m_width = width;
                this->m_height = height;
        }

        constexpr void SetData(Lumen::Core::Memory::ReadWritePtr<T> data_ptr,
                               std::size_t width, std::size_t height) noexcept
        {
                this->m_data_ptr = data_ptr;
                this->m_width = width;
                this->m_height = height;
        }

        constexpr GridIterator begin(void) noexcept
        {
                return GridIterator{this->m_data_ptr};
        }

        constexpr GridIterator end(void) noexcept
        {
                return GridIterator{this->m_data_ptr.Get() + this->m_width * this->m_height};
        }

        constexpr const GridIterator begin(void) const noexcept
        {
                return GridIterator{this->m_data_ptr};
        }

        constexpr const GridIterator end(void) const noexcept
        {
                return GridIterator{this->m_data_ptr.Get() + this->m_width * this->m_height};
        }
};

} // namespace ProceduralContentGeneration
} // namespace Utility
} // namespace Lumen