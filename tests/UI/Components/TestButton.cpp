
#include <Action/ActionManager.hpp>
#include <UI/Components/Button.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include <iostream>

#include <SFML/Window/Window.hpp>
#include <SFML/Window/Keyboard.hpp>

#if 0
void TestButtonCase1(void) noexcept 
{
        //sf::RenderWindow window{sf::VideoMode{{1920, 1080}}, {"My window"}, sf::Style::Default, sf::State::Fullscreen,};
        sf::RenderWindow window{sf::VideoMode{{1920, 1080}}, {"My window"}};


        Lumen::Action::ActionManager action_manager{};

        action_manager.Init();

        int i{0};

        struct DoButtonActionData {
                int &i;
                sf::RenderWindow &window;
        } do_button_action_data{i, window};

        Lumen::UI::Component::Button button{
                "test", sf::Sprite{}, Lumen::UI::Component::BoundingBox{{50, 70}},
                Lumen::UI::Component::TransformTopLeft{{100, 200}}, &window,
                &do_button_action_data, [](void *data) {
                        assert(nullptr != data);
                        DoButtonActionData &do_button_action_data = *static_cast<DoButtonActionData *>(data);
                        if (do_button_action_data.i > 10) {
                                do_button_action_data.window.close();
                        }
                        std::cout << "[DoButtonActionFn]: " << do_button_action_data.i << "\n";
                        ++do_button_action_data.i;
                }

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
                button.DoSelectionAction({selection_action, selection_action.position});
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
                button.Render();
                window.display();
        }
}
#endif

void TestFont(void) noexcept
{
        sf::Font font;
        if (!font.openFromFile("test.ttf")) {
                std::cerr << "Failed to load font\n";
                return;
        }

        sf::Text text{font, "Hello, world!"};

        sf::RenderWindow window{sf::VideoMode{{1920, 1080}}, {"My window"}};

        while (window.isOpen()) {
                for (std::optional<sf::Event> optional_event = window.pollEvent();
                     optional_event.has_value();
                     optional_event = window.pollEvent()) {
                        sf::Event &event = optional_event.value();

                        if (event.is<sf::Event::KeyPressed>()) {
                                const sf::Event::KeyPressed &key_pressed_data = *event.getIf<sf::Event::KeyPressed>();

                                if (sf::Keyboard::Key::Escape == key_pressed_data.code) {
                                        window.close();
                                }
                        } else if (event.is<sf::Event::Closed>()) {
                                window.close();
                        }
                }

                window.clear();
                window.draw(text);
                window.display();
        }

}

int main(void)
{
        //TestActionManagerCase1();
        //TestButtonCase1();
        TestFont();
        std::cout << "Done\n";
}
