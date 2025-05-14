
#pragma once

#include <Core/Memory/ReadWritePtr.hpp>
#include <Utility/Random/Lehmer.hpp>

#include <Utility/Grid.hpp>

#include <cstddef>

#include <iostream>

namespace Lumen {
namespace Utility {
namespace ProceduralContentGeneration {


template<typename T>
constexpr void InitCellularAutomata(
        Lumen::Utility::Grid<T> &cellular_automata_data,
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
constexpr void InitCellularAutomata2D(
        Lumen::Utility::Grid<T> &cellular_automata_data,
        const std::uint32_t seed, const std::uint32_t alive_if_under_or_equal_to) noexcept
{

        std::size_t index{0};
        for (std::size_t i = 0; i < cellular_automata_data.GetHeight(); ++i) {
                for (std::size_t j = 0; j < cellular_automata_data.GetWidth(); ++j) {
                        const auto random_number = Lumen::Utility::Random::LehmerRandomNumberGenerator2D(
                                static_cast<std::uint32_t>(j ^ seed), static_cast<std::uint32_t>(i));

                        if (random_number <= alive_if_under_or_equal_to) {
                                cellular_automata_data.At(index) = true;
                        } else {
                                cellular_automata_data.At(index) = false;
                        }
                        ++index;
                }

                //++i;
        }
}

namespace Detail {

template<typename T>
constexpr std::size_t CountAliveNeighbors(
        const Lumen::Utility::Grid<T> &cellular_automata_data,
        std::size_t x, std::size_t y) noexcept
{
        assert(x < cellular_automata_data.GetWidth());
        assert(y < cellular_automata_data.GetHeight());
        std::size_t width = cellular_automata_data.GetWidth();
        std::size_t height = cellular_automata_data.GetHeight();

        std::size_t count{0};

        // count += cellular_automata_data.At(((y - 1) % height) * width + ((x - 1) % width)) ? 1 : 0;
        // count += cellular_automata_data.At(((y - 1) % height) * width + x) ? 1 : 0;
        // count += cellular_automata_data.At(((y - 1) % height) * width + ((x + 1) % width)) ? 1 : 0;

        // count += cellular_automata_data.At(y * width + ((x - 1) % width)) ? 1 : 0;
        // count += cellular_automata_data.At(y * width + ((x + 1) % width)) ? 1 : 0;

        // count += cellular_automata_data.At(((y + 1) % height) * width + ((x - 1) % width)) ? 1 : 0;
        // count += cellular_automata_data.At(((y + 1) % height) * width + x) ? 1 : 0;
        // count += cellular_automata_data.At(((y + 1) % height) * width + ((x + 1) % width)) ? 1 : 0;

        std::size_t y_minus_1 = (y - 1) % height;
        std::size_t y_plus_1 = (y + 1) % height;
        std::size_t x_minus_1 = (x - 1) % width;
        std::size_t x_plus_1 = (x + 1) % width;

        count += cellular_automata_data.At(y_minus_1 * width + x_minus_1);
        count += cellular_automata_data.At(y_minus_1 * width + x);
        count += cellular_automata_data.At(y_minus_1 * width + x_plus_1);

        count += cellular_automata_data.At(y * width + x_minus_1);
        count += cellular_automata_data.At(y * width + x_plus_1);

        count += cellular_automata_data.At(y_plus_1 * width + x_minus_1);
        count += cellular_automata_data.At(y_plus_1 * width + x);
        count += cellular_automata_data.At(y_plus_1 * width + x_plus_1);


        return count;
}

template<typename T>
constexpr void UpdateASingleCellInCellularAutomata(
        Lumen::Utility::Grid<T> &cellular_automata_data,
        std::size_t x, std::size_t y,
        const std::size_t death_limit, const std::size_t birth_limit) noexcept
{
        std::size_t num_alive_neighbors = 
                Lumen::Utility::ProceduralContentGeneration::Detail::CountAliveNeighbors(cellular_automata_data, x, y);
        
        if (num_alive_neighbors < death_limit) {
                cellular_automata_data.At(x, y) = false;
        } else if (num_alive_neighbors > birth_limit) {
                cellular_automata_data.At(x, y) = true;
        }

        // if (cellular_automata_data.At(x, y)) {
        //         if (num_alive_neighbors < death_limit) {
        //                 cellular_automata_data.At(x, y) = false;
        //         }
        // } else {
        //         if (num_alive_neighbors > birth_limit) {
        //                 cellular_automata_data.At(x, y) = true;
        //         }
        // }
        
}

} // namespace Detail

template<typename T>
constexpr void UpdateCellularAutomata(
        Lumen::Utility::Grid<T> &cellular_automata_data,
        const std::size_t death_limit, const std::size_t birth_limit) noexcept
{
//std::cout << __LINE__ << "\n";
        std::size_t width = cellular_automata_data.GetWidth();
        std::size_t height = cellular_automata_data.GetHeight();
//std::cout << __LINE__ << "\n";
        for (std::size_t y{0}; y < height; ++y) {
                for (std::size_t x{0}; x < width; ++x) {
//std::cout << __LINE__ << "\n";
                        Lumen::Utility::ProceduralContentGeneration::Detail::UpdateASingleCellInCellularAutomata(
                                cellular_automata_data, x, y, death_limit, birth_limit);
                }
        }
}


} // namespace ProceduralContentGeneration
} // namespace Utility
} // namespace Lumen
