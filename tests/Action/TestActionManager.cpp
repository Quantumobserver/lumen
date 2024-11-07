
#define LUMEN_DEBUG_ACTION_FORMAT_ENABLED

#include <Action/ActionManager.hpp>
#include "Render/Window/RenderWindow.hpp"

#include <iostream>

#include <SFML/Window/Window.hpp>
#include <SFML/Window/Keyboard.hpp>

void TestActionManagerCase1(void) noexcept
{
        Lumen::RenderWindow window{Lumen::VideoMode{{800, 600}}, Lumen::WindowTitle{"My window"}};
        Lumen::Action::ActionManager action_manager{};

        action_manager.Init();

        action_manager.RegisteAction(sf::Keyboard::Key::W, Lumen::Action::ActionName::MOVE_UP);
        action_manager.RegisteAction(sf::Keyboard::Key::A, Lumen::Action::ActionName::MOVE_LEFT);
        action_manager.RegisteAction(sf::Keyboard::Key::S, Lumen::Action::ActionName::MOVE_DOWN);
        action_manager.RegisteAction(sf::Keyboard::Key::D, Lumen::Action::ActionName::MOVE_RIGHT);

        action_manager.RegisteAction(sf::Keyboard::Key::G, Lumen::Action::ActionName::TOGGLE_DRAWING_GRID);
        action_manager.SetActionKind(Lumen::Action::ActionName::TOGGLE_DRAWING_GRID, Lumen::Action::ActionKind::TRIGGER);

        //sf::Image image{};
        // run the program as long as the window is open
        while (window.IsOpen()) {
                // check all the window's events that were triggered since the last iteration of the loop
                for (Lumen::Event event = window.PollEvent(); event.HasEvent(); event = window.PollEvent()) {
                        // "close requested" event: we close the window
                        action_manager.CreateActionFromEvent(event);
                        std::cout << "[Update Action]: ";
                        for (auto action : action_manager) {
                                std::cout << "{ " << Lumen::Action::fmt(action.action_name) << ", " << Lumen::Action::fmt(action.action_status) << " }, ";
                        }
                        std::cout << "\n";

                        if (event.IsThisEventType<sf::Event::KeyPressed>()) {
                                const sf::Event::KeyPressed key_pressed_data = event.GetEventData<sf::Event::KeyPressed>();
                                if (sf::Keyboard::Key::A == key_pressed_data.code) {
                                        std::cout << "[SFML KeyPressed]: A\n";

                                }
                                if (sf::Keyboard::Key::Escape == key_pressed_data.code) {
                                        window.CloseWindow();
                                }
                        } else if (event.IsThisEventType<sf::Event::KeyReleased>()) {
                                const sf::Event::KeyReleased key_pressed_data = event.GetEventData<sf::Event::KeyReleased>();
                                if (sf::Keyboard::Key::A == key_pressed_data.code) {
                                        std::cout << "[SFML KeyReleased]: A\n";
                                }
                        }

                        std::cout << "\n";
                }
        }
}

int main(void)
{
        TestActionManagerCase1();
        std::cout << "Done\n";
}
