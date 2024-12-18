#include <ResourceManager/ResourceManager.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

#include <iostream>

void TestResourceManagerCase1(void) noexcept
{
        sf::RenderWindow window{sf::VideoMode{{1920, 1080}}, {"My window"}};
        Lumen::ResourceManager::ResourceManager resource_manager;

        resource_manager.Init();

        resource_manager.LoadTextureFromFile("test_texture.jpg", Lumen::ResourceManager::TextureID::TEST_TEXTURE);
        {
                const auto &test_texture = resource_manager.GetTexture(Lumen::ResourceManager::TextureID::TEST_TEXTURE);
                resource_manager.AddSprite(sf::Sprite{test_texture, sf::IntRect{{300, 300}, {100, 200}}}, Lumen::ResourceManager::SpriteID::TEST_SPRITE);
        }

        while (window.isOpen()) {
                for (std::optional<sf::Event> optional_event = window.pollEvent(); optional_event.has_value();
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
                const auto &test_texture = resource_manager.GetTexture(Lumen::ResourceManager::TextureID::TEST_TEXTURE);
                sf::Sprite sprite{test_texture};
                window.draw(sprite);

                const auto &test_sprite = resource_manager.GetSprite(Lumen::ResourceManager::SpriteID::TEST_SPRITE);
                window.draw(test_sprite);

                window.display();
        }
}

int main(void)
{
        TestResourceManagerCase1();
        std::cout << "Done\n";
}

