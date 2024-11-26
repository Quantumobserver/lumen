
#pragma once

#include <cstdint>
#include <cstdlib>
#include <random>
#include <thread>
#include <chrono>

namespace Lumen {
namespace Utility {
namespace Random {

using RandomSeed = std::uint32_t;

namespace Detail {

constexpr RandomSeed StdRandomDeviceResultTypeCastToRandomSeed(const std::random_device::result_type &random_device_reslut) noexcept
{
        return static_cast<RandomSeed>(random_device_reslut);
}

constexpr RandomSeed StdThreadIdCastToRandomSeed(const std::thread::id &thread_id) noexcept
{
        RandomSeed random_from_thread_id{0};
        RandomSeed tmp;

        std::size_t i = 0;
        while (i < sizeof(thread_id)) {
                if ((sizeof(thread_id) - i) > sizeof(RandomSeed)) {
                        std::memcpy(&tmp, &thread_id, sizeof(tmp));
                        random_from_thread_id ^= tmp;
                        i += sizeof(RandomSeed);
                } else {
                        std::memcpy(&tmp, &thread_id, (sizeof(thread_id) - i));
                        random_from_thread_id ^= tmp;
                        i = sizeof(thread_id);
                }
        }

        return random_from_thread_id;
}

constexpr RandomSeed StdChronoTimePointStdChronoSystemClockCastToRandomSeed(const std::chrono::time_point<std::chrono::system_clock> &time) noexcept
{
        RandomSeed random_from_time{0};
        RandomSeed tmp;

        std::size_t i = 0;
        while (i < sizeof(time)) {
                if ((sizeof(time) - i) > sizeof(RandomSeed)) {
                        std::memcpy(&tmp, &time, sizeof(tmp));
                        random_from_time ^= tmp;
                        i += sizeof(RandomSeed);
                } else {
                        std::memcpy(&tmp, &time, (sizeof(time) - i));
                        random_from_time ^= tmp;
                        i = sizeof(time);
                }
        }

        return random_from_time;
}

constexpr RandomSeed GenerateRandomSeedImpl(
        const std::random_device::result_type &random_device_reslut,
        const std::thread::id &thread_id,
        const std::chrono::time_point<std::chrono::system_clock> &time) noexcept
{
        RandomSeed random_from_random_device_reslut = StdRandomDeviceResultTypeCastToRandomSeed(random_device_reslut);
        RandomSeed random_from_thread_id = StdThreadIdCastToRandomSeed(thread_id);
        RandomSeed random_from_time = StdChronoTimePointStdChronoSystemClockCastToRandomSeed(time);
        return random_from_random_device_reslut ^ random_from_thread_id ^ random_from_time;
}

} // namespace Detail

constexpr RandomSeed GenerateRandomSeed(void) noexcept
{
        std::random_device random_device{};
        auto random_device_reslut = random_device();
        auto thread_id = std::this_thread::get_id();
        auto time_now = std::chrono::system_clock::now();

        return Detail::GenerateRandomSeedImpl(random_device_reslut, thread_id, time_now);
}

} //namespace Random
} // namespace Utility
} // namespace Lumen