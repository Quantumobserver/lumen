#pragma once

#include <UI/Components/Panel.hpp>

namespace Lumen {
namespace Scene {
namespace PreGameLayer {

class PreGameUILayer : public Lumen::LayerStack::BaseLayer
{

private:
        Lumen::Scene::BaseScene *m_pre_game_scene;
        Lumen::UI::UIManager m_ui_manager{};
        // const sf::Font *m_menu_font{};
        // sf::Sprite start_game_menu_button_sprite;

public:
        constexpr PreGameUILayer(Lumen::Scene::BaseScene *pre_game_scene) :
                m_pre_game_scene{pre_game_scene}
        {
                assert(nullptr != this->m_pre_game_scene);

                this->Init();

                // auto &resource_manager = *this->m_menu_scene->m_resource_manager_ptr;

                // this->InitMainMenu(resource_manager);
        }

        constexpr void Update(float delta_time) noexcept override
        {
                (void)delta_time;
                // std::cout << "[ExampleLayer2]: Update\n";
        }

        constexpr void Init(void) noexcept
        {
                this->m_ui_manager.Init(*this->m_pre_game_scene->m_resource_manager_ptr, *this->m_pre_game_scene->m_window_ptr);
        }

        constexpr void Render(void) noexcept override
        {
                // this->m_ui_manager.Render(*this->m_menu_scene->m_window_ptr);
        }

        [[nodiscard]] constexpr Lumen::LayerStack::BaseLayer::DoActionResult DoAction(Lumen::Action::Action action) noexcept override {
                switch (action.action_name) {
                        case Lumen::Action::ActionName::QUIT:
                                std::cout << "[PreGameScene:PreGameUILayer]: QUIT\n";
                                this->m_pre_game_scene->m_inter_scene_communication_data->running = false;
                                return Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked;
                        default:
                                break;
                }
                return Lumen::LayerStack::BaseLayer::DoActionResult::NotHandledOrNotBlocked;
        }

};

} // namespace PreGameLayer
} // namespace Scene     
} // namespace Lumen
