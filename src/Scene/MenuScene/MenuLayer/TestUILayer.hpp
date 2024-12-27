
#pragma once

#include <Scene/Scene.hpp>
#include <UI/Components/BasicUIComponent.hpp>
#include <UI/Components/Menu.hpp>

#include <cassert>
#include <iostream>

namespace Lumen {
namespace Scene {
namespace MenuLayer {

class TestUILayer : public Lumen::LayerStack::BaseLayer {
private:

        Lumen::Scene::BaseScene *m_menu_scene;
        std::vector<std::unique_ptr<Lumen::UI::Component::BasicUIComponent>> m_ui_components;

public:
        constexpr TestUILayer(Lumen::Scene::BaseScene *menu_scene) noexcept
        : m_menu_scene{menu_scene}
        {
                assert(nullptr != this->m_menu_scene);

                auto window_size = this->m_menu_scene->m_window_ptr->getSize();
                std::cout << "[MenuScene:TestUILayer]: Window position: (" << window_size.x << ", " << window_size.y << ")\n";
                Lumen::Core::Math::Vec2i window_center{static_cast<int>((window_size.x) / 2),
                                                       static_cast<int>((window_size.y) / 2)};
                std::cout << "[MenuScene:TestUILayer]: Window center: (" << window_center.x << ", " << window_center.y << ")\n";

                constexpr const int scale = 5;
                constexpr const Lumen::Core::Math::Vec2i main_menu_size = {50 * scale, 70 * scale};
                constexpr const Lumen::Core::Math::Vec2i main_button_menu_size = {50 * scale, 10 * scale};
                Lumen::Core::Math::Vec2i main_menu_position{window_center.x - (main_menu_size.x / 2),
                                                            window_center.y - (main_menu_size.y/2)};
                
                auto menu_ptr = std::make_unique<Lumen::UI::Component::Menu>(
                        Lumen::UI::Component::TransformTopLeft{main_menu_position},
                        Lumen::UI::Component::BoundingBox{main_menu_size},
                        Lumen::UI::Component::BoundingBox{main_button_menu_size},
                        this->m_menu_scene->m_window_ptr
                );

                this->AddUIComponent(std::move(menu_ptr));
        }

        constexpr void Update(void) noexcept override
        {
                std::cout << "[ExampleLayer1]: Update\n";
                for (auto &ui_component : this->m_ui_components) {
                        ui_component->Update(0.25f);
                }
        }

        constexpr void Render(void) noexcept override
        {
                //std::cout << "[ExampleLayer1]: Render\n";
                for (auto &ui_component : this->m_ui_components) {
                        ui_component->Render();
                }
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

        constexpr void AddUIComponent(std::unique_ptr<Lumen::UI::Component::BasicUIComponent> ui_component) noexcept
        {
                this->m_ui_components.push_back(std::move(ui_component));
        }
};


} // namespace MenuLayer
} // namespace Scene
} // namespace Lumen