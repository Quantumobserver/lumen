
#include <iostream>

#include <SFML/Window.hpp>
#include <glm/vec2.hpp>

#include "Render/Window/RenderWindow.hpp"

void Test1(void)
{
        glm::vec2 vec2_1{34.6f, 55.9f};
        glm::vec2 vec2_2{34.6f, 55.9f};
        glm::vec2 vec2_result{vec2_1 + vec2_2};
        std::cout << "vec2_result.x: " << vec2_result.x << " " << "vec2_result.y: " << vec2_result.x << "\n";

        sf::Window window(sf::VideoMode(sf::Vector2u{800, 600}), "My window");

        sf::Image image{};
        // run the program as long as the window is open
        while (window.isOpen()) {
                // check all the window's events that were triggered since the last iteration of the loop
                std::optional<sf::Event> event = window.pollEvent();
                while (event.has_value()) {
                        // "close requested" event: we close the window
                        if (nullptr != event.value().getIf<sf::Event::Closed>()) {
                                window.close();
                        }
                        event = window.pollEvent();
                }
        }
}

int main()
{
        //sf::Window window(sf::VideoMode(sf::Vector2u{800, 600}), "My window");

        Lumen::RenderWindow window{Lumen::VideoMode{{800, 600}}, Lumen::WindowTitle{"My window"}};

        //sf::Image image{};
        // run the program as long as the window is open
        while (window.IsOpen()) {
                // check all the window's events that were triggered since the last iteration of the loop
                for (Lumen::Event event = window.PollEvent(); event.HasEvent(); event = window.PollEvent()) {
                        // "close requested" event: we close the window
                        if (event.IsThisEventType<sf::Event::KeyPressed>()) {
                                sf::Event::KeyPressed key_pressed_data = event.GetEventData<sf::Event::KeyPressed>();
                                std::cout << "key_pressed_data.code: " << static_cast<int>(key_pressed_data.code) << "\n";

                                if (sf::Keyboard::Key::Escape != key_pressed_data.code) {
                                        continue;
                                }
                                window.CloseWindow();
                        }
                }
        }

    return 0;
}
