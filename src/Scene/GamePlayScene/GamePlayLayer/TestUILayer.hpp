
#pragma once

#include <Scene/Scene.hpp>

#include <cassert>
#include <iostream>

namespace Lumen {
namespace Scene {

class GamePlayScene;

namespace GamePlayLayer {

class TestUILayer : public Lumen::LayerStack::BaseLayer {
private:
        friend Lumen::Scene::GamePlayScene;

        Lumen::Scene::BaseScene *m_game_play_scene;

public:
        constexpr TestUILayer(Lumen::Scene::BaseScene *game_play_scene) noexcept
        : m_game_play_scene{game_play_scene}
        {
                assert(nullptr != this->m_game_play_scene);
        }

private:

        constexpr void Update(void) noexcept override
        {
                //std::cout << "[ExampleLayer1]: Update\n";
        }

        [[nodiscard]] constexpr Lumen::LayerStack::BaseLayer::DoActionResult DoAction(Lumen::Action::Action action) noexcept override
        {
                switch (action.action_name) {
                case Lumen::Action::ActionName::QUIT: {
                        std::cout << "[GamePlayScene:TestUILayer]: QUIT\n";
                        assert(nullptr != this->m_game_play_scene);
                        assert(nullptr != this->m_game_play_scene->m_inter_scene_communication_data);
                        this->m_game_play_scene->m_inter_scene_communication_data->change_scene = true;
                        auto &change_scene_args = this->m_game_play_scene->m_inter_scene_communication_data->change_scene_args;
                        change_scene_args.from_scene = Lumen::Scene::SceneID::GAME_PLAY;
                        change_scene_args.change_to_this_scene = Lumen::Scene::SceneID::MENU;
                        this->m_game_play_scene->m_action_manager.ResetActionBuffer();
                        return Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked;
                }
                case Lumen::Action::ActionName::MOVE_UP:
                        std::cout << "[GamePlayScene:TestUILayer]: MOVE_UP\n";
                        return Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked;
                
                default:
                        break;
                }
                return Lumen::LayerStack::BaseLayer::DoActionResult::NotHandedOrNotBlocked;
        }
};

} // namespace GamePlayLayer
} // namespace Scene
} // namespace Lumen