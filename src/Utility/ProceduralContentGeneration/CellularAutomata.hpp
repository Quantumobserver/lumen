
#pragma once

#include <Core/Memory/ReadWritePtr.hpp>
#include <Utility/Random/Lehmer.hpp>

#include <cstddef>
#include <utility>

#include <iostream>

namespace Lumen {
namespace Utility {
namespace ProceduralContentGeneration {

template<typename T>
class CellularAutomataData {
private:

        class CellularAutomataDataIterator {
        private:
                Lumen::Core::Memory::ReadWritePtr<T> m_data_ptr;
        public:
                constexpr CellularAutomataDataIterator(Lumen::Core::Memory::ReadWritePtr<T> data_ptr) noexcept
                : m_data_ptr{std::move(data_ptr)} {}

                constexpr CellularAutomataDataIterator(const CellularAutomataDataIterator &) noexcept = default;
                constexpr CellularAutomataDataIterator(CellularAutomataDataIterator &&) noexcept = default;

                constexpr CellularAutomataDataIterator &operator=(const CellularAutomataDataIterator &) noexcept = default;
                constexpr CellularAutomataDataIterator &operator=(CellularAutomataDataIterator &&) noexcept = default;

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

                constexpr CellularAutomataDataIterator &operator++(void) noexcept
                {
                        this->m_data_ptr = this->m_data_ptr.Get() + 1;
                        return *this;
                }

                friend constexpr bool operator==(const CellularAutomataDataIterator &lhs,
                                                 const CellularAutomataDataIterator &rhs) noexcept
                {
                        return lhs.m_data_ptr == rhs.m_data_ptr;
                }

                friend constexpr bool operator!=(const CellularAutomataDataIterator &lhs,
                                                 const CellularAutomataDataIterator &rhs) noexcept
                {
                        return lhs.m_data_ptr != rhs.m_data_ptr;
                }
        };

        Lumen::Core::Memory::ReadWritePtr<T> m_data_ptr;
        std::size_t m_width;
        std::size_t m_height;

public:
        constexpr CellularAutomataData(Lumen::Core::Memory::ReadWritePtr<T> data_ptr,
                                       std::size_t width, std::size_t height) noexcept
        : m_data_ptr{std::move(data_ptr)}, m_width{width}, m_height{height} {}

        template<std::size_t WIDTH, std::size_t HEIGHT>
        constexpr CellularAutomataData(T (&array_2d)[WIDTH][HEIGHT]) noexcept
        : m_data_ptr{&(array_2d[0][0])}, m_width{WIDTH}, m_height{HEIGHT} {}

        constexpr CellularAutomataData(const CellularAutomataData &) noexcept = default;
        constexpr CellularAutomataData(CellularAutomataData &&) noexcept = default;

        constexpr CellularAutomataData &operator=(const CellularAutomataData &) noexcept = default;
        constexpr CellularAutomataData &operator=(CellularAutomataData &&) noexcept = default;

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

        constexpr CellularAutomataDataIterator begin(void) noexcept
        {
                return CellularAutomataDataIterator{this->m_data_ptr};
        }

        constexpr CellularAutomataDataIterator end(void) noexcept
        {
                return CellularAutomataDataIterator{this->m_data_ptr.Get() + this->m_width * this->m_height};
        }

        constexpr const CellularAutomataDataIterator begin(void) const noexcept
        {
                return CellularAutomataDataIterator{this->m_data_ptr};
        }

        constexpr const CellularAutomataDataIterator end(void) const noexcept
        {
                return CellularAutomataDataIterator{this->m_data_ptr.Get() + this->m_width * this->m_height};
        }
};

template<typename T>
constexpr void InitCellularAutomata(
        Lumen::Utility::ProceduralContentGeneration::CellularAutomataData<T> &cellular_automata_data,
        const std::uint32_t random_seed, const std::uint32_t alive_if_under_or_equal_to) noexcept
{
        //std::size_t i{0};
        std::uint32_t random_number = random_seed;
        for (auto &cell : cellular_automata_data) {

                random_number = Lumen::Utility::Random::LehmerRandomNumberGenerator(random_number);
                if (random_number <= alive_if_under_or_equal_to) {
                        cell = true;
                } else {
                        cell = false;
                }

                //++i;
        }
}

template<typename T>
constexpr std::size_t CountAliveNeighbors(
        const Lumen::Utility::ProceduralContentGeneration::CellularAutomataData<T> &cellular_automata_data,
        std::size_t x, std::size_t y) noexcept
{
        assert(x < cellular_automata_data.GetWidth());
        assert(y < cellular_automata_data.GetHeight());
        std::size_t width = cellular_automata_data.GetWidth();
        std::size_t height = cellular_automata_data.GetHeight();

        std::size_t count{0};

        count += cellular_automata_data.At(((y - 1) % height) * width + ((x - 1) % width)) ? 1 : 0;
        count += cellular_automata_data.At(((y - 1) % height) * width + ((x) % width)) ? 1 : 0;
        count += cellular_automata_data.At(((y - 1) % height) * width + ((x + 1) % width)) ? 1 : 0;

        count += cellular_automata_data.At(((y) % height) * width + ((x - 1) % width)) ? 1 : 0;
        count += cellular_automata_data.At(((y) % height) * width + ((x + 1) % width)) ? 1 : 0;

        count += cellular_automata_data.At(((y + 1) % height) * width + ((x - 1) % width)) ? 1 : 0;
        count += cellular_automata_data.At(((y + 1) % height) * width + ((x) % width)) ? 1 : 0;
        count += cellular_automata_data.At(((y + 1) % height) * width + ((x + 1) % width)) ? 1 : 0;
        // {
        //         count += cellular_automata_data.At(top_row_index) ? 1 : 0;
        //         count += cellular_automata_data.At(top_row_index + 1) ? 1 : 0;
        //         count += cellular_automata_data.At(top_row_index + 2) ? 1 : 0;
        // }

        // {
        //         std::size_t middle_row_index = ((y - 1) * width) + (x);
        //         count += cellular_automata_data.At(middle_row_index) ? 1 : 0;
        //         count += cellular_automata_data.At(middle_row_index + 2) ? 1 : 0;
        // }

        // {
        //         std::size_t bottom_row_index = ((y + 1) * width) + (x - 1);
        //         count += cellular_automata_data.At(bottom_row_index) ? 1 : 0;
        //         count += cellular_automata_data.At(bottom_row_index + 1) ? 1 : 0;
        //         count += cellular_automata_data.At(bottom_row_index + 2) ? 1 : 0;
        // }

        return count;
}

template<typename T>
constexpr void UpdateASingleCellInCellularAutomata(
        Lumen::Utility::ProceduralContentGeneration::CellularAutomataData<T> &cellular_automata_data,
        std::size_t x, std::size_t y,
        const std::size_t death_limit, const std::size_t birth_limit) noexcept
{
        std::size_t num_alive_neighbors = 
                Lumen::Utility::ProceduralContentGeneration::CountAliveNeighbors(cellular_automata_data, x, y);
        
        // if (num_alive_neighbors < death_limit) {
        //         cellular_automata_data.At(x, y) = false;
        // } else if (num_alive_neighbors > birth_limit) {
        //         cellular_automata_data.At(x, y) = true;
        // }

        if (cellular_automata_data.At(x, y)) {
                if (num_alive_neighbors < death_limit) {
                        cellular_automata_data.At(x, y) = false;
                }
        } else {
                if (num_alive_neighbors > birth_limit) {
                        cellular_automata_data.At(x, y) = true;
                }
        }
        
}

template<typename T>
constexpr void UpdateCellularAutomata(
        Lumen::Utility::ProceduralContentGeneration::CellularAutomataData<T> &cellular_automata_data,
        const std::size_t death_limit, const std::size_t birth_limit) noexcept
{
//std::cout << __LINE__ << "\n";
        std::size_t width = cellular_automata_data.GetWidth();
        std::size_t height = cellular_automata_data.GetHeight();
//std::cout << __LINE__ << "\n";
        for (std::size_t y{0}; y < height; ++y) {
                for (std::size_t x{0}; x < width; ++x) {
//std::cout << __LINE__ << "\n";
                        Lumen::Utility::ProceduralContentGeneration::UpdateASingleCellInCellularAutomata(
                                cellular_automata_data, x, y, death_limit, birth_limit);
                }
        }
}


} // namespace ProceduralContentGeneration
} // namespace Utility
} // namespace Lumen
