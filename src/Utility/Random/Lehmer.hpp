
#pragma once

#include <cstdint>

namespace Lumen {
namespace Utility {
namespace Random {

constexpr std::uint32_t LehmerRandomNumberGenerator(std::uint32_t seed) noexcept
{
        std::uint64_t tmp = static_cast<std::uint64_t>(seed) * 0x4a39b70d;
        std::uint32_t m1 = static_cast<std::uint32_t>((tmp >> 32) ^ tmp);
        tmp = static_cast<std::uint64_t>(m1) * 0x12fad5c9;
        std::uint32_t m2 = static_cast<std::uint32_t>((tmp >> 32) ^ tmp);
        return m2;
}

constexpr std::uint32_t LehmerRandomNumberGenerator2D(std::uint32_t seed1, std::uint32_t seed2) noexcept
{
        return LehmerRandomNumberGenerator(LehmerRandomNumberGenerator(seed1) ^ seed2);
}

constexpr std::uint32_t LehmerRandomNumberGenerator3D(std::uint32_t seed1, std::uint32_t seed2, std::uint32_t seed3) noexcept
{
        return LehmerRandomNumberGenerator(LehmerRandomNumberGenerator2D(seed1, seed2) ^ seed3);
}

} //namespace Random
} // namespace Utility
} // namespace Lumen
