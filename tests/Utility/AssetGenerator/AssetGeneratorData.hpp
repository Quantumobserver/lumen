
#pragma once

#include "UI/AssetGeneratorUIM.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

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
        sf::RectangleShape commands_input_box;
        Lumen::Utility::AssetGenerator::UI::AssetGeneratorUIM * ui_manager{nullptr};
        bool is_commands_input{false};
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
