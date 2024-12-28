
#pragma once

#include <Scene/Scene.hpp>
#include <UI/Components/BasicUIComponent.hpp>
#include <UI/Components/Menu.hpp>
#include <UI/Components/Button.hpp>
#include <ResourceManager/ResourceManager.hpp>

#include <SFML/Graphics/Text.hpp>

#include <cassert>
#include <iostream>

namespace Lumen {
namespace Scene {
namespace MenuLayer {

class TestUILayer : public Lumen::LayerStack::BaseLayer {
private:

        Lumen::Scene::BaseScene *m_menu_scene;
        std::vector<std::unique_ptr<Lumen::UI::Component::BasicUIComponent>> m_ui_components;

        // Button Actions
        struct DoButtonActionData {
                int i;
                Lumen::Scene::InterSceneCommunicationData &inter_scene_communication_data;
                sf::RenderWindow &window;
        } m_do_button_action_data;

public:
        constexpr TestUILayer(Lumen::Scene::BaseScene *menu_scene) noexcept
        : m_menu_scene{menu_scene}, m_ui_components{},
          m_do_button_action_data{0, *this->m_menu_scene->m_inter_scene_communication_data,
                                  *this->m_menu_scene->m_window_ptr}
        {
                assert(nullptr != this->m_menu_scene);

                auto window_size = this->m_menu_scene->m_window_ptr->getSize();
                // std::cout << "[MenuScene:TestUILayer]: Window position: ("
                //           << window_size.x << ", " << window_size.y << ")\n";
                Lumen::Core::Math::Vec2i window_center{static_cast<int>((window_size.x) / 2),
                                                       static_cast<int>((window_size.y) / 2)};
                // std::cout << "[MenuScene:TestUILayer]: Window center: (" << window_center.x
                //           << ", " << window_center.y << ")\n";

                // Button Vector
                std::vector<Lumen::UI::Component::MenuButton> buttons{};

                // Initialize font
                auto &resource_manager = *this->m_menu_scene->m_resource_manager_ptr;
                const sf::Font &menu_font{resource_manager.GetFont(Lumen::ResourceManager::FontID::DROID_FONT)};
// std::cout << __FILE__ " :" << __LINE__ << "\n";

//                 sf::Text text{menu_font, __FILE__ ": Hello, World!"};
//                 this->m_menu_scene->m_window_ptr->draw(text);
//                 this->m_menu_scene->m_window_ptr->display();
//                 std::string i;
//                 std::cin >> i;

                sf::Text menu_button_start_game_label{menu_font, L"开始游戏"};
                menu_button_start_game_label.setFillColor(sf::Color::Black);
                menu_button_start_game_label.setStyle(sf::Text::Bold);

                // Create buttons
                auto main_menu_start_game_button = Lumen::UI::Component::MenuButton{
                        sf::Text{menu_button_start_game_label}, sf::Sprite{},
                        &this->m_do_button_action_data,
                        [](void *data, const Lumen::UI::Component::RelativeSelectionAction &relative_selection_action) noexcept
                        -> std::optional<Lumen::UI::Component::Menu::SelectedSubMenuItem> {
                                assert(nullptr != data);

                                DoButtonActionData &do_button_action_data = *static_cast<DoButtonActionData *>(data);

                                switch (relative_selection_action.selection_action.selection_action_type) {
                                case Lumen::Action::SelectionAction::SelectionActionTypeTag::NONE:
                                        std::cout << "Button None\n";
                                        break;
                                case Lumen::Action::SelectionAction::SelectionActionTypeTag::PRESS:
                                        std::cout << "[MenuButton_1]: Button Pressed " << do_button_action_data.i << "\n";
                                        ++do_button_action_data.i;
                                        break;
                                case Lumen::Action::SelectionAction::SelectionActionTypeTag::RELEASE:
                                        std::cout << "[MenuButton_1]:Button Released\n";
                                                do_button_action_data.inter_scene_communication_data.change_scene_args.SetChangeSceneArgs(
                                                        Lumen::Scene::SceneID::MENU,
                                                        Lumen::Scene::SceneID::GAME_PLAY,
                                                        Lumen::Scene::ChangeSceneArgs::Args{}
                                                );
                                                do_button_action_data.inter_scene_communication_data.change_scene = true;
                                        break;
                                case Lumen::Action::SelectionAction::SelectionActionTypeTag::CURSOR_MOVEMENT:
                                        break;
                                case Lumen::Action::SelectionAction::SelectionActionTypeTag::WHEEL_SCROLL:
                                        std::cout << "Button Wheel Scroll\n";
                                        break;
                                }

                                return std::nullopt;
                        }
                };
                main_menu_start_game_button.SetMenuButtonLabelLayout(
                        Lumen::UI::Component::MenuButton::MenuButtonLabelLayout::CENTER);

                // Add buttons to the vector
                buttons.push_back(std::move(main_menu_start_game_button));

                constexpr const int scale = 5;
                constexpr const Lumen::Core::Math::Vec2i main_menu_size = {50 * scale, 70 * scale};
                constexpr const Lumen::Core::Math::Vec2i main_button_menu_size = {50 * scale, 10 * scale};
                Lumen::Core::Math::Vec2i main_menu_position{window_center.x - (main_menu_size.x / 2),
                                                            window_center.y - (main_menu_size.y / 2)};
                
                auto menu_ptr = std::make_unique<Lumen::UI::Component::Menu>(
                        Lumen::UI::Component::TransformTopLeft{main_menu_position},
                        Lumen::UI::Component::BoundingBox{main_menu_size},
                        Lumen::UI::Component::BoundingBox{main_button_menu_size},
                        this->m_menu_scene->m_window_ptr,
                        std::move(buttons)
                );

                this->AddUIComponent(std::move(menu_ptr));
        }

        constexpr void Update(void) noexcept override
        {
                // std::cout << "[ExampleLayer1]: Update\n";
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

        virtual DoActionResult DoSelectionAction(Lumen::Action::SelectionAction selection_action) noexcept override
        {
                // Lumen::UI::Component::Menu *menu_ptr = 
                //         dynamic_cast<Lumen::UI::Component::Menu *>(
                //                 this->m_ui_components.at(this->m_ui_components.size() - 1).get());

                // std::cout << "[MenuUILayer1]: Added UI Component\n";
                // menu_ptr->Render();
                // this->m_menu_scene->m_window_ptr->display();
                // std::string i;
                // std::cin >> i;

                //std::cout << "[MenuUILayer1]: DoSelectionAction\n";
                Lumen::UI::Component::RelativeSelectionAction relative_selection_action{selection_action,selection_action.position};
                for (auto &ui_component : this->m_ui_components) {
                        //std::cout << "[MenuUILayer1_for]: DoSelectionAction\n";
                        ui_component->DoSelectionAction(relative_selection_action);
                }
                return DoActionResult::HandledOrBlocked;
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

                // Lumen::UI::Component::Menu *menu_ptr = 
                //         dynamic_cast<Lumen::UI::Component::Menu *>(
                //                 this->m_ui_components.at(this->m_ui_components.size() - 1).get());

                // std::cout << "[MenuUILayer1]: Added UI Component\n";
                // menu_ptr->Render();
                // this->m_menu_scene->m_window_ptr->display();
                // std::string i;
                // std::cin >> i;
        }
};


} // namespace MenuLayer
} // namespace Scene
} // namespace Lumen