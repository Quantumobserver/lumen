
#pragma once

#include <Scene/Scene.hpp>
#include <UI/Components/BasicUIComponent.hpp>
#include <UI/Components/Menu.hpp>
#include <UI/Components/Button.hpp>
#include <ResourceManager/ResourceManager.hpp>
#include <UI/UIManager.hpp>
#include <Utility/Unicode/UnicodeString.hpp>

#include <SFML/Graphics/Text.hpp>

#include <cassert>
#include <iostream>

namespace Lumen {
namespace Scene {
namespace MenuLayer {

class TestUILayer : public Lumen::LayerStack::BaseLayer {
private:

        Lumen::Scene::BaseScene *m_menu_scene;
        Lumen::UI::UIManager m_ui_manager{};

        struct DoButtonActionData {
                int i;
                Lumen::Scene::InterSceneCommunicationData &inter_scene_communication_data;
                sf::RenderWindow &window;
        } m_do_button_action_data;

public:
        constexpr TestUILayer(Lumen::Scene::BaseScene *menu_scene) noexcept
        : m_menu_scene{menu_scene},
          m_do_button_action_data{0, *this->m_menu_scene->m_inter_scene_communication_data,
                                 *this->m_menu_scene->m_window_ptr}
        {
                assert(nullptr != this->m_menu_scene);

                this->Init();

                auto &resource_manager = *this->m_menu_scene->m_resource_manager_ptr;
                const sf::Font &menu_font{resource_manager.GetFont(Lumen::ResourceManager::FontID::DROID_FONT)};

                auto window_size = this->m_menu_scene->m_window_ptr->getSize();
                // std::cout << "[MenuScene:TestUILayer]: Window position: ("
                //           << window_size.x << ", " << window_size.y << ")\n";
                Lumen::Core::Math::Vec2f32 window_center{static_cast<float>(window_size.x) / 2.0f, static_cast<float>(window_size.y) / 2.0f};
                // std::cout << "[MenuScene:TestUILayer]: Window center: (" << window_center.x
                //           << ", " << window_center.y << ")\n";
                
                constexpr const float scale = 5.f;
                constexpr const Lumen::Core::Math::Vec2f32 main_menu_size = {50.0f * scale, 70.0f * scale};
                constexpr const Lumen::Core::Math::Vec2f32 main_button_menu_size = {50.0f * scale, 10.0f * scale};
                Lumen::Core::Math::Vec2f32 main_menu_position{window_center.x - (main_menu_size.x / 2.0f),
                                                            window_center.y - (main_menu_size.y / 2.0f)};
                
                auto &main_menu = this->m_ui_manager.CreateMenu();
                main_menu.SetMenuRelativeTopLeftPosition(main_menu_position);
                main_menu.SetMenuBoundingBox({{main_menu_size}});
                main_menu.SetMenuButtonBoundingBox({{main_button_menu_size}});
                main_menu.SetVisible(true);

                Lumen::UI::Component::Menu::Layout menu_layout;
                menu_layout.direction_type_tag = Lumen::UI::Component::Menu::Layout::DirectionTypeTag::VERTICAL;
                menu_layout.spacing_type_tag = Lumen::UI::Component::Menu::Layout::SpacingTypeTag::UNIFORM_DISTRIBUTION;
                menu_layout.alignment_type_tag = Lumen::UI::Component::Menu::Layout::AlignmentTypeTag::TOP_LEFT;
                main_menu.SetLayout(menu_layout);

                auto &main_menu_game_start_button = main_menu.CreateMenuButton();
                main_menu_game_start_button.SetText(&menu_font, LUMEN_UTILITY_UNICODE_STRING("开始游戏"), sf::Color::Black);
                sf::Sprite start_game_menu_button_sprite{resource_manager.GetSprite(Lumen::ResourceManager::SpriteID::MENU_BUTTON_SPRITE)};

                auto main_menu_button_sprite_x = static_cast<float>(start_game_menu_button_sprite.getTextureRect().position.x);
                auto main_menu_button_sprite_y = static_cast<float>(start_game_menu_button_sprite.getTextureRect().position.y);
                auto main_menu_button_sprite_width = static_cast<float>(start_game_menu_button_sprite.getTextureRect().size.x);
                auto main_menu_button_sprite_height = static_cast<float>(start_game_menu_button_sprite.getTextureRect().size.y);
                Lumen::UI::Component::AbsoluteTransformRectangleArea texture_rectangle_area {};
                texture_rectangle_area.top_left_position = {main_menu_button_sprite_x, main_menu_button_sprite_y};
                texture_rectangle_area.bottom_right_position = {main_menu_button_sprite_width + main_menu_button_sprite_x,
                                                              main_menu_button_sprite_height + main_menu_button_sprite_y};
                main_menu_game_start_button.SetBackground({texture_rectangle_area});

                main_menu_game_start_button.SetVisible(true);
                main_menu_game_start_button.SetActionHandler(
                        [](void *data, const Lumen::UI::Component::RelativeSelectionAction &relative_selection_action) noexcept
                        -> Lumen::UI::Component::MenuButton::ActionResult {
                                (void)data;
                                auto &do_button_action_data = *static_cast<DoButtonActionData *>(data);
                                if (Lumen::Action::SelectionAction::SelectionActionTypeTag::RELEASE ==
                                    relative_selection_action.selection_action.selection_action_type) {
                                        std::cout << "Start Game\n";
                                        do_button_action_data.inter_scene_communication_data.change_scene_args.SetChangeSceneArgs(
                                                Lumen::Scene::SceneID::MENU,
                                                Lumen::Scene::SceneID::GAME_PLAY,
                                                Lumen::Scene::ChangeSceneArgs::Args{}
                                        );
                                        do_button_action_data.inter_scene_communication_data.change_scene = true;
                                }

                                return Lumen::UI::Component::MenuButton::ActionResult{
                                        true, true, std::nullopt,
                                };
                        }, &this->m_do_button_action_data);

                main_menu.ComputeBoundingBoxAndTransform({{0, 0}});

        }

        constexpr void Init(void) noexcept
        {
                this->m_ui_manager.Init(*this->m_menu_scene->m_resource_manager_ptr, *this->m_menu_scene->m_window_ptr);
        }

        constexpr void Update(float delta_time) noexcept override
        {
                this->m_ui_manager.Update(delta_time);
        }

        constexpr void Render(void) noexcept override
        {
                this->m_ui_manager.Render(*this->m_menu_scene->m_window_ptr);
        }

        virtual DoActionResult DoSelectionAction(Lumen::Action::SelectionAction selection_action) noexcept override
        {
                Lumen::UI::Component::RelativeSelectionAction relative_selection_action{
                        selection_action,
                        selection_action.position,
                };
                this->m_ui_manager.DoSelectionAction(relative_selection_action);
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
                return Lumen::LayerStack::BaseLayer::DoActionResult::NotHandledOrNotBlocked;
        }

        [[nodiscard]] constexpr Lumen::LayerStack::BaseLayer::DoActionResult DoWindowResizeAction(void) noexcept override
        {
                // std::cout << "[MenuScene:TestUILayer]: DoWindowResizeAction\n";
                this->m_ui_manager.WindowResize(*this->m_menu_scene->m_window_ptr);
                return Lumen::LayerStack::BaseLayer::DoActionResult::NotHandledOrNotBlocked;
        }
};


} // namespace MenuLayer
} // namespace Scene
} // namespace Lumen