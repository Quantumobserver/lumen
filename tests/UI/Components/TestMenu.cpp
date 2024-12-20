
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
                &do_button_action_data, [](void *data, const Lumen::Action::SelectionAction &selection_action) {
                        assert(nullptr != data);

                        DoButtonActionData &do_button_action_data = *static_cast<DoButtonActionData *>(data);

                        switch (selection_action.selection_action_type) {
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
                }
        };

        auto menu_button_2 = Lumen::UI::Component::Detail::MenuButton{
                "test", sf::Sprite{},
                &do_button_action_data, [](void *data, const Lumen::Action::SelectionAction &selection_action) {
                        assert(nullptr != data);

                        DoButtonActionData &do_button_action_data = *static_cast<DoButtonActionData *>(data);

                        switch (selection_action.selection_action_type) {
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
                }

        };

        //auto button_3 = std::move(button_1);

        auto buttons = std::vector<Lumen::UI::Component::Detail::MenuButton>{
                //std::move(button_1),
                //std::move(button_2),
        };
        buttons.push_back(std::move(menu_button_1));
        buttons.push_back(std::move(menu_button_2));

        constexpr const int scale = 2;
        Lumen::UI::Component::Menu menu{
                Lumen::UI::Component::Transform{{400, 300}},
                Lumen::UI::Component::BoundingBox{{50 * scale, 70 * scale}},
                Lumen::UI::Component::BoundingBox{{50 * scale, 10 * scale}},
                &window,
                std::move(buttons),
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
                const auto &selection_action = action_manager.GetSelectionAction();
                menu.DoSelectionAction(selection_action);
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

                action_manager.ResetActionBuffer();

                window.clear();
                menu.Render();
                window.display();
        }
}

int main(void)
{
        //TestMenuCase1();
        TestMenuCase1();
        std::cout << "Done\n";
}
