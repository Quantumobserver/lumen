
#pragma once

#include <Scene/Scene.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <cassert>
#include <iostream>
#include <array>
#include <string_view>

namespace Lumen {
namespace Scene {

namespace GamePlayLayer {

class TestBackgroundLayer : public Lumen::LayerStack::BaseLayer {
private:
        using this_t = TestBackgroundLayer;

        Lumen::Scene::BaseScene *m_game_play_scene;
        sf::VertexArray m_tile_map_vertices;

        static constexpr const std::size_t TILE_MAP_WIDTH = 25;
        static constexpr const float TILE_SIZE = 20.0f;

        // W : water
        // G : grass
        // S : stone
        // L : Land
        static constexpr const std::string_view TILE_MAP = 
                "WWWWWWWWWWWGGGGGGGGGGLLLLLLLLLLSSSSSSSSSS"
                "WWWWWWWWWWGGGGGGGGGGGLLLLLLLLLSSSSSSSSSSS"
                "WWWWWWWWWWGGGGGGGGGGGLLLLLLLLLSSSSSSSSSSS"
                "WWWWWWWWWWGGGGGGGGGGGLLLLLLLLLSSSSSSSSSSS"
                "WWWWWWWWWWGGGGGGGGGGGLLLLLLLLLSSSSSSSSSSS";
        
        constexpr static const std::array<sf::Color, 256> &GetTileColors(void) noexcept
        {
                static std::array<sf::Color, 256> tile_colors{
                        sf::Color::Transparent
                };
                tile_colors['W'] = sf::Color::Blue;
                tile_colors['G'] = sf::Color::Green;
                tile_colors['S'] = sf::Color{128, 128, 128};
                tile_colors['L'] = sf::Color{128, 128, 0};

                return tile_colors;
        }


public:
        constexpr TestBackgroundLayer(Lumen::Scene::BaseScene *game_play_scene) noexcept
        : m_game_play_scene{game_play_scene}, m_tile_map_vertices{sf::PrimitiveType::Triangles, this_t::TILE_MAP.size() * 6}
        {
                assert(nullptr != game_play_scene);
        }

        constexpr void Update(void) noexcept override
        {
                //std::cout << "[ExampleLayer2]: Update\n";
        }

        [[nodiscard]] constexpr Lumen::LayerStack::BaseLayer::DoActionResult DoAction(Lumen::Action::Action action) noexcept override
        {
                switch (action.action_name) {
                case Lumen::Action::ActionName::MOVE_RIGHT:
                        //std::cout << "[GamePlayScene:TestBackgroundLayer]: MOVE_RIGHT\n";
                        return Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked;
                
                default:
                        break;
                }
                return Lumen::LayerStack::BaseLayer::DoActionResult::NotHandledOrNotBlocked;
        }

        constexpr void Render(void) noexcept override
        {
                //std::cout << "[ExampleLayer2]: Render\n";
                for (std::size_t i = 0, j = 0; i < this_t::TILE_MAP.size(); ++i, j += 6) {
                        const auto &color = this_t::GetTileColors()[static_cast<std::size_t>(this_t::TILE_MAP[i])];
                        const float index_x = static_cast<float>(i % this_t::TILE_MAP_WIDTH);
                        const float index_y = static_cast<float>(i / this_t::TILE_MAP_WIDTH);
                        const float position_x = (index_x * this_t::TILE_SIZE);
                        const float position_y = (index_y * this_t::TILE_SIZE);
                        const float position_x_plus_tile_size = position_x + this_t::TILE_SIZE;
                        const float position_y_plus_tile_size = position_y + this_t::TILE_SIZE;
                        // if (i==0){
                        //         std::cout << "position_x: " << position_x << ", position_y: " << position_y << "\n";
                        //         std::cout << "position_x_plus_tile_size: " 
                        //                   << position_x_plus_tile_size 
                        //                   << ", position_y_plus_tile_size: " 
                        //                   << position_y_plus_tile_size << "\n";
                        // }
                        this->m_tile_map_vertices[j].position = sf::Vector2f{position_x, position_y};
                        this->m_tile_map_vertices[j].color = color;

                        this->m_tile_map_vertices[j + 1].position = sf::Vector2f{position_x, position_y_plus_tile_size};
                        this->m_tile_map_vertices[j + 1].color = color;

                        this->m_tile_map_vertices[j + 2].position = sf::Vector2f{position_x_plus_tile_size, position_y};
                        this->m_tile_map_vertices[j + 2].color = color;
                        
                        this->m_tile_map_vertices[j + 3].position = sf::Vector2f{position_x_plus_tile_size, position_y};
                        this->m_tile_map_vertices[j + 3].color = color;
                        
                        this->m_tile_map_vertices[j + 4].position = sf::Vector2f{position_x_plus_tile_size, position_y_plus_tile_size};
                        this->m_tile_map_vertices[j + 4].color = color;
                        
                        this->m_tile_map_vertices[j + 5].position = sf::Vector2f{position_x, position_y_plus_tile_size};
                        this->m_tile_map_vertices[j + 5].color = color;
                }
                this->m_game_play_scene->m_window_ptr->draw(m_tile_map_vertices);
        }

};

} // namespace GamePlayLayer
} // namespace Scene
} // namespace Lumen