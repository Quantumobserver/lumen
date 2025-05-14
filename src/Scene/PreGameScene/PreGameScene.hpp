#pragma once

#include <LumenDef/ConstexprIf.hpp>

#include <Scene/Scene.hpp>
#include "PreGameLayer/BackgroundLayer.hpp"
#include "PreGameLayer/PreGameUILayer.hpp"
#include "Map/MapGener.hpp"

namespace Lumen
{
namespace Scene
{

class PreGameScene : public Lumen::Scene::BaseScene {
private:
        bool m_is_initialized{false};
public:
        PreGameScene(sf::RenderWindow *window_ptr,
                     Lumen::ResourceManager::ResourceManager *resource_manager_ptr,
                     Lumen::ECS::Entity::EntityManager *entity_manager_ptr,
                     Lumen::Scene::InterSceneCommunicationData *inter_scene_communication_data) noexcept
         : Lumen::Scene::BaseScene{Lumen::Scene::SceneID::PRE_GAME,
                                   window_ptr,
                                   resource_manager_ptr,
                                   entity_manager_ptr,
                                   inter_scene_communication_data},
           m_is_initialized{false}
        { 
                std::cout << "[PreGameScene] Constructor: RM addr: " << resource_manager_ptr << "\n";
        }

        constexpr void Init(void) noexcept override
        {
                if (this->m_is_initialized) {
                        return;
                }

                this->InitActionManager();
                this->InitLayerStack();

                this->m_is_initialized = true;
                std::cout << "[PreGameScene] Initialized\n";
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
                // std::cout << "[PreGameScene] Update end\n";
        }

        constexpr void Render(void) noexcept override
        {
                assert(this->m_is_initialized);
                if (Lumen::Scene::BaseScene::m_inter_scene_communication_data->change_scene) {
                        // std::cout << "[PreGameScene] Render change_scene: true\n";
                }
                for (auto &layer : this->m_layer_stack) {
                        layer->Render();
                }
                // std::cout << "[PreGameScene] Render end\n";
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

                Lumen::Scene::BaseScene::RegisteAction(sf::Keyboard::Key::Escape, Lumen::Action::ActionName::QUIT);
                Lumen::Scene::BaseScene::m_action_manager.SetActionKind(Lumen::Action::ActionName::QUIT, Lumen::Action::ActionKind::TRIGGER);
        }

        CONSTEXPR_IF_CXX_23 void InitLayerStack(void) noexcept
        {
                assert(!this->m_is_initialized);
                Lumen::Scene::BaseScene::m_layer_stack.PushBackLayer(Lumen::LayerStack::MakeLayer<PreGameLayer::BackgroundLayer>(this));
                Lumen::Scene::BaseScene::m_layer_stack.PushBackLayer(Lumen::LayerStack::MakeLayer<PreGameLayer::PreGameUILayer>(this));
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
                //std::cout << "[PreGameScene] DoActions end\n";
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
                        this->SetView();
                        Lumen::Scene::BaseScene::m_action_manager.ResetWindowResizeAction();
                }
                for (auto layer_stack_it = Lumen::Scene::BaseScene::m_layer_stack.rbegin();
                     layer_stack_it != Lumen::Scene::BaseScene::m_layer_stack.rend();
                     ++layer_stack_it) {
                        Lumen::LayerStack::LayerPtr &layer_ptr = (*layer_stack_it);
                        if (Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked == layer_ptr->DoWindowResizeAction()) {
                                break;
                        }
                }
        }

        constexpr void DoWindowCloseAction(void) noexcept
        {
                if (Lumen::Scene::BaseScene::m_action_manager.IsWindowCloseActionHappened()) {
                        Lumen::Scene::BaseScene::m_inter_scene_communication_data->running = false;
                        Lumen::Scene::BaseScene::m_action_manager.ResetActionBuffer();
                }
        }

        constexpr void DoSelectionAction(void) noexcept
        {
                auto selection_action = Lumen::Scene::BaseScene::m_action_manager.GetSelectionAction();
                if (!selection_action.HasSelectionAction()) {
                        return;
                }
                for (auto layer_stack_it = Lumen::Scene::BaseScene::m_layer_stack.rbegin();
                     layer_stack_it != Lumen::Scene::BaseScene::m_layer_stack.rend();
                     ++layer_stack_it) {
                        Lumen::LayerStack::LayerPtr &layer_ptr = (*layer_stack_it);
                        if (Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked == layer_ptr->DoSelectionAction(selection_action)) {
                                break;
                        }
                }
        }


        
};

} // namespace Scene
} // namespace Lumen
