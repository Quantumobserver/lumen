
#include <Action/ActionManager.hpp>
#include <UI/Components/Button.hpp>
#include <UI/Components/Menu.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

#include <iostream>

#include <SFML/Window/Window.hpp>
#include <SFML/Window/Keyboard.hpp>

void TestMenuCase1(void) noexcept 
{
        sf::RenderWindow window{sf::VideoMode{{800, 600}}, {"My window"}};

        Lumen::Action::ActionManager action_manager{};
        action_manager.Init();

        int i{0};

        struct DoButtonActionData {
                int &i;
                sf::RenderWindow &window;
        } do_button_action_data{i, window};

        auto menu_button_1 = Lumen::UI::Component::Detail::MenuButton{
                "test", sf::Sprite{},
                &do_button_action_data,
                [](void *data, const Lumen::UI::Component::RelativeSelectionAction &relative_selection_action) noexcept
                -> std::optional<Lumen::UI::Component::Menu::SelectedSubMenuItem> {
                        assert(nullptr != data);

                        DoButtonActionData &do_button_action_data = *static_cast<DoButtonActionData *>(data);

                        switch (relative_selection_action.selection_action.selection_action_type) {
                        case Lumen::Action::SelectionAction::SelectionActionTypeTag::NONE:
                                //std::cout << "Button None\n";
                                break;
                        case Lumen::Action::SelectionAction::SelectionActionTypeTag::PRESS:
                                std::cout << "[MenuButton_1]: Button Pressed " << do_button_action_data.i << "\n";
                                ++do_button_action_data.i;
                                break;
                        case Lumen::Action::SelectionAction::SelectionActionTypeTag::RELEASE:
                                std::cout << "[MenuButton_1]:Button Released\n";
                                if (do_button_action_data.i > 5) {
                                        do_button_action_data.window.close();
                                }
                                break;
                        case Lumen::Action::SelectionAction::SelectionActionTypeTag::CURSOR_MOVEMENT:
                                break;
                        case Lumen::Action::SelectionAction::SelectionActionTypeTag::WHEEL_SCROLL:
                                std::cout << "Button Wheel Scroll\n";
                                break;
                        }

                        return std::nullopt;
                }
        };

        auto menu_button_2 = Lumen::UI::Component::Detail::MenuButton{
                "test", sf::Sprite{},
                &do_button_action_data,
                [](void *data, const Lumen::UI::Component::RelativeSelectionAction &relative_selection_action) noexcept
                -> std::optional<Lumen::UI::Component::Menu::SelectedSubMenuItem> {
                        assert(nullptr != data);

                        DoButtonActionData &do_button_action_data = *static_cast<DoButtonActionData *>(data);

                        switch (relative_selection_action.selection_action.selection_action_type) {
                        case Lumen::Action::SelectionAction::SelectionActionTypeTag::NONE:
                                //std::cout << "Button None\n";
                                break;
                        case Lumen::Action::SelectionAction::SelectionActionTypeTag::PRESS:
                                std::cout << "[MenuButton_2]: Button Pressed " << do_button_action_data.i << "\n";
                                --do_button_action_data.i;
                                break;
                        case Lumen::Action::SelectionAction::SelectionActionTypeTag::RELEASE:
                                std::cout << "[MenuButton_2]:Button Released\n";
                                if (do_button_action_data.i < -5) {
                                        do_button_action_data.window.close();
                                }
                                break;
                        case Lumen::Action::SelectionAction::SelectionActionTypeTag::CURSOR_MOVEMENT:
                                break;
                        case Lumen::Action::SelectionAction::SelectionActionTypeTag::WHEEL_SCROLL:
                                std::cout << "Button Wheel Scroll\n";
                                break;
                        }
                        return std::nullopt;
                }

        };

        constexpr const int scale = 2;

        auto sub_menu_ptr = std::make_unique<Lumen::UI::Component::Menu>(
                Lumen::UI::Component::TransformCenter{{400, 300}},
                Lumen::UI::Component::BoundingBox{{50 * scale, 70 * scale}},
                Lumen::UI::Component::BoundingBox{{50 * scale, 10 * scale}},
                &window
        );

        auto menu_button_3 = Lumen::UI::Component::Detail::MenuButton{
                "test", sf::Sprite{},
                sub_menu_ptr.get(),
                [](void *data, const Lumen::UI::Component::RelativeSelectionAction &relative_selection_action) noexcept
                -> std::optional<Lumen::UI::Component::Menu::SelectedSubMenuItem> {
                        assert(nullptr != data);

                        Lumen::UI::Component::Menu &sub_menu = *static_cast<Lumen::UI::Component::Menu *>(data);

                        switch (relative_selection_action.selection_action.selection_action_type) {
                        case Lumen::Action::SelectionAction::SelectionActionTypeTag::NONE:
                                //std::cout << "Button None\n";
                                break;
                        case Lumen::Action::SelectionAction::SelectionActionTypeTag::PRESS:
                                std::cout << "[MenuButton_3]: Button Pressed\n";
                                return std::optional<Lumen::UI::Component::Menu::SelectedSubMenuItem>{{
                                        Lumen::UI::Component::SelectedSubMenuItem::SelectionTypeTag::PRESS,
                                        Lumen::UI::Component::SelectedSubMenuItem::SpawnSide::RIGHT_SIDE,
                                        Lumen::UI::Component::SelectedSubMenuItem::SpawnAlignment::TOP,
                                        Lumen::Core::Memory::ReadWritePtr<Lumen::UI::Component::Menu>{&sub_menu}
                                }};
                        case Lumen::Action::SelectionAction::SelectionActionTypeTag::RELEASE:
                                std::cout << "[MenuButton_3]:Button Released\n";
                                return std::optional<Lumen::UI::Component::Menu::SelectedSubMenuItem>{};
                        case Lumen::Action::SelectionAction::SelectionActionTypeTag::CURSOR_MOVEMENT:
                                return std::optional<Lumen::UI::Component::Menu::SelectedSubMenuItem>{{
                                        Lumen::UI::Component::SelectedSubMenuItem::SelectionTypeTag::HOVER,
                                        Lumen::UI::Component::SelectedSubMenuItem::SpawnSide::LEFT_SIDE,
                                        Lumen::UI::Component::SelectedSubMenuItem::SpawnAlignment::BOTTOM,
                                        Lumen::Core::Memory::ReadWritePtr<Lumen::UI::Component::Menu>{&sub_menu}

                                }};
                                /*return std::optional<Lumen::UI::Component::Menu::SelectedSubMenuItem>{
                                        Lumen::UI::Component::SelectedSubMenuItem::SelectionTypeTag::HOVER,
                                        Lumen::Core::Memory::ReadWritePtr<Lumen::UI::Component::Menu>{&sub_menu}
                                };*/
                        case Lumen::Action::SelectionAction::SelectionActionTypeTag::WHEEL_SCROLL:
                                std::cout << "Button Wheel Scroll\n";
                                break;
                        }
                        return std::nullopt;
                }

        };

        auto buttons = std::vector<Lumen::UI::Component::Detail::MenuButton>{
                //std::move(button_1),
                //std::move(button_2),
        };
        buttons.push_back(std::move(menu_button_1));
        buttons.push_back(std::move(menu_button_2));
        buttons.push_back(std::move(menu_button_3));

        std::vector<std::unique_ptr<Lumen::UI::Component::Menu>> sub_menus{};
        sub_menus.push_back(std::move(sub_menu_ptr));

        /*Lumen::UI::Component::Menu menu{
                Lumen::UI::Component::TransformCenter{{400, 300}},
                Lumen::UI::Component::BoundingBox{{50 * scale, 70 * scale}},
                Lumen::UI::Component::BoundingBox{{50 * scale, 10 * scale}},
                &window,
                std::move(buttons),
        };*/
        Lumen::UI::Component::Menu menu{
                Lumen::UI::Component::TransformCenter{{400, 300}},
                Lumen::UI::Component::BoundingBox{{50 * scale, 10 * scale}},
                &window,
                std::move(buttons),
                Lumen::UI::Component::Menu::MenuLayout::VerticalFixedSpacingAutoData{
                        .fixed_spacing_header_y = 30,
                        .fixed_spacing_y = 50,
                        .fixed_spacing_footer_y = 100
                },
                std::move(sub_menus)
        };

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
                
                /*switch (selection_action.selection_action_type) {
                case Lumen::Action::SelectionAction::SelectionActionTypeTag::NONE:
                        //std::cout << "[SelectionAction] None: \n";
                        break;

                case Lumen::Action::SelectionAction::SelectionActionTypeTag::PRESS:
                        std::cout << "[SelectionAction] Press: \n";
                        {
                                const auto &position = selection_action.position;
                                std::cout << "[SelectionAction Press]: { " << position.x << ", " << position.y << " }\n";
                                std::cout << Lumen::Action::fmt(selection_action.click_type) << "\n";
                        }
                        break;
                
                case Lumen::Action::SelectionAction::SelectionActionTypeTag::RELEASE:
                        std::cout << "[SelectionAction] Release: \n";
                        {
                                const auto &position = selection_action.position;
                                std::cout << "[SelectionAction Press]: { " << position.x << ", " << position.y << " }\n";
                                std::cout << Lumen::Action::fmt(selection_action.click_type) << "\n";
                        }
                        break;
                
                case Lumen::Action::SelectionAction::SelectionActionTypeTag::WHEEL_SCROLL:
                        std::cout << "[SelectionAction] WheelScroll: "
                                  << Lumen::Action::fmt(selection_action.wheel_scroll.scroll_direction)
                                  << " " << selection_action.wheel_scroll.wheel_offset << "\n";
                        break;
                        
                case Lumen::Action::SelectionAction::SelectionActionTypeTag::CURSOR_MOVEMENT: {
                        [[maybe_unused]] const auto &position = selection_action.position;
                        //std::cout << "[SelectionAction] Move: { " << position.x << ", " << position.y << " } \n";
                        break;
                }

                default:
                        break;
                }*/


                window.clear();
                menu.Render();
                window.display();

                if (!action_manager.IsSelectionActionHappened()) {
                        continue;
                }
                const auto &selection_action = action_manager.GetSelectionAction();
                menu.DoSelectionAction({selection_action, selection_action.position});
                menu.Update({0, 0});

                if (2 == i) {
                        menu.SetPosition({std::abs(i) * 100, std::abs(i) * 100});
                } else if (-2 == i) {
                        menu.SetPosition({400, 300});
                }
                action_manager.ResetActionBuffer();
        }
}

int main(void)
{
        //TestMenuCase1();
        TestMenuCase1();
        std::cout << "Done\n";
}
