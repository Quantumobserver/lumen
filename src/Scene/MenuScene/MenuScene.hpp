
#pragma once

#include <Scene/Scene.hpp>
#include "MenuLayer/TestUILayer.hpp"
#include "MenuLayer/TestBackgroundLayer.hpp"

#include <iostream>

namespace Lumen {
namespace Scene {

class MenuScene : public Lumen::Scene::BaseScene {
private:
        bool m_is_initialized{false};
public:
        constexpr MenuScene(sf::RenderWindow *window_ptr,
                            Lumen::ECS::Entity::EntityManager *entity_manager_ptr,
                            Lumen::Scene::InterSceneCommunicationData *inter_scene_communication_data) noexcept
         : Lumen::Scene::BaseScene{Lumen::Scene::SceneID::MENU,
                                   window_ptr,
                                   entity_manager_ptr,
                                   inter_scene_communication_data},
           m_is_initialized{false} {}

        constexpr void Init(void) noexcept override
        {
                if (this->m_is_initialized) {
                        return;
                }

                this->InitActionManager();
                this->InitLayerStack();

                this->m_is_initialized = true;
        }


        constexpr void Update(void) noexcept override
        {
                assert(this->m_is_initialized);

                this->CreateActions();
                
                this->DoActions();
        }

        constexpr void Render(void) noexcept override
        {
                assert(this->m_is_initialized);
                if (Lumen::Scene::BaseScene::m_inter_scene_communication_data->change_scene) {std::cout << "[MenuScene] Render change_scene: true\n";}
        }

        constexpr void ChangeToThisScene(Lumen::Scene::ChangeSceneArgs &change_scene_args) noexcept override
        {
                assert(this->m_is_initialized);
                (void)change_scene_args;
        }

private:
        constexpr void InitActionManager(void) noexcept
        {
                assert(!this->m_is_initialized);

                Lumen::Scene::BaseScene::m_action_manager.Init();

                // TODO: read action map data from file
                Lumen::Scene::BaseScene::RegisteAction(sf::Keyboard::Key::W, Lumen::Action::ActionName::MOVE_UP);
                Lumen::Scene::BaseScene::RegisteAction(sf::Keyboard::Key::A, Lumen::Action::ActionName::MOVE_LEFT);
                Lumen::Scene::BaseScene::RegisteAction(sf::Keyboard::Key::S, Lumen::Action::ActionName::MOVE_DOWN);
                Lumen::Scene::BaseScene::RegisteAction(sf::Keyboard::Key::D, Lumen::Action::ActionName::MOVE_RIGHT);

                Lumen::Scene::BaseScene::RegisteAction(sf::Keyboard::Key::Up, Lumen::Action::ActionName::MOVE_UP);
                Lumen::Scene::BaseScene::RegisteAction(sf::Keyboard::Key::Left, Lumen::Action::ActionName::MOVE_LEFT);
                Lumen::Scene::BaseScene::RegisteAction(sf::Keyboard::Key::Down, Lumen::Action::ActionName::MOVE_DOWN);
                Lumen::Scene::BaseScene::RegisteAction(sf::Keyboard::Key::Right, Lumen::Action::ActionName::MOVE_RIGHT);

                Lumen::Scene::BaseScene::m_action_manager.SetActionKind(Lumen::Action::ActionName::MOVE_UP, Lumen::Action::ActionKind::TRIGGER);
                Lumen::Scene::BaseScene::m_action_manager.SetActionKind(Lumen::Action::ActionName::MOVE_LEFT, Lumen::Action::ActionKind::TRIGGER);
                Lumen::Scene::BaseScene::m_action_manager.SetActionKind(Lumen::Action::ActionName::MOVE_DOWN, Lumen::Action::ActionKind::TRIGGER);
                Lumen::Scene::BaseScene::m_action_manager.SetActionKind(Lumen::Action::ActionName::MOVE_RIGHT, Lumen::Action::ActionKind::TRIGGER);

                Lumen::Scene::BaseScene::RegisteAction(sf::Keyboard::Key::Escape, Lumen::Action::ActionName::QUIT);
                Lumen::Scene::BaseScene::m_action_manager.SetActionKind(Lumen::Action::ActionName::QUIT, Lumen::Action::ActionKind::TRIGGER);

                Lumen::Scene::BaseScene::RegisteAction(sf::Keyboard::Key::Enter, Lumen::Action::ActionName::START_SELECTED_LEVEL);
                Lumen::Scene::BaseScene::m_action_manager.SetActionKind(Lumen::Action::ActionName::START_SELECTED_LEVEL, Lumen::Action::ActionKind::TRIGGER);
                //for (auto &actionKeyCodePair : menuSceneConfigData.actionKeyCodePairVector) {
                //        this->m_action_manager.RegisteAction(actionKeyCodePair.keyCode, actionKeyCodePair.actionName);
                //}
        }

        constexpr void InitLayerStack(void) noexcept
        {
                assert(!this->m_is_initialized);
                Lumen::Scene::BaseScene::m_layer_stack.PushBackLayer(Lumen::LayerStack::MakeLayer<MenuLayer::TestUILayer>(this));
                Lumen::Scene::BaseScene::m_layer_stack.PushBackLayer(Lumen::LayerStack::MakeLayer<MenuLayer::TestBackgroundLayer>());
        }

        constexpr void CreateActions(void) noexcept
        {
                assert(this->m_is_initialized);
                for (std::optional<sf::Event> optional_event = Lumen::Scene::BaseScene::m_window_ptr->pollEvent();
                     optional_event.has_value();
                     optional_event = Lumen::Scene::BaseScene::m_window_ptr->pollEvent()) {
                        sf::Event &event = optional_event.value();
                        Lumen::Scene::BaseScene::m_action_manager.CreateActionFromEvent(event);
                }
        }

        constexpr void DoActions(void) noexcept
        {
                assert(this->m_is_initialized);
                for (auto action : Lumen::Scene::BaseScene::m_action_manager) {
                        for (auto layer_stack_it = Lumen::Scene::BaseScene::m_layer_stack.rbegin();
                             layer_stack_it != Lumen::Scene::BaseScene::m_layer_stack.rend();
                             ++layer_stack_it) {
                                Lumen::LayerStack::LayerPtr &layer_ptr = (*layer_stack_it);
                                if (Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked == layer_ptr->DoAction(action)) {
                                        break;
                                }
                        }
                }//std::cout << "[MenuScene] DoActions end\n";
        }
};

} // namespace Scene
} // namespace Lumen
