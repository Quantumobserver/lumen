
#include <Utility/ProceduralContentGeneration/PerlinNoise.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>

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

        //const sf::Color alive_color{sf::Color::White};
        //const sf::Color dead_color{sf::Color::Red};

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

constexpr const std::size_t SCALE = 5;
constexpr const std::size_t GRID_WIDTH = 80 * SCALE;
constexpr const std::size_t GRID_HEIGHT = 60 * SCALE;
constexpr const std::size_t OCTAVE = 6;

float float_array[OCTAVE][GRID_WIDTH][GRID_HEIGHT] = {{}};

void TestPalinNoiseCase1(void)
{
        sf::RenderWindow window{sf::VideoMode{{1920, 1080}}, {"My window"}};
        window.setFramerateLimit(60);

        Lumen::Utility::ProceduralContentGeneration::Grid<float> base_noise{float_array[0]};
        Lumen::Utility::ProceduralContentGeneration::GenerateWhiteNoise(
                base_noise, 132565);
        
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

        Lumen::Utility::ProceduralContentGeneration::GeneratePerlinNoise(perlin_noise_data);
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
                                        std::cout << "Space\n";
                                        // Lumen::Utility::ProceduralContentGeneration::GenerateSmoothNoise(
                                        //         smooth_noise, base_noise, 1);
// std::cout << "PerlinNoiseData: " << __LINE__ << "perlin_noise addr:" << perlin_noise.GetData() << "\n";
//                                         SetVertexArrayPerlinNoise(vertices, perlin_noise, cell_size);
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
}

int main(void)
{
        TestPalinNoiseCase1();
        return 0;
}
