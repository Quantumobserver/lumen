
#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include <string>
#include <string_view>
#include <unordered_map>
#include <functional>
#include <optional>

#include <iostream>

//std::unordered_map<std::string, std::string> map;

struct AssetGeneratorData {
        sf::RenderWindow window;
        sf::Font font;
        std::optional<sf::Text> text;
        std::optional<std::string> commands;
        bool is_running{true};

        constexpr bool IsRunning(void) const noexcept
        {
                return this->is_running;
        }

        constexpr void Exit(void) noexcept
        {
                this->is_running = false;
        }
};
