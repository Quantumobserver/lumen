
#include <iostream>

#include <SFML/Window.hpp>
#include <glm/vec2.hpp>

int main()
{
        glm::vec2 vec2_1{34.6f, 55.9f};
        glm::vec2 vec2_2{34.6f, 55.9f};
        glm::vec2 vec2_result{vec2_1 + vec2_2};
        std::cout << "vec2_result.x: " << vec2_result.x << " " << "vec2_result.y: " << vec2_result.x << "\n";

    sf::Window window(sf::VideoMode(sf::Vector2u{800, 600}), "My window");

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        std::optional<sf::Event> event = window.pollEvent();
        while (event.has_value())
        {
            // "close requested" event: we close the window
            if (nullptr != event.value().getIf<sf::Event::Closed>()) {

                window.close();
            }
                event = window.pollEvent();
        }
    }

    return 0;
}
