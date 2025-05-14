
#include <Utility/ProceduralContentGeneration/CellularAutomata.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <iostream>

void SetVertexArrayCelluarAutomata(
        sf::VertexArray &vertices,
        const Lumen::Utility::Grid<bool> &cellular_automata_data,
        const float cell_size) noexcept
{
        vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
        vertices.resize(cellular_automata_data.GetSize() * 6);

        std::size_t cellular_automata_index{0};
        std::size_t vertex_index{0};

        const sf::Color alive_color{sf::Color::Green};
        const sf::Color dead_color{sf::Color::Blue};

        for (const auto &cell : cellular_automata_data) {
                const std::size_t x = cellular_automata_index % cellular_automata_data.GetWidth();
                const std::size_t y = cellular_automata_index / cellular_automata_data.GetWidth();

                const float position_left_x = static_cast<float>(x) * cell_size;
                const float position_top_y = static_cast<float>(y) * cell_size;
                const float position_right_x = static_cast<float>(x + 1) * cell_size;
                const float position_bottom_y = static_cast<float>(y + 1) * cell_size;

                vertices[vertex_index].position = sf::Vector2f{position_left_x, position_top_y};
                vertices[vertex_index].color = cell ? alive_color : dead_color;

                vertices[vertex_index + 1].position = sf::Vector2f{position_right_x, position_top_y};
                vertices[vertex_index + 1].color = cell ? alive_color : dead_color;

                vertices[vertex_index + 2].position = sf::Vector2f{position_right_x, position_bottom_y};
                vertices[vertex_index + 2].color = cell ? alive_color : dead_color;

                vertices[vertex_index + 3].position = sf::Vector2f{position_left_x, position_top_y};
                vertices[vertex_index + 3].color = cell ? alive_color : dead_color;

                vertices[vertex_index + 4].position = sf::Vector2f{position_right_x, position_bottom_y};
                vertices[vertex_index + 4].color = cell ? alive_color : dead_color;

                vertices[vertex_index + 5].position = sf::Vector2f{position_left_x, position_bottom_y};
                vertices[vertex_index + 5].color = cell ? alive_color : dead_color;

                ++cellular_automata_index;
                vertex_index += 6;
        }
}

void TestCACase1(void)
{
        sf::RenderWindow window{sf::VideoMode{{1920, 1080}}, {"My window"}};
        window.setFramerateLimit(60);

        constexpr const std::size_t SCALE = 14;
        constexpr const std::size_t CELLULAR_AUTOMATA_WIDTH = 80 * SCALE;
        constexpr const std::size_t CELLULAR_AUTOMATA_HEIGHT = 60 * SCALE;

        bool bool_array[CELLULAR_AUTOMATA_WIDTH][CELLULAR_AUTOMATA_HEIGHT] = {{}};

        Lumen::Utility::Grid<bool> cellular_automata_data{bool_array};
        Lumen::Utility::ProceduralContentGeneration::InitCellularAutomata(
                cellular_automata_data, 132565, (UINT32_MAX / 10) * 4);

        sf::VertexArray vertices{};

        float cell_size = 1.0f;
        SetVertexArrayCelluarAutomata(vertices, cellular_automata_data, cell_size);

        while (window.isOpen()) {
                for (std::optional<sf::Event> optional_event = window.pollEvent(); optional_event.has_value(); optional_event = window.pollEvent()) {
                        sf::Event &event = optional_event.value();
std::cout << "Event\n";
                        if (event.is<sf::Event::KeyPressed>()) {
                                const sf::Event::KeyPressed &key_pressed_data = *event.getIf<sf::Event::KeyPressed>();

                                if (sf::Keyboard::Key::Escape == key_pressed_data.code) {
                                        window.close();
                                } else if (sf::Keyboard::Key::Space == key_pressed_data.code) {
                                        std::cout << "Space\n";
                                        Lumen::Utility::ProceduralContentGeneration::UpdateCellularAutomata(
                                                cellular_automata_data, 3, 4);
                                        SetVertexArrayCelluarAutomata(vertices, cellular_automata_data, cell_size);
                                }
                        } else if (event.is<sf::Event::Closed>()) {
                                window.close();
                        }
                }

                window.clear(sf::Color::Yellow);
                window.draw(vertices);
                window.display();
        }
}

int main(void)
{
        TestCACase1();
        return 0;
}
