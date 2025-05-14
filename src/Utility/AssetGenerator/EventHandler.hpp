
#pragma once

#include "AssetGeneratorData.hpp"
#include "DoCommand.hpp"

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

void EventHandler(AssetGeneratorData &asset_generator_data) noexcept
{
        for (std::optional<sf::Event> optional_event = asset_generator_data.window.pollEvent();
             optional_event.has_value(); optional_event = asset_generator_data.window.pollEvent()) {
                sf::Event &event = optional_event.value();

                if (event.is<sf::Event::KeyPressed>()) {
                        const sf::Event::KeyPressed &key_pressed_data = *event.getIf<sf::Event::KeyPressed>();

                        if (sf::Keyboard::Key::Escape == key_pressed_data.code) {
                                asset_generator_data.Exit();
                        } if ((sf::Keyboard::Key::Semicolon == key_pressed_data.code) &&
                              (true == key_pressed_data.shift)) {
                                if (asset_generator_data.commands.has_value()) {
                                        asset_generator_data.commands.value() += ":";
                                } else {
                                        asset_generator_data.commands = ":";
                                }
                        } else if (sf::Keyboard::Key::Enter == key_pressed_data.code) {
                                if (asset_generator_data.commands.has_value()) {
                                        DoCommands(asset_generator_data);
                                }
                        }
                } else if (event.is<sf::Event::Closed>()) {
                        asset_generator_data.Exit();
                }
        }
}
