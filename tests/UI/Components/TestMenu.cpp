
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

void TestMenuCase1(void) noexcept 
{
        sf::RenderWindow window{sf::VideoMode{{800, 600}}, {"My window"}};
        window.setFramerateLimit(60);

        Lumen::Action::ActionManager action_manager{};
        action_manager.Init();

        Lumen::ResourceManager::ResourceManager resource_manager{};
        resource_manager.Init();
        resource_manager.LoadFontFromFile("../../../src/Assets/Fonts/DroidSansFallback.ttf", Lumen::ResourceManager::FontID::DROID_FONT);
        resource_manager.LoadTextureFromFile("../../../src/Assets/Images/TestUITexture.jpeg", Lumen::ResourceManager::TextureID::UI_TEXTURE);
        {
                sf::Texture &texture = resource_manager.GetTexture(Lumen::ResourceManager::TextureID::UI_TEXTURE);
                sf::Sprite sprite{texture, sf::IntRect{{0, 0}, sf::Vector2i{texture.getSize()} / 2}};
                resource_manager.AddSprite(std::move(sprite), static_cast<Lumen::ResourceManager::SpriteID>(0));
        }

        Lumen::UI::UIManager ui_manager{};
//std::cout << __FILE__ " :" << __LINE__ << "\n";
        ui_manager.Init(resource_manager);

// sf::Text text{resource_manager.GetFont(Lumen::ResourceManager::FontID::DROID_FONT), "Hello, World!"};
// //text.setPosition({0.0f, 0.0f});
// //text.setScale({5.5f, 5.5f});
// text.setScale({0.5f, 0.5f});
// text.setPosition(sf::Vector2f{0.0f, 0.0f} -
//         sf::Vector2f{text.getLocalBounds().position.x * text.getScale().x,
//                      text.getLocalBounds().position.y * text.getScale().y});
// sf::RectangleShape text_box{};
// text_box.setSize(text.getLocalBounds().size);
// text_box.setPosition(text.getGlobalBounds().position);
// text_box.setFillColor(sf::Color::Transparent);
// text_box.setOutlineColor(sf::Color::Red);
// text_box.setOutlineThickness(1.0f);
// window.draw(text_box);
// text_box.setSize({text.getLocalBounds().size.x * text.getScale().x, text.getLocalBounds().size.y * text.getScale().y});
// text_box.setOutlineColor(sf::Color::Green);
// window.draw(text_box);
// window.draw(text);
// window.display();
// std::string i;
// std::cin >> i;

        ui_manager.SetDrawBoundingBox(true);

//std::cout << __FILE__ " :" << __LINE__ << "\n";
        auto &menu = ui_manager.CreateMenu();

        menu.SetVisible(true);
        
        menu.SetMenuBoundingBox({{50, 70}});
        menu.SetMenuRelativeTopLeftPosition({400, 300});

        Lumen::UI::Component::Menu::Layout layout{};
        layout.direction_type_tag = Lumen::UI::Component::Menu::Layout::DirectionTypeTag::VERTICAL;
        layout.spacing_type_tag = Lumen::UI::Component::Menu::Layout::SpacingTypeTag::FIXED;
        layout.alignment_type_tag = Lumen::UI::Component::Menu::Layout::AlignmentTypeTag::TOP_LEFT;
        layout.fixed_spacing_data = {3, 5, 10};
// std::cout << __FILE__ " :" << __LINE__ << "\n";
        menu.SetLayout(layout);
// std::cout << __FILE__ " :" << __LINE__ << "\n";

        {
                auto &button = menu.CreateMenuButton();
                button.SetVisible(true);
                button.SetText({*ui_manager.GetDefaultFont(), "Start Game"});
                button.SetBackground(ui_manager.GetDefaultSprite());
                button.SetActionHandler(
                        [](void *data, const Lumen::UI::Component::RelativeSelectionAction &relative_selection_action) noexcept
                        -> Lumen::UI::Component::MenuButton::ActionResult {
                                (void)data;
                                (void)relative_selection_action;
                                if (Lumen::Action::SelectionAction::SelectionActionTypeTag::PRESS ==
                                    relative_selection_action.selection_action.selection_action_type) {
                                        std::cout << "Start Game\n";
                                }

                                return Lumen::UI::Component::MenuButton::ActionResult{
                                        true, true, std::nullopt,
                                };
                        }, nullptr);
        }

        {
                auto &button = menu.CreateMenuButton();
                button.SetVisible(true);
                //button.SetText({*ui_manager.GetDefaultFont(), "++++++++++"});
        }

        {
                auto &button = menu.CreateMenuButton();
                button.SetVisible(true);
                //button.SetText({*ui_manager.GetDefaultFont(), "End Game"});
                button.SetText(ui_manager.GetDefaultFont(), "End Game");

        }

        menu.SetMenuButtonBoundingBox({{200, 50}});
        //menu.SetMenuButtonBoundingBox({{50, 10}});
        menu.ComputeBoundingBoxAndTransform({{0.0f, 0.0f}});

        {
                auto &v_menu = ui_manager.CreateMenu();
                v_menu.SetVisible(true);
                v_menu.SetMenuBoundingBox({{100, 140}});
                v_menu.SetMenuButtonBoundingBox({{100, 20}});
                v_menu.SetMenuRelativeTopLeftPosition({10.0f, 30.0f});

                Lumen::UI::Component::Menu::Layout layout{};
                layout.direction_type_tag = Lumen::UI::Component::Menu::Layout::DirectionTypeTag::VERTICAL;
                layout.spacing_type_tag = Lumen::UI::Component::Menu::Layout::SpacingTypeTag::UNIFORM_DISTRIBUTION;
                layout.alignment_type_tag = Lumen::UI::Component::Menu::Layout::AlignmentTypeTag::TOP_LEFT;
                //layout.fixed_spacing_data = {3, 5, 10};
                v_menu.SetLayout(layout);


                {
                        auto &sub_menu = v_menu.CreateSubMenu();
                        sub_menu.SetVisible(true);
                        sub_menu.SetMenuBoundingBox({{100, 140}});
                        sub_menu.SetMenuButtonBoundingBox({{100, 20}});
                        sub_menu.SetMenuRelativeTopLeftPosition({10.0f, 30.0f});

                        auto &button = v_menu.CreateMenuButton();
                        button.SetVisible(true);
                        button.SetText(ui_manager.GetDefaultFont(), "+++++");
                        //button.SetBackground(ui_manager.GetDefaultSprite());
                        button.SetActionHandler(
                                [&button](void *data, const Lumen::UI::Component::RelativeSelectionAction &relative_selection_action) noexcept
                                -> Lumen::UI::Component::MenuButton::ActionResult {
                                        auto &sub_menu = *static_cast<Lumen::UI::Component::Menu *>(data);
                                        const auto selection_type =
                                                relative_selection_action.selection_action.selection_action_type;
                                        if (Lumen::Action::SelectionAction::SelectionActionTypeTag::PRESS ==
                                            selection_type) {
                                                std::cout << "++++\n";
                                        }
                                        
                                        Lumen::UI::Component::SelectedSubMenu selected_sub_menu{};
                                        selected_sub_menu.menu_button_ptr = &button;
                                        selected_sub_menu.sub_menu_ptr = &sub_menu;

                                        switch (selection_type) {
                                        case Lumen::Action::SelectionAction::SelectionActionTypeTag::PRESS: {

                                                /* code */
                                                selected_sub_menu.selection_type = Lumen::UI::Component::SelectedSubMenu::SelectionTypeTag::PRESS;                       
                                                break;
                                        }
                                        
                                        default:
                                                break;
                                        }

                                        Lumen::UI::Component::MenuButton::ActionResult result{
                                                true, true, {selected_sub_menu},
                                        };

                                        return result;
                                }, &sub_menu);
                }

                {
                        v_menu.CreateMenuButton();
                        
                }

                v_menu.ComputeBoundingBoxAndTransform({{0.0f, 0.0f}});
        }

        auto last_update_time_point = std::chrono::high_resolution_clock::now();

        while (window.isOpen()) {
                for (std::optional<sf::Event> optional_event = window.pollEvent(); optional_event.has_value(); optional_event = window.pollEvent()) {
                        sf::Event &event = optional_event.value();

                        if (event.is<sf::Event::KeyPressed>()) {
                                const sf::Event::KeyPressed &key_pressed_data = *event.getIf<sf::Event::KeyPressed>();

                                if (sf::Keyboard::Key::Escape == key_pressed_data.code) {
                                        window.close();
                                } else if (sf::Keyboard::Key::W == key_pressed_data.code ||
                                           sf::Keyboard::Key::Up == key_pressed_data.code) {

                                        auto veiw = window.getView();
                                        veiw.move({0.0f, -10.0f});
                                        window.setView(veiw);

                                } else if (sf::Keyboard::Key::S == key_pressed_data.code ||
                                           sf::Keyboard::Key::Down == key_pressed_data.code) {
                                        auto veiw = window.getView();
                                        veiw.move({0.0f, 10.0f});
                                        window.setView(veiw);
                                } else if (sf::Keyboard::Key::A == key_pressed_data.code ||
                                           sf::Keyboard::Key::Left == key_pressed_data.code) {
                                        auto veiw = window.getView();
                                        veiw.move({-10.0f, 0.0f});
                                        window.setView(veiw);  
                                } else if (sf::Keyboard::Key::D == key_pressed_data.code ||
                                           sf::Keyboard::Key::Right == key_pressed_data.code) {
                                        auto veiw = window.getView();
                                        veiw.move({10.0f, 0.0f});
                                        window.setView(veiw);
                                }
                        } else if (event.is<sf::Event::Resized>()) {
                                // auto view = window.getView();
                                // view.setSize({static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)});
                                // view.setCenter({static_cast<float>(window.getSize().x / 2),
                                //                 static_cast<float>(window.getSize().y / 2)});
                                // window.setView(view);
                        } else if (event.is<sf::Event::Closed>()) {
                                window.close();
                        }

                        action_manager.CreateActionFromEvent(event);
                }

                window.clear();
// std::cout << __FILE__ " :" << __LINE__ << "\n";

                {
                        Lumen::UI::Component::RelativeSelectionAction relative_selection_action{
                                action_manager.GetSelectionAction(),
                                action_manager.GetSelectionAction().position,
                        };
                        ui_manager.DoSelectionAction(relative_selection_action);
                }

                auto current_time_point = std::chrono::high_resolution_clock::now();
                auto delta_time = std::chrono::duration<float>{current_time_point - last_update_time_point}.count();
                last_update_time_point = current_time_point;
                // std::cout << "delta_time=" << delta_time << "\n";
                ui_manager.Update(delta_time);

                ui_manager.Render(window);
// std::cout << __FILE__ " :" << __LINE__ << "\n";
                //window.draw(ui_manager.GetDefaultSprite());
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
        //TestMenuCase1();
        TestMenuCase1();
        std::cout << "Done\n";
}
