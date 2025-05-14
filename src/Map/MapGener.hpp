#pragma once

#include <Utility/ProceduralContentGeneration/PerlinNoise.hpp>

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>

#include <chrono>
#include <iostream>

namespace Lumen
{
namespace Map
{
namespace MapGener
{

class MapGener
{
public:

void SetVertexArrayPerlinNoise(
    sf::VertexArray &vertices,
    const Lumen::Utility::Grid<float> &perlin_noise,
    const float cell_size) noexcept
{
        vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
        vertices.resize(perlin_noise.GetSize() * 6);

        std::size_t cellular_automata_index{0};
        std::size_t vertex_index{0};

        for (const auto &cell : perlin_noise)
        {
                const std::size_t x = cellular_automata_index % perlin_noise.GetWidth();
                const std::size_t y = cellular_automata_index / perlin_noise.GetWidth();

                const float position_left_x = static_cast<float>(x) * cell_size;
                const float position_top_y = static_cast<float>(y) * cell_size;
                const float position_right_x = static_cast<float>(x + 1) * cell_size;
                const float position_bottom_y = static_cast<float>(y + 1) * cell_size;

                sf::Color color{};

                if (cell < 0.3f)
                {
                        color = {
                            // static_cast<std::uint8_t>((cell * sf::Color::Green.r)),
                            // static_cast<std::uint8_t>((cell * sf::Color::Green.g)),
                            // static_cast<std::uint8_t>((cell * sf::Color::Green.b)),
                            // static_cast<std::uint8_t>((sf::Color::Green.a)),

                            static_cast<std::uint8_t>((0)),
                            static_cast<std::uint8_t>((cell * sf::Color::Green.g) + 127),
                            static_cast<std::uint8_t>((cell * 127)),
                            static_cast<std::uint8_t>((sf::Color::Green.a)),
                        };
                }
                else
                {
                        color = sf::Color::Green;
                }

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

    MapGener() = default;
    ~MapGener() = default;

    void GenerateMap(void) noexcept;
};

} // namespace MapGener   
} // namespace Map    
} // namespace Lumen
