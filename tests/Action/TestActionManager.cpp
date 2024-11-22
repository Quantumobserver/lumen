
#define LUMEN_DEBUG_ACTION_FORMAT_ENABLED

#include <Action/ActionManager.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <iostream>

#include <SFML/Window/Window.hpp>
#include <SFML/Window/Keyboard.hpp>

void TestActionManagerCase1(void) noexcept
{
        sf::RenderWindow window{sf::VideoMode{{800, 600}}, {"My window"}};
        Lumen::Action::ActionManager action_manager{};

        action_manager.Init();

        action_manager.RegisteAction(sf::Keyboard::Key::W, Lumen::Action::ActionName::MOVE_UP);
        action_manager.RegisteAction(sf::Keyboard::Key::A, Lumen::Action::ActionName::MOVE_LEFT);
        action_manager.RegisteAction(sf::Keyboard::Key::S, Lumen::Action::ActionName::MOVE_DOWN);
        action_manager.RegisteAction(sf::Keyboard::Key::D, Lumen::Action::ActionName::MOVE_RIGHT);

        action_manager.RegisteAction(sf::Keyboard::Key::G, Lumen::Action::ActionName::TOGGLE_DRAWING_GRID);
        action_manager.SetActionKind(Lumen::Action::ActionName::TOGGLE_DRAWING_GRID, Lumen::Action::ActionKind::TRIGGER);

        while (window.isOpen()) {
                for (std::optional<sf::Event> optional_event = window.pollEvent(); optional_event.has_value(); optional_event = window.pollEvent()) {
                        sf::Event &event = optional_event.value();
                        action_manager.CreateActionFromEvent(event);
                        std::cout << "[Update Action]: ";
                        for (auto action : action_manager) {
                                std::cout << "{ " << Lumen::Action::fmt(action.action_name) << ", " << Lumen::Action::fmt(action.action_status) << " }, ";
                        }
                        std::cout << "\n";

                        if (event.is<sf::Event::KeyPressed>()) {
                                const sf::Event::KeyPressed &key_pressed_data = *event.getIf<sf::Event::KeyPressed>();
                                if (sf::Keyboard::Key::A == key_pressed_data.code) {
                                        std::cout << "[SFML KeyPressed]: A\n";

                                }
                                if (sf::Keyboard::Key::Escape == key_pressed_data.code) {
                                        window.close();
                                }
                        } else if (event.is<sf::Event::KeyReleased>()) {
                                const sf::Event::KeyReleased &key_pressed_data = *event.getIf<sf::Event::KeyReleased>();
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
