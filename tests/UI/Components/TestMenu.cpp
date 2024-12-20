
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

        auto button_1 = std::make_unique<Lumen::UI::Component::Button>(
                "test", sf::Sprite{}, Lumen::UI::Component::BoundingBox{{50, 70}},
                Lumen::UI::Component::Transform{{100, 200}}, &window,
                &do_button_action_data, [](void *data) {
                        assert(nullptr != data);
                        DoButtonActionData &do_button_action_data = *static_cast<DoButtonActionData *>(data);
                        if (do_button_action_data.i > 10) {
                                do_button_action_data.window.close();
                        }
                        std::cout << "[DoButtonActionFn]: " << do_button_action_data.i << "\n";
                        ++do_button_action_data.i;
                }
        );

        auto button_2 = std::make_unique<Lumen::UI::Component::Button>(
                "test", sf::Sprite{}, Lumen::UI::Component::BoundingBox{{50, 70}},
                Lumen::UI::Component::Transform{{400, 100}}, &window,
                &do_button_action_data, [](void *data) {
                        assert(nullptr != data);
                        DoButtonActionData &do_button_action_data = *static_cast<DoButtonActionData *>(data);
                        if (do_button_action_data.i < -5) {
                                do_button_action_data.window.close();
                        }
                        std::cout << "[DoButtonActionFn 2]: " << do_button_action_data.i << "\n";
                        --do_button_action_data.i;
                }

        );

        //auto button_3 = std::move(button_1);

        auto buttons = std::vector<std::unique_ptr<Lumen::UI::Component::Button> >{
                //std::move(button_1),
                //std::move(button_2),
        };
        buttons.push_back(std::move(button_1));
        buttons.push_back(std::move(button_2));

        Lumen::UI::Component::Menu menu{
                Lumen::UI::Component::Transform{{0, 0}},
                Lumen::UI::Component::BoundingBox{{50, 70}},
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
