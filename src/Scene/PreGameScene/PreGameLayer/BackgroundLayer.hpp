#pragma once

#include <Scene/Scene.hpp>
#include <ResourceManager/ResourceManager.hpp>
#include <UI/UIManager.hpp>

namespace Lumen {
namespace Scene {
namespace PreGameLayer {

class BackgroundLayer : public Lumen::LayerStack::BaseLayer
{
private:
        Lumen::Scene::BaseScene *m_pre_game_scene;
        Lumen::UI::UIManager m_ui_manager{};
        // const sf::Font *m_menu_font{};
        // sf::Sprite start_game_menu_button_sprite;

public:
    constexpr BackgroundLayer(Lumen::Scene::BaseScene *pre_game_scene) noexcept
    : m_pre_game_scene{pre_game_scene}
    {
        assert(nullptr != this->m_pre_game_scene);

        this->Init();

        
        // auto &resource_manager = *this->m_pre_game_scene->m_resource_manager_ptr;

        std::cout << "[BackgroundLayer]: Constructed\n";
    }

    constexpr void Init() noexcept
    {
        this->m_ui_manager.Init(*this->m_pre_game_scene->m_resource_manager_ptr, *this->m_pre_game_scene->m_window_ptr);
    }

    constexpr void Update(float delta_time) noexcept override
    {
        (void)delta_time;
        //std::cout << "[ExampleLayer2]: Update\n";
    }

    constexpr void Render() noexcept override
    {
        //std::cout << "[ExampleLayer2]: Render\n";
    }
};

} // namespace PreGameLayer
} // namespace Scene     
} // namespace Lumen
