
#include <Action/ActionManager.hpp>
#include <UI/Components/Button.hpp>
#include <UI/Components/Menu.hpp>
#include <UI/UIManager.hpp>
#include <ResourceManager/ResourceManager.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <iostream>
#include <chrono>

#include <SFML/Window/Window.hpp>
#include <SFML/Window/Keyboard.hpp>

void TestUIManagerCase1(void) noexcept 
{
        sf::RenderWindow window{sf::VideoMode{{800, 600}}, {"My window"}};

        Lumen::Action::ActionManager action_manager{};
        action_manager.Init();

        Lumen::ResourceManager::ResourceManager resource_manager{};
        resource_manager.Init();
        resource_manager.LoadFontFromFile("../../src/Assets/Fonts/DroidSansFallback.ttf", Lumen::ResourceManager::FontID::DROID_FONT);
        resource_manager.LoadTextureFromFile("../../src/Assets/Images/TestUITexture.jpeg", Lumen::ResourceManager::TextureID::UI_TEXTURE);
        {
                sf::Texture &texture = resource_manager.GetTexture(Lumen::ResourceManager::TextureID::UI_TEXTURE);
                sf::Sprite sprite{texture};
                resource_manager.AddSprite(std::move(sprite), static_cast<Lumen::ResourceManager::SpriteID>(0));
        }

        Lumen::UI::UIManager ui_manager{};

        ui_manager.Init(resource_manager);

        // auto &menu = ui_manager.CreateMenu();

        // menu.SetVisible(true);
        // menu.SetMenuBoundingBox({{50, 70}});

        // menu.ComputeBoundingBoxAndTransform({{0.0f, 0.0f}});

        //auto last_update_time_point = std::chrono::high_resolution_clock::now();
        while (window.isOpen()) {
                for (std::optional<sf::Event> optional_event = window.pollEvent(); optional_event.has_value(); optional_event = window.pollEvent()) {
                        sf::Event &event = optional_event.value();

                        if (event.is<sf::Event::KeyPressed>()) {
                                const sf::Event::KeyPressed &key_pressed_data = *event.getIf<sf::Event::KeyPressed>();

                                if (sf::Keyboard::Key::Escape == key_pressed_data.code) {
                                        window.close();
                                }
                        } else if (event.is<sf::Event::Closed>()) {
                                window.close();
                        }

                        action_manager.CreateActionFromEvent(event);
                }

                window.clear();
                //ui_manager.Render(window);
                //window.draw(ui_manager.GetDefaultSprite());
                sf::Text text{*ui_manager.GetDefaultFont(), "Hello, World!"};
                window.draw(text);
                window.display();

                // const auto current_time_point = std::chrono::high_resolution_clock::now();
                // //const std::chrono::duration<float> fp_ms
                // const auto delta_time = std::chrono::duration<float>{current_time_point - last_update_time_point}.count();
                // last_update_time_point = current_time_point;
                // menu.Update(delta_time);
                
                // if (!action_manager.IsSelectionActionHappened()) {
                //         continue;
                // }
                // const auto &selection_action = action_manager.GetSelectionAction();
                // menu.DoSelectionAction({selection_action, selection_action.position});


                // if (2 == i) {
                //         menu.SetPosition({std::abs(i) * 100, std::abs(i) * 100});
                // } else if (-2 == i) {
                //         menu.SetPosition({400, 300});
                // }
                action_manager.ResetActionBuffer();
        }
}

int main(void)
{
        //TestUIManagerCase1();
        TestUIManagerCase1();
        std::cout << "Done\n";
}
