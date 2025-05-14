
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
        asset_generator_data.window.create(sf::VideoMode{{1920, 1080}}, {"Asset Generator"});
        asset_generator_data.window.setFramerateLimit(60);
        if (!asset_generator_data.font.openFromFile("../../../src/Assets/Fonts/DroidSansFallback.ttf")) {
                std::cerr << "Failed to load font\n";
                std::abort();
        }
}

void AssetGenerator(void) noexcept
{
        AssetGeneratorData asset_generator_data;

        InitializeAssetGeneratorData(asset_generator_data);

        while (asset_generator_data.IsRunning()) {
                EventHandler(asset_generator_data);

                asset_generator_data.window.clear();

                if (asset_generator_data.text.has_value()) {
                        asset_generator_data.window.draw(asset_generator_data.text.value());
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