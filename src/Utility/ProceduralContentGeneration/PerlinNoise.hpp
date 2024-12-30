
#pragma once

#include <Core/Memory/ReadWritePtr.hpp>
#include <Utility/Random/Lehmer.hpp>

#include "Grid.hpp"

#include <cstdint>
#include <vector>

#include <iostream>

namespace Lumen {
namespace Utility {
namespace ProceduralContentGeneration {

constexpr void GenerateWhiteNoise(Lumen::Utility::ProceduralContentGeneration::Grid<float> &grid, 
                                  std::uint32_t white_noise_seed) noexcept
{
        std::uint32_t random_number = white_noise_seed;

        std::size_t index{0};
        for (std::size_t j = 0; j < grid.GetHeight(); ++j) {
                for (std::size_t i = 0; i < grid.GetWidth(); ++i) {
                        random_number = Lumen::Utility::Random::LehmerRandomNumberGenerator2D(
                                random_number ^ static_cast<std::uint32_t>(i), static_cast<std::uint32_t>(j));
                        grid.At(index) = static_cast<float>(random_number) / static_cast<float>(UINT32_MAX);
                        ++index;
                }
        }
}

namespace Detail {

constexpr std::size_t PowerOfTwo(std::size_t exponent) noexcept
{
        std::size_t result = 1;
        for (std::size_t i = 0; i < exponent; ++i) {
                result *= 2;
        }
        return result;

}

constexpr float Interpolate(float x0, float x1, float alpha)
{
        return x0 * (1.0f - alpha) + alpha * x1;
}


} // namespace Detail

constexpr void GenerateSmoothNoise(Lumen::Utility::ProceduralContentGeneration::Grid<float> &smooth_noise_octave,
                                   const Lumen::Utility::ProceduralContentGeneration::Grid<float> &base_noise,
                                   std::size_t octave) noexcept
{
        std::size_t sample_period = Lumen::Utility::ProceduralContentGeneration::Detail::PowerOfTwo(octave);
        float sample_frequency = 1.0f / static_cast<float>(sample_period);
 
        for (std::size_t j = 0; j < base_noise.GetHeight(); ++j) {
                //calculate the vertical sampling indices
                std::size_t sample_j0 = (j / sample_period) * sample_period;
                std::size_t sample_j1 = (sample_j0 + sample_period) % base_noise.GetHeight(); //wrap around
                float vertical_blend = static_cast<float>(j - sample_j0) * sample_frequency;

                for (std::size_t i = 0; i < base_noise.GetWidth(); ++i) {
                        //calculate the horizontal sampling indices
                        std::size_t sample_i0 = (i / sample_period) * sample_period;
                        std::size_t sample_i1 = (sample_i0 + sample_period) % base_noise.GetWidth(); //wrap around
                        float horizontal_blend = static_cast<float>(i - sample_i0) * sample_frequency;

                        //blend the top two corners
                        float top = Lumen::Utility::ProceduralContentGeneration::Detail::Interpolate(
                                base_noise.At(sample_i0, sample_j0), 
                                base_noise.At(sample_i1, sample_j0), horizontal_blend);

                        //blend the bottom two corners
                        float bottom = Lumen::Utility::ProceduralContentGeneration::Detail::Interpolate(
                                base_noise.At(sample_i0, sample_j1),
                                base_noise.At(sample_i1, sample_j1), horizontal_blend);

                        //final blend
                        smooth_noise_octave.At(i, j) = Lumen::Utility::ProceduralContentGeneration::Detail::Interpolate(
                                top, bottom, vertical_blend);
                }
        }
}

struct PerlinNoiseData {
private:
        Lumen::Utility::ProceduralContentGeneration::Grid<float> base_noise;
        std::vector<Lumen::Utility::ProceduralContentGeneration::Grid<float> >smooth_noise_octave;
        Lumen::Utility::ProceduralContentGeneration::Grid<float> perlin_noise;

public:
        constexpr PerlinNoiseData(void) noexcept = default;

        template<std::size_t OCTAVE_COUNT, std::size_t WIDTH, std::size_t HEIGHT>
        constexpr PerlinNoiseData(
                float (&base_noise)[WIDTH][HEIGHT],
                float (&smooth_noise_octave)[OCTAVE_COUNT][WIDTH][HEIGHT],
                float (&perlin_noise)[WIDTH][HEIGHT]) noexcept
        : base_noise{base_noise}, perlin_noise{perlin_noise}
        {
                for (std::size_t i = 0; i < OCTAVE_COUNT; ++i) {
                        this->smooth_noise_octave.push_back(Lumen::Utility::ProceduralContentGeneration::Grid<float>{smooth_noise_octave[i]});
                }
        }

        constexpr PerlinNoiseData(
                Lumen::Utility::ProceduralContentGeneration::Grid<float> &&base_noise,
                std::vector<Lumen::Utility::ProceduralContentGeneration::Grid<float> > &&smooth_noise,
                Lumen::Utility::ProceduralContentGeneration::Grid<float> &&perlin_noise) noexcept
        : base_noise{std::move(base_noise)}, smooth_noise_octave{std::move(smooth_noise)},
          perlin_noise{std::move(perlin_noise)} {}

        constexpr PerlinNoiseData(const PerlinNoiseData &) = delete;
        constexpr PerlinNoiseData(PerlinNoiseData &&other) = default;

        constexpr PerlinNoiseData &operator=(const PerlinNoiseData &) = delete;
        constexpr PerlinNoiseData &operator=(PerlinNoiseData &&other) = default;

//         template<std::size_t OCTAVE_COUNT>
//         constexpr PerlinNoiseData(
//                 Lumen::Utility::ProceduralContentGeneration::Grid<float> (&base_noise_array)[OCTAVE_COUNT]) noexcept
//         : noise_ptr{&base_noise_array[0]}, octave_count{OCTAVE_COUNT}
//         {
// #ifndef NDEBUG
//                 static_assert(OCTAVE_COUNT > 0, "PerlinNoiseData must have at least one octave");
//                 for (std::size_t i = 1; i < OCTAVE_COUNT; ++i) {
//                         assert(base_noise_array[i].GetWidth() == base_noise_array[0].GetWidth());
//                         assert(base_noise_array[i].GetHeight() == base_noise_array[0].GetHeight());
//                 }
// #endif // NDEBUG
//         }

        constexpr std::size_t GetOctaveCount(void) const noexcept
        {
                return this->smooth_noise_octave.size();
        }

        constexpr Lumen::Utility::ProceduralContentGeneration::Grid<float> &GetBaseNoise(void) noexcept
        {
                return this->base_noise;
        }

        constexpr Lumen::Utility::ProceduralContentGeneration::Grid<float> &GetSmoothNoise(std::size_t octave) noexcept
        {
                assert(octave < this->GetOctaveCount());
                return this->smooth_noise_octave[octave];
        }

        constexpr Lumen::Utility::ProceduralContentGeneration::Grid<float> &GetPerlinNoise(void) noexcept
        {
                return this->perlin_noise;
        }

        constexpr std::size_t GetWidth(void) const noexcept
        {
                return this->perlin_noise.GetWidth();
        }

        constexpr std::size_t GetHeight(void) const noexcept
        {
                return this->perlin_noise.GetHeight();
        }

        constexpr std::size_t GetSize(void) const noexcept
        {
                return this->perlin_noise.GetSize();
        }
};

namespace Detail {

constexpr void GenerateSmoothNoiseOctave(PerlinNoiseData &perlin_noise_data) noexcept
{
        for (std::size_t i = 0; i < perlin_noise_data.GetOctaveCount(); ++i) {
                GenerateSmoothNoise(perlin_noise_data.GetSmoothNoise(i), perlin_noise_data.GetBaseNoise(), i);
        }
}

constexpr void SetPerlinNoiseToAllZeros(PerlinNoiseData &perlin_noise_data) noexcept
{
        auto &perlin_noise = perlin_noise_data.GetPerlinNoise();

        for (auto &cell : perlin_noise) {
                cell = 0.0f;
        }
}

struct PerlinNoiseTotalAmplitude {
        float total_amplitude{0.0f};
};

[[nodiscard]] constexpr PerlinNoiseTotalAmplitude BlendSmoothNoiseOctaveTogether(PerlinNoiseData &perlin_noise_data) noexcept
{
        float persistance = 0.5f;

        float amplitude = 1.0f;
        float total_amplitude = 0.0f;

        for (std::size_t octave_reverse_index = 0; octave_reverse_index < perlin_noise_data.GetOctaveCount();
             ++octave_reverse_index) {

                amplitude *= persistance;

                total_amplitude += amplitude;

                const auto &smooth_noise = perlin_noise_data.GetSmoothNoise(
                        perlin_noise_data.GetOctaveCount() - 1 - octave_reverse_index);

                const std::size_t size = perlin_noise_data.GetSize();

                for (std::size_t i = 0; i < size; ++i) {
                        perlin_noise_data.GetPerlinNoise().At(i) += smooth_noise.At(i) * amplitude;
                }
        }

        return {total_amplitude};
}

constexpr void NormalizePerlinNoise(PerlinNoiseData &perlin_noise_data,
                                    const PerlinNoiseTotalAmplitude &total_amplitude) noexcept
{
        auto &perlin_noise = perlin_noise_data.GetPerlinNoise();

        for (auto &cell : perlin_noise) {
                cell /= total_amplitude.total_amplitude;
        }
}

} // namespace Detail

constexpr void GeneratePerlinNoise(PerlinNoiseData &perlin_noise_data) noexcept
{
        Lumen::Utility::ProceduralContentGeneration::Detail::GenerateSmoothNoiseOctave(perlin_noise_data);

        Lumen::Utility::ProceduralContentGeneration::Detail::SetPerlinNoiseToAllZeros(perlin_noise_data);

        const auto total_amplitude = Lumen::Utility::ProceduralContentGeneration::Detail::BlendSmoothNoiseOctaveTogether(
                                        perlin_noise_data);

        Lumen::Utility::ProceduralContentGeneration::Detail::NormalizePerlinNoise(perlin_noise_data, total_amplitude);
}

} // namespace ProceduralContentGeneration
} // namespace Utility
} // namespace Lumen