
#pragma once

#include <Utility/Random/Lehmer.hpp>
#include <Utility/Grid.hpp>

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Color.hpp>

namespace Lumen {
namespace Utility {
namespace AssetGenerator {

constexpr void GenerateWhiteNoise(Lumen::Utility::Grid<sf::Color> &grid, 
                                  std::uint32_t white_noise_seed) noexcept
{
        std::uint32_t random_number = white_noise_seed;

        std::size_t index{0};
        for (std::size_t j = 0; j < grid.GetHeight(); ++j) {
                for (std::size_t i = 0; i < grid.GetWidth(); ++i) {
                        random_number = Lumen::Utility::Random::LehmerRandomNumberGenerator2D(
                                random_number ^ static_cast<std::uint32_t>(i), static_cast<std::uint32_t>(j));
                        grid.At(index) = sf::Color{static_cast<std::uint8_t>(random_number & 0xFF),
                                                   static_cast<std::uint8_t>((random_number >> 8) & 0xFF),
                                                   static_cast<std::uint8_t>((random_number >> 16) & 0xFF)};
                        ++index;
                }
        }
}

constexpr float Interpolate(float x0, float x1, float alpha)
{
        return x0 * (1.0f - alpha) + alpha * x1;
}

constexpr void GenerateGreenNoise(Lumen::Utility::Grid<sf::Color> &grid, 
                                  std::uint32_t white_noise_seed) noexcept
{
        std::uint32_t random_number = white_noise_seed;

        std::size_t index{0};
        for (std::size_t j = 0; j < grid.GetHeight(); ++j) {
                for (std::size_t i = 0; i < grid.GetWidth(); ++i) {
                        random_number = Lumen::Utility::Random::LehmerRandomNumberGenerator2D(
                                random_number ^ static_cast<std::uint32_t>(i), static_cast<std::uint32_t>(j));
                        grid.At(index) = sf::Color{static_cast<std::uint8_t>(((random_number & 0xFF) / 4)),
                                                   static_cast<std::uint8_t>((random_number >> 8) & 0xFF),
                                                   static_cast<std::uint8_t>((((random_number >> 16) & 0xFF) / 3) * 2)};
                        ++index;
                }
        }
}

} // namespace AssetGenerator
} // namespace Utility
} // namespace Lumen