
#include <Utility/ProceduralContentGeneration/PerlinNoise.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <chrono>
#include <iostream>

void SetVertexArrayPerlinNoise(
        sf::VertexArray &vertices,
        const Lumen::Utility::ProceduralContentGeneration::Grid<float> &perlin_noise,
        const float cell_size) noexcept
{
        vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
        vertices.resize(perlin_noise.GetSize() * 6);

        std::size_t cellular_automata_index{0};
        std::size_t vertex_index{0};

        // const sf::Color alive_color{sf::Color::Black};
        // const sf::Color dead_color{sf::Color::White};

        // const sf::Color alive_color{sf::Color::White};
        // const sf::Color dead_color{sf::Color::Red};

        for (const auto &cell : perlin_noise) {
                const std::size_t x = cellular_automata_index % perlin_noise.GetWidth();
                const std::size_t y = cellular_automata_index / perlin_noise.GetWidth();

                const float position_left_x = static_cast<float>(x) * cell_size;
                const float position_top_y = static_cast<float>(y) * cell_size;
                const float position_right_x = static_cast<float>(x + 1) * cell_size;
                const float position_bottom_y = static_cast<float>(y + 1) * cell_size;

                // const sf::Color color = {
                //         static_cast<std::uint8_t>((cell * alive_color.r) + ((1 - cell) * dead_color.r)),
                //         static_cast<std::uint8_t>((cell * alive_color.g) + ((1 - cell) * dead_color.g)),
                //         static_cast<std::uint8_t>((cell * alive_color.b) + ((1 - cell) * dead_color.b)),
                //         static_cast<std::uint8_t>((cell * alive_color.a) + ((1 - cell) * dead_color.a)),
                // };

                // sf::Color color{};
                // if  (cell < 0.5f) {
                //         color = {
                //                 static_cast<std::uint8_t>((cell * sf::Color::Red.r) + ((1 - cell) * sf::Color::Black.r)),
                //                 static_cast<std::uint8_t>((cell * sf::Color::Red.g) + ((1 - cell) * sf::Color::Black.g)),
                //                 static_cast<std::uint8_t>((cell * sf::Color::Red.b) + ((1 - cell) * sf::Color::Black.b)),
                //                 static_cast<std::uint8_t>((cell * sf::Color::Red.a) + ((1 - cell) * sf::Color::Black.a)),
                //         };
                // } else {
                //         color = {
                //                 static_cast<std::uint8_t>((cell * sf::Color::Red.r) + ((1 - cell) * sf::Color::White.r)),
                //                 static_cast<std::uint8_t>((cell * sf::Color::Red.g) + ((1 - cell) * sf::Color::White.g)),
                //                 static_cast<std::uint8_t>((cell * sf::Color::Red.b) + ((1 - cell) * sf::Color::White.b)),
                //                 static_cast<std::uint8_t>((cell * sf::Color::Red.a) + ((1 - cell) * sf::Color::White.a)),
                //         };
                // };

                sf::Color color{};
                if  (cell < 0.5f) {
                        color = {
                                static_cast<std::uint8_t>((cell * sf::Color::Blue.r) + ((1 - cell) * sf::Color::Black.r)),
                                static_cast<std::uint8_t>((cell * sf::Color::Blue.g) + ((1 - cell) * sf::Color::Black.g)),
                                static_cast<std::uint8_t>((cell * sf::Color::Blue.b) + ((1 - cell) * sf::Color::Black.b)),
                                static_cast<std::uint8_t>((cell * sf::Color::Blue.a) + ((1 - cell) * sf::Color::Black.a)),
                        };
                } else {
                        color = {
                                static_cast<std::uint8_t>((cell * sf::Color::Green.r) + ((1 - cell) * sf::Color::Black.r)),
                                static_cast<std::uint8_t>((cell * sf::Color::Green.g) + ((1 - cell) * sf::Color::Black.g)),
                                static_cast<std::uint8_t>((cell * sf::Color::Green.b) + ((1 - cell) * sf::Color::Black.b)),
                                static_cast<std::uint8_t>((cell * sf::Color::Green.a) + ((1 - cell) * sf::Color::Black.a)),
                        };
                };

                // sf::Color color{};
                // if  (cell < 0.3f) {
                //         color = sf::Color::Black;
                // } else {
                //         color = sf::Color::White;
                // };

                vertices[vertex_index].position = sf::Vector2f{position_left_x, position_top_y};
                vertices[vertex_index].color = color;

                vertices[vertex_index + 1].position = sf::Vector2f{position_right_x, position_top_y};
                vertices[vertex_index + 1].color = color;

                vertices[vertex_index + 2].position = sf::Vector2f{position_right_x, position_bottom_y};
                vertices[vertex_index + 2].color = color;

                vertices[vertex_index + 3].position = sf::Vector2f{position_left_x, position_top_y};
                vertices[vertex_index + 3].color = color;

                vertices[vertex_index + 4].position = sf::Vector2f{position_right_x, position_bottom_y};
                vertices[vertex_index + 4].color = color;

                vertices[vertex_index + 5].position = sf::Vector2f{position_left_x, position_bottom_y};
                vertices[vertex_index + 5].color = color;

                ++cellular_automata_index;
                vertex_index += 6;
        }
}

constexpr const std::size_t SCALE = 30;
constexpr const std::size_t GRID_WIDTH = 80 * SCALE;
constexpr const std::size_t GRID_HEIGHT = 60 * SCALE;
constexpr const std::size_t OCTAVE = 6;

//float float_array[OCTAVE][GRID_WIDTH][GRID_HEIGHT] = {{}};
float (&float_array)[OCTAVE][GRID_WIDTH][GRID_HEIGHT] =
        *reinterpret_cast<float (*)[OCTAVE][GRID_WIDTH][GRID_HEIGHT]>(new float[OCTAVE][GRID_WIDTH][GRID_HEIGHT]);

constexpr void GenerateWhiteNoise_Old(Lumen::Utility::ProceduralContentGeneration::Grid<float> &grid, 
                                  std::uint32_t random_seed) noexcept
{
        std::uint32_t random_number = random_seed;
        for (std::size_t i = 0; i < grid.GetHeight(); ++i) {
                for (std::size_t j = 0; j < grid.GetWidth(); ++j) {
                        random_number = Lumen::Utility::Random::LehmerRandomNumberGenerator3D(
                                random_number, static_cast<std::uint32_t>(i), static_cast<std::uint32_t>(j));
                        grid.At(j, i) = static_cast<float>(random_number) / static_cast<float>(UINT32_MAX);
                }
        }
}

constexpr void GenerateWhiteNoise_New(Lumen::Utility::ProceduralContentGeneration::Grid<float> &grid, 
                                  std::uint32_t random_seed) noexcept
{
        std::uint32_t random_number = random_seed;
        for (std::size_t i = 0; i < grid.GetHeight(); ++i) {
                for (std::size_t j = 0; j < grid.GetWidth(); ++j) {
                        random_number = Lumen::Utility::Random::LehmerRandomNumberGenerator2D(
                                random_number ^ static_cast<std::uint32_t>(i), random_number ^ static_cast<std::uint32_t>(j));
                        grid.At(j, i) = static_cast<float>(random_number) / static_cast<float>(UINT32_MAX);
                }
        }
}

constexpr void GenerateWhiteNoise_New_2(Lumen::Utility::ProceduralContentGeneration::Grid<float> &grid, 
                                  std::uint32_t random_seed) noexcept
{
        std::uint32_t random_number = random_seed;
        for (std::size_t i = 0; i < grid.GetHeight(); ++i) {
                for (std::size_t j = 0; j < grid.GetWidth(); ++j) {
                        random_number = Lumen::Utility::Random::LehmerRandomNumberGenerator2D(
                                random_number ^ static_cast<std::uint32_t>(i), static_cast<std::uint32_t>(j));
                        grid.At(j, i) = static_cast<float>(random_number) / static_cast<float>(UINT32_MAX);
                }
        }
}

constexpr void GenerateWhiteNoise_New_3(Lumen::Utility::ProceduralContentGeneration::Grid<float> &grid, 
                                  std::uint32_t random_seed) noexcept
{
        std::uint32_t random_number = random_seed;
        for (std::size_t i = 0; i < grid.GetWidth(); ++i) {
                for (std::size_t j = 0; j < grid.GetHeight(); ++j) {
                        random_number = Lumen::Utility::Random::LehmerRandomNumberGenerator2D(
                                random_number ^ static_cast<std::uint32_t>(i), static_cast<std::uint32_t>(j));
                        grid.At(i, j) = static_cast<float>(random_number) / static_cast<float>(UINT32_MAX);
                }
        }
}

constexpr void GenerateWhiteNoise_New_4(Lumen::Utility::ProceduralContentGeneration::Grid<float> &grid, 
                                  std::uint32_t random_seed) noexcept
{
        std::uint32_t random_number = random_seed;

        std::size_t index = 0;
        for (std::size_t i = 0; i < grid.GetWidth(); ++i) {
                for (std::size_t j = 0; j < grid.GetHeight(); ++j) {
                        random_number = Lumen::Utility::Random::LehmerRandomNumberGenerator2D(
                                random_number ^ static_cast<std::uint32_t>(i), static_cast<std::uint32_t>(j));
                        grid.At(index) = static_cast<float>(random_number) / static_cast<float>(UINT32_MAX);
                        ++index;
                }
        }
}

constexpr void GenerateSmoothNoise_Old(Lumen::Utility::ProceduralContentGeneration::Grid<float> &smooth_noise_octave,
                                   const Lumen::Utility::ProceduralContentGeneration::Grid<float> &base_noise,
                                   std::size_t octave) noexcept
{
        std::size_t sample_period = Lumen::Utility::ProceduralContentGeneration::Detail::PowerOfTwo(octave);
        float sample_frequency = 1.0f / static_cast<float>(sample_period);
 
        //for (std::size_t i = 0; (i * sample_period) < base_noise.GetWidth(); ++i) {
        for (std::size_t i = 0; i < base_noise.GetWidth(); ++i) {
                //calculate the horizontal sampling indices
                std::size_t sample_i0 = (i / sample_period) * sample_period;
                std::size_t sample_i1 = (sample_i0 + sample_period) % base_noise.GetWidth(); //wrap around
                float horizontal_blend = static_cast<float>(i - sample_i0) * sample_frequency;
        
                //for (std::size_t j = 0; (j * sample_period) < base_noise.GetHeight(); ++j) {
                for (std::size_t j = 0; j < base_noise.GetHeight(); ++j) {
                        //calculate the vertical sampling indices
                        std::size_t sample_j0 = (j / sample_period) * sample_period;
                        std::size_t sample_j1 = (sample_j0 + sample_period) % base_noise.GetHeight(); //wrap around
                        float vertical_blend = static_cast<float>(j - sample_j0) * sample_frequency;
                
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

constexpr void GenerateSmoothNoise_New(Lumen::Utility::ProceduralContentGeneration::Grid<float> &smooth_noise_octave,
                                   const Lumen::Utility::ProceduralContentGeneration::Grid<float> &base_noise,
                                   std::size_t octave) noexcept
{
        std::size_t sample_period = Lumen::Utility::ProceduralContentGeneration::Detail::PowerOfTwo(octave);
        float sample_frequency = 1.0f / static_cast<float>(sample_period);
 
        //for (std::size_t j = 0; (j * sample_period) < base_noise.GetHeight(); ++j) {
        for (std::size_t j = 0; j < base_noise.GetHeight(); ++j) {
                //calculate the vertical sampling indices
                std::size_t sample_j0 = (j / sample_period) * sample_period;
                std::size_t sample_j1 = (sample_j0 + sample_period) % base_noise.GetHeight(); //wrap around
                float vertical_blend = static_cast<float>(j - sample_j0) * sample_frequency;

                //for (std::size_t i = 0; (i * sample_period) < base_noise.GetWidth(); ++i) {
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

struct PerlinNoiseTotalAmplitude {
        float total_amplitude{0.0f};
};

[[nodiscard]] constexpr PerlinNoiseTotalAmplitude BlendSmoothNoiseOctaveTogether_Old(
        Lumen::Utility::ProceduralContentGeneration::PerlinNoiseData &perlin_noise_data) noexcept
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
                
                for (std::size_t i = 0; i < perlin_noise_data.GetWidth(); ++i) {
                        for (std::size_t j = 0; j < perlin_noise_data.GetHeight(); ++j) {
                                perlin_noise_data.GetPerlinNoise().At(i, j) += smooth_noise.At(i, j) * amplitude;
                        }
                }
        }

        return {total_amplitude};
}

[[nodiscard]] constexpr PerlinNoiseTotalAmplitude BlendSmoothNoiseOctaveTogether_New(
        Lumen::Utility::ProceduralContentGeneration::PerlinNoiseData &perlin_noise_data) noexcept
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
                
                for (std::size_t j = 0; j < perlin_noise_data.GetHeight(); ++j) {
                        for (std::size_t i = 0; i < perlin_noise_data.GetWidth(); ++i) {
                                perlin_noise_data.GetPerlinNoise().At(i, j) += smooth_noise.At(i, j) * amplitude;
                        }
                }
        }

        return {total_amplitude};
}

[[nodiscard]] constexpr PerlinNoiseTotalAmplitude BlendSmoothNoiseOctaveTogether_New_2(
        Lumen::Utility::ProceduralContentGeneration::PerlinNoiseData &perlin_noise_data) noexcept
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

constexpr void NormalizePerlinNoise_Old(
        Lumen::Utility::ProceduralContentGeneration::PerlinNoiseData &perlin_noise_data,
        const Lumen::Utility::ProceduralContentGeneration::Detail::PerlinNoiseTotalAmplitude &total_amplitude) noexcept
{
        for (std::size_t i = 0; i < perlin_noise_data.GetWidth(); ++i) {
                for (std::size_t j = 0; j < perlin_noise_data.GetHeight(); ++j) {
                        perlin_noise_data.GetPerlinNoise().At(i, j) /= total_amplitude.total_amplitude;
                }
        }
}

constexpr void NormalizePerlinNoise_New(
        Lumen::Utility::ProceduralContentGeneration::PerlinNoiseData &perlin_noise_data,
        const Lumen::Utility::ProceduralContentGeneration::Detail::PerlinNoiseTotalAmplitude &total_amplitude) noexcept
{
        for (std::size_t j = 0; j < perlin_noise_data.GetHeight(); ++j) {
                for (std::size_t i = 0; i < perlin_noise_data.GetWidth(); ++i) {
                        perlin_noise_data.GetPerlinNoise().At(i, j) /= total_amplitude.total_amplitude;
                }
        }
}

void TestPalinNoiseCase1(void)
{
#if 0
        {
                Lumen::Utility::ProceduralContentGeneration::Grid<float> base_noise{float_array[0]};

                // const auto start_old = std::chrono::high_resolution_clock::now();
                // GenerateWhiteNoise_Old(base_noise, 23646);
                // const auto end_old = std::chrono::high_resolution_clock::now();

                const auto start_new = std::chrono::high_resolution_clock::now();
                GenerateWhiteNoise_New(base_noise, 23646);
                const auto end_new = std::chrono::high_resolution_clock::now();

                const auto start_new_2 = std::chrono::high_resolution_clock::now();
                GenerateWhiteNoise_New_2(base_noise, 23646);
                const auto end_new_2 = std::chrono::high_resolution_clock::now();

                // const auto start_new_3 = std::chrono::high_resolution_clock::now();
                // GenerateWhiteNoise_New_3(base_noise, 23646);
                // const auto end_new_3 = std::chrono::high_resolution_clock::now();

                const auto start_new_4 = std::chrono::high_resolution_clock::now();
                GenerateWhiteNoise_New_4(base_noise, 23646);
                const auto end_new_4 = std::chrono::high_resolution_clock::now();

                //std::cout << "Old: " << std::chrono::duration<double>(end_old - start_old).count() << "\n";
                std::cout << "New: " << std::chrono::duration<double>(end_new - start_new).count() << "\n";
                std::cout << "New_2: " << std::chrono::duration<double>(end_new_2 - start_new_2).count() << "\n";
                //std::cout << "New_3: " << std::chrono::duration<double>(end_new_3 - start_new_3).count() << "\n";
                std::cout << "New_4: " << std::chrono::duration<double>(end_new_4 - start_new_4).count() << "\n";
        }
#endif

#if 0
        {
                Lumen::Utility::ProceduralContentGeneration::Grid<float> base_noise{float_array[0]};

                const auto gen_white_noise_start_old = std::chrono::high_resolution_clock::now();
                Lumen::Utility::ProceduralContentGeneration::GenerateWhiteNoise(base_noise, 23646);
                const auto gen_white_noise_end_old = std::chrono::high_resolution_clock::now();


                Lumen::Utility::ProceduralContentGeneration::Grid<float> smooth_noise_octave{float_array[1]};

                const auto start_old = std::chrono::high_resolution_clock::now();
                GenerateSmoothNoise_Old(smooth_noise_octave, base_noise, 1);
                const auto end_old = std::chrono::high_resolution_clock::now();

                const auto start_new = std::chrono::high_resolution_clock::now();
                GenerateSmoothNoise_New(smooth_noise_octave, base_noise, 1);
                const auto end_new = std::chrono::high_resolution_clock::now();

                const auto gen_smooth_noise_start = std::chrono::high_resolution_clock::now();
                Lumen::Utility::ProceduralContentGeneration::GenerateSmoothNoise(smooth_noise_octave, base_noise, 1);
                const auto gen_smooth_noise_end = std::chrono::high_resolution_clock::now();

                std::cout << "GenWhiteNoise: " << std::chrono::duration<double>(gen_white_noise_end_old - gen_white_noise_start_old).count() << "\n";

                std::cout << "Old: " << std::chrono::duration<double>(end_old - start_old).count() << "\n";
                std::cout << "New: " << std::chrono::duration<double>(end_new - start_new).count() << "\n";

                std::cout << "GenSmoothNoise: " << std::chrono::duration<double>(gen_smooth_noise_end - gen_smooth_noise_start).count() << "\n";

        }
#endif

#if 0
        {
                Lumen::Utility::ProceduralContentGeneration::Grid<float> base_noise{float_array[0]};

                const auto gen_white_noise_start_old = std::chrono::high_resolution_clock::now();
                Lumen::Utility::ProceduralContentGeneration::GenerateWhiteNoise(base_noise, 23646);
                const auto gen_white_noise_end_old = std::chrono::high_resolution_clock::now();

                std::vector<Lumen::Utility::ProceduralContentGeneration::Grid<float> > smooth_noise_octave{
                        { float_array[1] },
                        { float_array[2] },
                        { float_array[3] },
                        { float_array[4] },
                };

                Lumen::Utility::ProceduralContentGeneration::Grid<float> perlin_noise{float_array[OCTAVE - 1]};

                Lumen::Utility::ProceduralContentGeneration::PerlinNoiseData perlin_noise_data{
                        Lumen::Utility::ProceduralContentGeneration::Grid<float>{base_noise}, 
                        std::move(smooth_noise_octave),
                        Lumen::Utility::ProceduralContentGeneration::Grid<float>{perlin_noise},
                };

                const auto gen_smooth_noise_start = std::chrono::high_resolution_clock::now();
                Lumen::Utility::ProceduralContentGeneration::Detail::GenerateSmoothNoiseOctave(perlin_noise_data);
                const auto gen_smooth_noise_end = std::chrono::high_resolution_clock::now();

                // const auto start_old = std::chrono::high_resolution_clock::now();
                // const auto amp_old = BlendSmoothNoiseOctaveTogether_Old(perlin_noise_data);
                // (void)amp_old;
                // const auto end_old = std::chrono::high_resolution_clock::now();

                const auto start_new = std::chrono::high_resolution_clock::now();
                const auto amp_new = BlendSmoothNoiseOctaveTogether_New(perlin_noise_data);
                (void)amp_new;
                const auto end_new = std::chrono::high_resolution_clock::now();

                const auto start_new_2 = std::chrono::high_resolution_clock::now();
                const auto amp_new_2 = BlendSmoothNoiseOctaveTogether_New_2(perlin_noise_data);
                (void)amp_new_2;
                const auto end_new_2 = std::chrono::high_resolution_clock::now();

                const auto blend_smooth_noise_start = std::chrono::high_resolution_clock::now();
                const auto amp = Lumen::Utility::ProceduralContentGeneration::Detail::BlendSmoothNoiseOctaveTogether(perlin_noise_data);
                (void)amp;
                const auto blend_smooth_noise_end = std::chrono::high_resolution_clock::now();

                std::cout << "GenWhiteNoise: " << std::chrono::duration<double>(gen_white_noise_end_old - gen_white_noise_start_old).count() << "\n";

                std::cout << "GenSmoothNoise: " << std::chrono::duration<double>(gen_smooth_noise_end - gen_smooth_noise_start).count() << "\n";

                //std::cout << "Old: " << std::chrono::duration<double>(end_old - start_old).count() << "\n";
                std::cout << "New: " << std::chrono::duration<double>(end_new - start_new).count() << "\n";
                std::cout << "New_2: " << std::chrono::duration<double>(end_new_2 - start_new_2).count() << "\n";

                std::cout << "BlendSmoothNoise: " << std::chrono::duration<double>(blend_smooth_noise_end - blend_smooth_noise_start).count() << "\n";

        }
#endif

#if 0
        {
                Lumen::Utility::ProceduralContentGeneration::Grid<float> base_noise{float_array[0]};

                const auto gen_white_noise_start_old = std::chrono::high_resolution_clock::now();
                Lumen::Utility::ProceduralContentGeneration::GenerateWhiteNoise(base_noise, 23646);
                const auto gen_white_noise_end_old = std::chrono::high_resolution_clock::now();

                std::vector<Lumen::Utility::ProceduralContentGeneration::Grid<float> > smooth_noise_octave{
                        { float_array[1] },
                        { float_array[2] },
                        { float_array[3] },
                        { float_array[4] },
                };

                Lumen::Utility::ProceduralContentGeneration::Grid<float> perlin_noise{float_array[OCTAVE - 1]};

                Lumen::Utility::ProceduralContentGeneration::PerlinNoiseData perlin_noise_data{
                        Lumen::Utility::ProceduralContentGeneration::Grid<float>{base_noise}, 
                        std::move(smooth_noise_octave),
                        Lumen::Utility::ProceduralContentGeneration::Grid<float>{perlin_noise},
                };

                const auto gen_smooth_noise_start = std::chrono::high_resolution_clock::now();
                Lumen::Utility::ProceduralContentGeneration::Detail::GenerateSmoothNoiseOctave(perlin_noise_data);
                const auto gen_smooth_noise_end = std::chrono::high_resolution_clock::now();

                Lumen::Utility::ProceduralContentGeneration::Detail::SetPerlinNoiseToAllZeros(perlin_noise_data);

                const auto blend_smooth_noise_start = std::chrono::high_resolution_clock::now();
                const auto amp = Lumen::Utility::ProceduralContentGeneration::Detail::BlendSmoothNoiseOctaveTogether(perlin_noise_data);
                const auto blend_smooth_noise_end = std::chrono::high_resolution_clock::now();

                const auto start_old = std::chrono::high_resolution_clock::now();
                NormalizePerlinNoise_Old(perlin_noise_data, amp);
                const auto end_old = std::chrono::high_resolution_clock::now();

                const auto start_new = std::chrono::high_resolution_clock::now();
                NormalizePerlinNoise_New(perlin_noise_data, amp);
                const auto end_new = std::chrono::high_resolution_clock::now();


                std::cout << "GenWhiteNoise: " << std::chrono::duration<double>(gen_white_noise_end_old - gen_white_noise_start_old).count() << "\n";

                std::cout << "GenSmoothNoise: " << std::chrono::duration<double>(gen_smooth_noise_end - gen_smooth_noise_start).count() << "\n";

                std::cout << "BlendSmoothNoise: " << std::chrono::duration<double>(blend_smooth_noise_end - blend_smooth_noise_start).count() << "\n";
                
                std::cout << "Old: " << std::chrono::duration<double>(end_old - start_old).count() << "\n";
                std::cout << "New: " << std::chrono::duration<double>(end_new - start_new).count() << "\n";
        }
#endif

#if 1
        sf::RenderWindow window{sf::VideoMode{{1920, 1080}}, {"My window"}};
        window.setFramerateLimit(60);

        Lumen::Utility::ProceduralContentGeneration::Grid<float> base_noise{float_array[0]};
        Lumen::Utility::ProceduralContentGeneration::GenerateWhiteNoise(
                base_noise, 23646);
        
        std::vector<Lumen::Utility::ProceduralContentGeneration::Grid<float> > smooth_noise_octave{
                        //{ float_array[0] },
                        { float_array[1] },
                        { float_array[2] },
                        { float_array[3] },
                        { float_array[4] },
        };
        
        Lumen::Utility::ProceduralContentGeneration::Grid<float> perlin_noise{float_array[OCTAVE - 1]};
// // std::cout << "PerlinNoiseData: " << __LINE__ << "float_array[OCTAVE - 1] addr:" << float_array[OCTAVE - 1] << "\n";
// // std::cout << "PerlinNoiseData: " << __LINE__ << "perlin_noise addr:" << perlin_noise.GetData() << "\n";

        Lumen::Utility::ProceduralContentGeneration::PerlinNoiseData perlin_noise_data{
                Lumen::Utility::ProceduralContentGeneration::Grid<float>{base_noise}, 
                std::move(smooth_noise_octave),
                Lumen::Utility::ProceduralContentGeneration::Grid<float>{perlin_noise},
        };

        const auto gen_perlin_noise_start = std::chrono::high_resolution_clock::now();
        Lumen::Utility::ProceduralContentGeneration::GeneratePerlinNoise(perlin_noise_data);
        const auto gen_perlin_noise_end = std::chrono::high_resolution_clock::now();

        std::cout << "GenPerlinNoise: " << std::chrono::duration<double>(gen_perlin_noise_end - gen_perlin_noise_start).count() << "\n";
//std::cout << "PerlinNoiseData: " << __LINE__ << "\n";
        sf::VertexArray vertices{};

        float cell_size = 5.0f;
//std::cout << "PerlinNoiseData: " << __LINE__ << "\n";
        //SetVertexArrayPerlinNoise(vertices, base_noise, cell_size);
        SetVertexArrayPerlinNoise(vertices, perlin_noise_data.GetPerlinNoise(), cell_size);
//std::cout << "PerlinNoiseData: " << __LINE__ << "\n";
        //Lumen::Utility::ProceduralContentGeneration::Grid<float> perlin_noise{bool_array[1]};

        // Lumen::Utility::ProceduralContentGeneration::Grid<float> smooth_noise{float_array[1]};
        // Lumen::Utility::ProceduralContentGeneration::GenerateSmoothNoise(smooth_noise, base_noise, 2);
        // SetVertexArrayPerlinNoise(vertices, smooth_noise, cell_size);


        // Lumen::Utility::ProceduralContentGeneration::Detail::GenerateSmoothNoiseOctave(perlin_noise_data);
        // SetVertexArrayPerlinNoise(vertices, perlin_noise_data.GetSmoothNoise(1), cell_size);


        while (window.isOpen()) {
                for (std::optional<sf::Event> optional_event = window.pollEvent(); optional_event.has_value(); optional_event = window.pollEvent()) {
                        sf::Event &event = optional_event.value();
//std::cout << "Event\n";
                        if (event.is<sf::Event::KeyPressed>()) {
                                const sf::Event::KeyPressed &key_pressed_data = *event.getIf<sf::Event::KeyPressed>();

                                if (sf::Keyboard::Key::Escape == key_pressed_data.code) {
                                        window.close();
                                } else if (sf::Keyboard::Key::Space == key_pressed_data.code) {
                                        //std::cout << "Space\n";
                                        // Lumen::Utility::ProceduralContentGeneration::GenerateSmoothNoise(
                                        //         smooth_noise, base_noise, 1);
// std::cout << "PerlinNoiseData: " << __LINE__ << "perlin_noise addr:" << perlin_noise.GetData() << "\n";
//                                         SetVertexArrayPerlinNoise(vertices, perlin_noise, cell_size);
                                } else if (sf::Keyboard::Key::W == key_pressed_data.code ||
                                           sf::Keyboard::Key::Up == key_pressed_data.code) {

                                        auto veiw = window.getView();
                                        veiw.move({0.0f, -10.0f});
                                        window.setView(veiw);

                                } else if (sf::Keyboard::Key::S == key_pressed_data.code ||
                                           sf::Keyboard::Key::Down == key_pressed_data.code) {
                                        auto veiw = window.getView();
                                        veiw.move({0.0f, 10.0f});
                                        window.setView(veiw);
                                } else if (sf::Keyboard::Key::A == key_pressed_data.code ||
                                           sf::Keyboard::Key::Left == key_pressed_data.code) {
                                        auto veiw = window.getView();
                                        veiw.move({-10.0f, 0.0f});
                                        window.setView(veiw);  
                                } else if (sf::Keyboard::Key::D == key_pressed_data.code ||
                                           sf::Keyboard::Key::Right == key_pressed_data.code) {
                                        auto veiw = window.getView();
                                        veiw.move({10.0f, 0.0f});
                                        window.setView(veiw);
                                }

                        } else if (event.is<sf::Event::Closed>()) {
                                window.close();
                        }
                }
//std::cout << "PerlinNoiseData: " << __LINE__ << "\n";
                window.clear(sf::Color::Yellow);
                window.draw(vertices);
                window.display();
//std::cout << "PerlinNoiseData: " << __LINE__ << "\n";
        }
#endif
}

int main(void)
{
        TestPalinNoiseCase1();
        return 0;
}
