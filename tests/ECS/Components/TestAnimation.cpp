
#define LUMEN_DEBUG_ACTION_FORMAT_ENABLED

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <ECS/Components/Components.hpp>

#include <iostream>

#include <SFML/Window/Window.hpp>
#include <SFML/Window/Keyboard.hpp>

class Timer {
        std::chrono::time_point<std::chrono::high_resolution_clock> last_update_time_point;
public:
        constexpr void Init(void) noexcept
        {
                last_update_time_point = std::chrono::high_resolution_clock::now();
        }

        constexpr float GetElapsed(void) noexcept
        {
                auto tmp_time_point = last_update_time_point;
                last_update_time_point = std::chrono::high_resolution_clock::now();
                std::chrono::duration<float> elapsed = (last_update_time_point - tmp_time_point);
                return elapsed.count();
        }
};

void TestAnimationCase1(void) noexcept
{
        sf::RenderWindow window{sf::VideoMode{{800, 600}}, {"My window"}};
        Timer timer;

        timer.Init();

        sf::Texture texture;
        bool is_succeed = texture.loadFromFile("test_texture.jpg");
        assert(is_succeed);
        constexpr const unsigned int number_of_frames_this_animation_has = 4;
        sf::Vector2i size{
                static_cast<int>(texture.getSize().x / number_of_frames_this_animation_has),
                static_cast<int>(texture.getSize().y)
        };
        sf::Sprite sprite{
                texture,
                sf::IntRect{
                        { 0, 0 },
                        size
                }
        };

        Lumen::Core::Math::Vec2f32 lumen_size{static_cast<float>(size.x), static_cast<float>(size.y)};

        Lumen::ECS::Component::Animation animation{number_of_frames_this_animation_has, 0.1f, lumen_size, sprite, false};

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
                }
                window.clear();
                animation.Update(timer.GetElapsed());
                auto animation_sprite = animation.GetSprite();
                animation_sprite.setPosition({400.0f, 300.0f});
                window.draw(animation_sprite);
                window.display();
        }
}

int main(void)
{
        TestAnimationCase1();
        std::cout << "Done\n";
}
