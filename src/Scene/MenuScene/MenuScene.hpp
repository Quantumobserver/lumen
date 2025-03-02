
#pragma once

#include <LumenDef/ConstexprIf.hpp>

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
        MenuScene(sf::RenderWindow *window_ptr,
                  Lumen::ResourceManager::ResourceManager *resource_manager_ptr,
                  Lumen::ECS::Entity::EntityManager *entity_manager_ptr,
                  Lumen::Scene::InterSceneCommunicationData *inter_scene_communication_data) noexcept
         : Lumen::Scene::BaseScene{Lumen::Scene::SceneID::MENU,
                                   window_ptr,
                                   resource_manager_ptr,
                                   entity_manager_ptr,
                                   inter_scene_communication_data},
           m_is_initialized{false}
        { 
                std::cout << "[MenuScene] Constructor: RM addr: " << resource_manager_ptr << "\n";
        }

        constexpr void Init(void) noexcept override
        {
                if (this->m_is_initialized) {
                        return;
                }

                this->InitActionManager();
                this->InitLayerStack();

                this->m_is_initialized = true;

                //sf::Font font;
                // if (!font.openFromFile("../src/Assets/Fonts/DroidSansFallback.ttf")) {
                //         std::cerr << "Failed to load font\n";
                //         return;
                // }
// std::cout << __FILE__ " :" << __LINE__ << "\n";
//                 font = Lumen::Scene::BaseScene::m_resource_manager_ptr ->GetFont(Lumen::ResourceManager::FontID::DROID_FONT);
// std::cout << __FILE__ " :" << __LINE__ << "\n";
//                 sf::Text text{font, __FILE__ ": Hello, World!"};
//                 this->m_window_ptr->draw(text);
//                 this->m_window_ptr->display();
//                 std::string i;
//                 std::cin >> i;
        }


        CONSTEXPR_IF_SF_RENDER_WINDOW_POLL_EVENT
        void Update(float delta_time) noexcept override
        {
                assert(this->m_is_initialized);

                this->CreateActions();
                
                this->DoActions();
                for (auto &layer : this->m_layer_stack) {
                        layer->Update(delta_time);
                }

        }

        constexpr void Render(void) noexcept override
        {
                assert(this->m_is_initialized);
                if (Lumen::Scene::BaseScene::m_inter_scene_communication_data->change_scene) {
                        //std::cout << "[MenuScene] Render change_scene: true\n";
                }
                for (auto &layer : this->m_layer_stack) {
                        layer->Render();
                }
        }

        constexpr void ChangeToThisScene(Lumen::Scene::ChangeSceneArgs &change_scene_args) noexcept override
        {
                assert(this->m_is_initialized);
                (void)change_scene_args;
                std::cout << "--------------------------------\n";
                Lumen::Scene::BaseScene::m_window_ptr->setView(Lumen::Scene::BaseScene::m_window_ptr->getDefaultView());
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

        CONSTEXPR_IF_CXX_23 void InitLayerStack(void) noexcept
        {
                assert(!this->m_is_initialized);
                Lumen::Scene::BaseScene::m_layer_stack.PushBackLayer(Lumen::LayerStack::MakeLayer<MenuLayer::TestBackgroundLayer>(this));
                Lumen::Scene::BaseScene::m_layer_stack.PushBackLayer(Lumen::LayerStack::MakeLayer<MenuLayer::TestUILayer>(this));
        }

        CONSTEXPR_IF_SF_RENDER_WINDOW_POLL_EVENT
        void CreateActions(void) noexcept
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
                //std::cout << "[MenuScene] DoActions end\n";
                this->DoBasicAction();
                this->DoWindowResizeAction();
                this->DoWindowCloseAction();
                this->DoSelectionAction();

                Lumen::Scene::BaseScene::m_action_manager.ResetActionBuffer();
        }

        constexpr void DoBasicAction(void) noexcept
        {
                for (auto action : Lumen::Scene::BaseScene::m_action_manager) {
                        for (auto layer_stack_it = Lumen::Scene::BaseScene::m_layer_stack.rbegin();
                             layer_stack_it != Lumen::Scene::BaseScene::m_layer_stack.rend();
                             ++layer_stack_it) {
                                Lumen::LayerStack::LayerPtr &layer_ptr = (*layer_stack_it);
                                if (Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked == layer_ptr->DoAction(action)) {
                                        break;
                                }
                        }
                }
        }

        CONSTEXPR_IF_SF_VIEW_GET_VIEW void SetView(void) noexcept
        {
                auto &window = *Lumen::Scene::BaseScene::m_window_ptr;
                auto view = window.getView();
                const auto window_size = window.getSize();
                view.setSize({static_cast<float>(window_size.x), static_cast<float>(window_size.y)});

                //const auto player_position = GetPlayerPosition();
                //view.setCenter({player_position.x, player_position.y});
                window.setView(view);
        }

        constexpr void DoWindowResizeAction(void) noexcept
        {
                if (Lumen::Scene::BaseScene::m_action_manager.HasWindowResizeAction()) {
                        //std::cout << "[GamePlayScene] HasWindowResizeAction\n";
                        this->SetView();
                        Lumen::Scene::BaseScene::m_action_manager.ResetWindowResizeAction();
                }
                for (auto layer_stack_it = Lumen::Scene::BaseScene::m_layer_stack.rbegin();
                     layer_stack_it != Lumen::Scene::BaseScene::m_layer_stack.rend();
                        ++layer_stack_it) {
                        Lumen::LayerStack::LayerPtr &layer_ptr = (*layer_stack_it);
                        if (Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked == 
                            layer_ptr->DoWindowResizeAction()) {
                                break;
                        }
                }
        }

        constexpr void DoWindowCloseAction(void) noexcept
        {
                if (Lumen::Scene::BaseScene::m_action_manager.IsWindowCloseActionHappened()) {
                        //std::cout << "[GamePlayScene] IsWindowCloseActionHappened\n";
                        Lumen::Scene::BaseScene::m_inter_scene_communication_data->running = false;
                        Lumen::Scene::BaseScene::m_action_manager.ResetActionBuffer();
                        //std::cout << "[MenuScene] IsWindowCloseActionHappened\n";
                }
        }

        constexpr void DoSelectionAction(void) noexcept
        {
                //std::cout << "[MenuScene] DoSelectionAction\n";
        
                auto selection_action = Lumen::Scene::BaseScene::m_action_manager.GetSelectionAction();
                if (!selection_action.HasSelectionAction()) {
                        return;
                }
                for (auto layer_stack_it = Lumen::Scene::BaseScene::m_layer_stack.rbegin();
                     layer_stack_it != Lumen::Scene::BaseScene::m_layer_stack.rend();
                        ++layer_stack_it) {
                        Lumen::LayerStack::LayerPtr &layer_ptr = (*layer_stack_it);
                        if (Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked == 
                            layer_ptr->DoSelectionAction(selection_action)) {
                                break;
                        }
                }
        }

};

} // namespace Scene
} // namespace Lumen
