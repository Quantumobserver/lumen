
#include <ECS/Entity/EntityManager.hpp>
#include <Scene/SceneManager.hpp>
#include <ResourceManager/ResourceManager.hpp>
#include "VisualProfiling/VisualProfiler.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <glm/vec2.hpp>

#include <cassert>
#include <iostream>
#include <string>
#include <chrono>

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

class Engine {
private:
        sf::RenderWindow m_window;

        // TODO: the final version will contaion more data
        Lumen::Config m_config;

        Lumen::Scene::SceneManager m_scene_manager;
        Lumen::ECS::Entity::EntityManager m_entity_manager;
        Lumen::ResourceManager::ResourceManager m_resource_manager_ptr;

        Lumen::Timer m_timer;

        bool m_is_initialized{false};

public:
        constexpr void Init(void) noexcept
        {
                LUMEN_VISUAL_PROFILE_FUNCTION();
                if (this->m_is_initialized) {
                        return;
                }

                this->LoadInitConfig();
                this->CreateWindow();
                this->InitResourceManager();
                this->InitSceneManager();
                this->m_timer.Init();
                this->m_is_initialized = true;
        }

        void Updata(void)
        {
                LUMEN_VISUAL_PROFILE_FUNCTION();
                assert(this->m_is_initialized);
                const float delta_time = this->m_timer.GetElapsed();
                this->m_scene_manager.SetDeltaTime(delta_time);
                this->m_scene_manager.Update(delta_time);
                //this->TestUpdate();
        }

        void Render(void)
        {
                LUMEN_VISUAL_PROFILE_FUNCTION();
                assert(this->m_is_initialized);
                this->m_window.clear(sf::Color::Yellow);
                this->m_scene_manager.Render();
        }

        constexpr void Run(void) noexcept
        {
                LUMEN_VISUAL_PROFILE_FUNCTION();
                assert(this->m_is_initialized);
                while (this->IsRunning()) {
                        this->Updata();
                        this->Render();
                }
        }

private:
        constexpr void LoadInitConfig(void) noexcept
        {
                assert(!this->m_is_initialized);
                // TODO: LoadInitConfig from json file
                this->m_config.window.video_mode.width = 1200;
                this->m_config.window.video_mode.height = 800;
                this->m_config.window.title = "Lumen";
        }

        constexpr void CreateWindow(void) noexcept
        {
                LUMEN_VISUAL_PROFILE_FUNCTION();
                assert(!this->m_is_initialized);
                this->m_window.create(
                        sf::VideoMode{
                                {this->m_config.window.video_mode.width,
                                 this->m_config.window.video_mode.height}},
                                 this->m_config.window.title);
                this->m_window.setFramerateLimit(120);
        }

        constexpr void InitSceneManager(void) noexcept
        {
                LUMEN_VISUAL_PROFILE_FUNCTION();
                assert(!this->m_is_initialized);
                this->m_scene_manager.Init(&this->m_window, &this->m_resource_manager_ptr, &this->m_entity_manager);
        }

        constexpr void InitResourceManager(void) noexcept
        {
                LUMEN_VISUAL_PROFILE_FUNCTION();
                assert(!this->m_is_initialized);
                this->m_resource_manager_ptr.Init();

                this->LoadInitFonts();
                this->LoadInitTextures();
                this->InitSprite();
        }

        constexpr void LoadInitFonts(void) noexcept
        {
                assert(!this->m_is_initialized);
                this->m_resource_manager_ptr.LoadFontFromFile("./Assets/Fonts/DroidSansFallback.ttf", Lumen::ResourceManager::FontID::DROID_FONT);
        }

        constexpr void LoadInitTextures(void) noexcept
        {
                assert(!this->m_is_initialized);
                this->m_resource_manager_ptr.LoadTextureFromFile("./Assets/Images/testButton.png", Lumen::ResourceManager::TextureID::UI_TEXTURE);
                this->m_resource_manager_ptr.LoadTextureFromFile("./Assets/Images/TestBackground.png", Lumen::ResourceManager::TextureID::MENU_SENCE_BACKGROUND);
        }

        constexpr void InitSprite(void) noexcept
        {
                assert(!this->m_is_initialized);
                sf::Sprite menu_sence_background{this->m_resource_manager_ptr.GetTexture(Lumen::ResourceManager::TextureID::MENU_SENCE_BACKGROUND)};
                sf::Sprite menu_button_sprite{this->m_resource_manager_ptr.GetTexture(Lumen::ResourceManager::TextureID::UI_TEXTURE)};
                menu_button_sprite.setTextureRect(sf::IntRect({0, 17}, {64, 30}));
                this->m_resource_manager_ptr.AddSprite(std::move(menu_sence_background), Lumen::ResourceManager::SpriteID::MENU_SCENCE_BACKGROUND_SPRITE);
                this->m_resource_manager_ptr.AddSprite(std::move(menu_button_sprite), Lumen::ResourceManager::SpriteID::MENU_BUTTON_SPRITE);
        }

        constexpr bool IsRunning(void) const noexcept
        {
                assert(this->m_is_initialized);
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
