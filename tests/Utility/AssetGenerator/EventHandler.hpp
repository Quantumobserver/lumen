
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
                                std::cout << __LINE__ << "\n";
                                if (asset_generator_data.commands.has_value()) {
                                        asset_generator_data.commands.value() += "";
                                } else {
                                        asset_generator_data.commands = "";
                                }
                                asset_generator_data.is_commands_input = true;
                        } else if (sf::Keyboard::Key::Backspace == key_pressed_data.code){
                                if (asset_generator_data.commands.has_value()) {
                                        std::cout << __LINE__ << "\n";
                                        if (!asset_generator_data.commands.value().empty()) {
                                                asset_generator_data.commands.value().pop_back();
                                                std::cout << asset_generator_data.commands.value() << "\n";
                                        }
                                        asset_generator_data.text.value().setString("CMD: " + asset_generator_data.commands.value());
                                }
                        } else if (sf::Keyboard::Key::Enter == key_pressed_data.code) {
                                if (asset_generator_data.commands.has_value()) {
                                        DoCommands(asset_generator_data);
                                }
                                asset_generator_data.is_commands_input = false;
                                asset_generator_data.text.value().setString("CMD: ");

                        } 
                } else if (event.is<sf::Event::TextEntered>()){
                        std::cout << __LINE__ << "\n";
                        const sf::Event::TextEntered &text_entered = *event.getIf<sf::Event::TextEntered>();
                        std::cout <<text_entered.unicode << "\n";
                        if (asset_generator_data.commands.has_value() &&
                            text_entered.unicode != 8 && text_entered.unicode != 58) {
                                asset_generator_data.commands.value() += static_cast<char>(text_entered.unicode);
                                asset_generator_data.text.value().setString("CMD: " + asset_generator_data.commands.value());
                        }
                } else if (event.is<sf::Event::Closed>()) {
                        asset_generator_data.Exit();
                }
        }
}
