
#include "AssetGeneratorData.hpp"
#include "EventHandler.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include <cstdlib>
#include <string>
#include <string_view>
#include <unordered_map>
#include <functional>
#include <optional>

#include <iostream>

//std::unordered_map<std::string, std::string> map;

void InitializeAssetGeneratorData(AssetGeneratorData &asset_generator_data) noexcept
{
        asset_generator_data.window.create(sf::VideoMode{{800, 720}}, {"Asset Generator"});
        asset_generator_data.window.setFramerateLimit(60);
        if (!asset_generator_data.font.openFromFile("../../../src/Assets/Fonts/DroidSansFallback.ttf")) {
                std::cerr << "Failed to load font\n";
                std::abort();
        }
        auto window_size = asset_generator_data.window.getSize();
        asset_generator_data.text = sf::Text{ asset_generator_data.font, "CMD:", 15 };

        float commands_input_box_height = 50.f;
        float commands_input_box_width = static_cast<float>(window_size.x);

        asset_generator_data.commands_input_box = sf::RectangleShape{{commands_input_box_width,commands_input_box_height}};
        asset_generator_data.commands_input_box.setPosition({0.f, 5.f});
        asset_generator_data.commands_input_box.setFillColor(sf::Color(255, 255, 255, 70));
        asset_generator_data.commands_input_box.setOutlineColor(sf::Color::Black);

        auto commands_input_box_position = asset_generator_data.commands_input_box.getPosition();
        auto text_bounds = asset_generator_data.text.value().getLocalBounds();

        // std::cout << "text position: " << commands_input_box_position.x << ", " << commands_input_box_position.y << "\n";
        
        auto text_position = asset_generator_data.text.value().getPosition();

        text_position = commands_input_box_position - text_bounds.position;

        asset_generator_data.text.value().setPosition({
                text_position.x + 5.f,
                text_position.y + (commands_input_box_height/2.f) - (text_bounds.size.y/2.f)

        });

}

void AssetGenerator(void) noexcept
{
        AssetGeneratorData asset_generator_data;

        InitializeAssetGeneratorData(asset_generator_data);

        while (asset_generator_data.IsRunning()) {
                EventHandler(asset_generator_data);

                asset_generator_data.window.clear();
                if (asset_generator_data.is_commands_input) {
                // std::cout << "Drawing\n";
                        if (asset_generator_data.text.has_value()) {
                                asset_generator_data.window.draw(asset_generator_data.text.value());
                        }
                        asset_generator_data.window.draw(asset_generator_data.commands_input_box);
                }

                asset_generator_data.window.display();
        }
}

int main(void)
{
        AssetGenerator();

        std::cout << "Done.\n";
        return 0;
}