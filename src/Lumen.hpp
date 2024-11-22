
#include <cassert>
#include <iostream>
#include <string>

#include <SFML/Graphics/RenderWindow.hpp>
#include <glm/vec2.hpp>

namespace Lumen {

// Just for now
struct Config {
        struct Window {
                struct VideoMode {
                        unsigned int width;
                        unsigned int height;
                };
                VideoMode video_mode;
                std::string title;
        };

        Window window;
};

class Engine {
private:
        sf::RenderWindow window;

        // TODO: the final version will contaion more data
        Lumen::Config config;

        //SceneManager scene_manager;

        bool m_not_paused;
        bool m_running;

public:
        constexpr void Init(void) noexcept
        {
                this->LoadInitConfig();
                this->CreateWindow();
                this->m_running = true;
                this->m_not_paused = true;
        }

        constexpr void Destroy(void) noexcept
        {
                this->m_running = false;
        }

        void Pause(void) noexcept
        {
                this->m_not_paused = !this->m_not_paused;
        }

        void Updata(void)
        {
                //this->scene_manager.Update();
                this->TestUpdate();
        }

        constexpr void Run(void) noexcept
        {
                while (this->m_running) {
                        this->Updata();
                }
        }

private:
        constexpr void LoadInitConfig(void) noexcept
        {
                // TODO: LoadInitConfig from json file
                this->config.window.video_mode.width = 800;
                this->config.window.video_mode.height = 500;
                this->config.window.title = "Lumen";
        }

        constexpr void CreateWindow(void) noexcept
        {
                this->window.create(
                        sf::VideoMode{
                                {this->config.window.video_mode.width,
                                 this->config.window.video_mode.height}},
                                this->config.window.title);
        }

        // TODO: Remove me
        constexpr void TestUpdate(void) noexcept
        {
                assert(this->window.isOpen());

                for (std::optional<sf::Event> optional_event = this->window.pollEvent(); optional_event.has_value(); optional_event = this->window.pollEvent()) {
                        sf::Event &event = optional_event.value();
                        if (event.is<sf::Event::KeyPressed>()) {
                                const sf::Event::KeyPressed &key_pressed_data = *event.getIf<sf::Event::KeyPressed>();
                                std::cout << "key_pressed_data.code: " << static_cast<int>(key_pressed_data.code) << "\n";

                                if (sf::Keyboard::Key::Escape != key_pressed_data.code) {
                                        continue;
                                }
                                this->Destroy();
                        }
                }
        }
};

class Game {
private:
        Lumen::Engine m_lumen_engine;

public:
        constexpr void Init(void) noexcept
        {
                this->m_lumen_engine.Init();
        }

        constexpr void Run(void) noexcept
        {
                this->m_lumen_engine.Run();
        }
};

} // namespace Lumen
