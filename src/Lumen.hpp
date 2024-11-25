
#include <ECS/Entity/EntityManager.hpp>
#include <Scene/SceneManager.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <glm/vec2.hpp>

#include <cassert>
#include <iostream>
#include <string>


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
        sf::RenderWindow m_window;

        // TODO: the final version will contaion more data
        Lumen::Config m_config;

        Lumen::Scene::SceneManager m_scene_manager;
        Lumen::ECS::Entity::EntityManager m_entity_manager;

public:
        constexpr void Init(void) noexcept
        {
                this->LoadInitConfig();
                this->CreateWindow();
                this->InitSceneManager();
        }

        void Updata(void)
        {
                this->m_scene_manager.Update();
                //this->TestUpdate();
        }

        constexpr void Run(void) noexcept
        {
                while (this->IsRunning()) {
                        this->Updata();
                }
        }

private:
        constexpr void LoadInitConfig(void) noexcept
        {
                // TODO: LoadInitConfig from json file
                this->m_config.window.video_mode.width = 800;
                this->m_config.window.video_mode.height = 500;
                this->m_config.window.title = "Lumen";
        }

        constexpr void CreateWindow(void) noexcept
        {
                this->m_window.create(
                        sf::VideoMode{
                                {this->m_config.window.video_mode.width,
                                 this->m_config.window.video_mode.height}},
                                 this->m_config.window.title);
        }

        constexpr void InitSceneManager(void) noexcept
        {
                this->m_scene_manager.Init(&this->m_window, &this->m_entity_manager);
        }

        constexpr bool IsRunning(void) const noexcept
        {
                return this->m_scene_manager.IsRunning();
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
