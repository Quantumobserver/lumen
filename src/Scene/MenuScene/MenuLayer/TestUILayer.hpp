
#pragma once

#include <Scene/Scene.hpp>

#include <cassert>
#include <iostream>

namespace Lumen {
namespace Scene {

class MenuScene;

namespace MenuLayer {

class TestUILayer : public Lumen::LayerStack::BaseLayer {
private:
        friend Lumen::Scene::MenuScene;

        Lumen::Scene::BaseScene *m_menu_scene;
public:
        constexpr TestUILayer(Lumen::Scene::BaseScene *menu_scene) noexcept
        : m_menu_scene{menu_scene}
        {
                assert(nullptr != this->m_menu_scene);
        }

private:
        constexpr void Update(void) noexcept override
        {
                //std::cout << "[ExampleLayer1]: Update\n";
        }

        [[nodiscard]] constexpr Lumen::LayerStack::BaseLayer::DoActionResult DoAction(Lumen::Action::Action action) noexcept override
        {
                switch (action.action_name) {
                case Lumen::Action::ActionName::QUIT:
                        std::cout << "[MenuScene:TestUILayer]: QUIT\n";
                        assert(nullptr != this->m_menu_scene);
                        assert(nullptr != this->m_menu_scene->m_inter_scene_communication_data);
                        this->m_menu_scene->m_inter_scene_communication_data->running = false;
                        return Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked;
                case Lumen::Action::ActionName::MOVE_UP:
                        std::cout << "[MenuScene:TestUILayer]: MOVE_UP\n";
                        return Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked;
                
                case Lumen::Action::ActionName::START_SELECTED_LEVEL: {
                        std::cout << "[MenuScene:TestUILayer]: START_SELECTED_LEVEL\n";
                        assert(nullptr != this->m_menu_scene);
                        assert(nullptr != this->m_menu_scene->m_inter_scene_communication_data);
                        this->m_menu_scene->m_inter_scene_communication_data->change_scene = true;
                        auto &change_scene_args = this->m_menu_scene->m_inter_scene_communication_data->change_scene_args;
                        change_scene_args.from_scene = Lumen::Scene::SceneID::MENU;
                        change_scene_args.change_to_this_scene = Lumen::Scene::SceneID::GAME_PLAY;
                        this->m_menu_scene->m_action_manager.ResetActionBuffer();
                        return Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked;
                }
                
                default:
                        break;
                }
                return Lumen::LayerStack::BaseLayer::DoActionResult::NotHandedOrNotBlocked;
        }
};


} // namespace MenuLayer
} // namespace Scene
} // namespace Lumen