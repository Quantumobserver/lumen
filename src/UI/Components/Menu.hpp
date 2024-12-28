#pragma once

#include <Core/Memory/ReadWritePtr.hpp>
#include "BasicUIComponent.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
// #include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include <cassert>
#include <cstddef>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <functional>

namespace Lumen {
namespace UI {
namespace Component {

class Menu;

struct SelectedSubMenuItem {
        enum class SelectionTypeTag {
                NONE,
                HOVER,
                PRESS,
        };
        enum class SpawnSide {
                LEFT_SIDE,
                RIGHT_SIDE,
        };
        enum class SpawnAlignment {
                TOP,
                BOTTOM,
        };
        SelectionTypeTag selection_type;
        SpawnSide spawn_side;
        SpawnAlignment spawn_alignment;
        Lumen::Core::Memory::ReadWritePtr<Lumen::UI::Component::Menu> sub_menu_ptr;

        // Menu class will set this value
        bool is_selected_in_last_update{true};
        float remain_time_in_second{0.0f};
        Lumen::UI::Component::TransformRectangleArea transform_rectangle_area;

        constexpr SelectedSubMenuItem(void) noexcept
        : selection_type{SelectionTypeTag::NONE}, spawn_side{SpawnSide::LEFT_SIDE}, spawn_alignment{SpawnAlignment::TOP} {}

        constexpr SelectedSubMenuItem(SelectionTypeTag selection_type, SpawnSide spawn_side,
                                      SpawnAlignment spawn_alignment,
                                      Lumen::Core::Memory::ReadWritePtr<Lumen::UI::Component::Menu> sub_menu_ptr) noexcept
        : selection_type{selection_type}, spawn_side{spawn_side}, spawn_alignment{spawn_alignment},
          sub_menu_ptr{sub_menu_ptr}, remain_time_in_second{GetRemainDurationInSecond()} {}

        constexpr bool HasSelectedSubMenu(void) const noexcept
        {
                return SelectionTypeTag::NONE != this->selection_type;
        }


        constexpr void Clear(void) noexcept
        {
                this->selection_type = SelectionTypeTag::NONE;
                this->sub_menu_ptr = nullptr;
        }

        static constexpr float GetRemainDurationInSecond(void) noexcept
        {
                return 0.25f;
        }
};

class MenuButton {
public:
        enum class MenuButtonLabelLayout {
                CENTER,
                LEFT,
                RIGHT,
                TOP,
                BOTTOM,
        };

        using MenuButtonActionHandler =
                std::function<
                        std::optional<Lumen::UI::Component::SelectedSubMenuItem>(void *, const Lumen::UI::Component::RelativeSelectionAction &)>;
private:
        Lumen::UI::Component::TransformRectangleArea m_transform_rectangle_area;
        //std::string m_text;
        sf::Text m_text_label;
        sf::Sprite m_icon;
        void *m_do_menu_button_action_data_ptr{nullptr};
        MenuButtonActionHandler fn_do_menu_button_action;
        MenuButtonLabelLayout m_menu_button_label_layout;

        sf::Color m_outline_color_menu_button{sf::Color::Cyan};

public:
        constexpr MenuButton(sf::Text &&text, const sf::Sprite &icon,
                             void *do_menu_button_action_data_ptr = nullptr,
                             MenuButtonActionHandler &&do_menu_button_action = nullptr,
                             MenuButtonLabelLayout menue_button_layout = MenuButtonLabelLayout::CENTER) noexcept
        : m_text_label{std::move(text)}, m_icon{icon},
          m_do_menu_button_action_data_ptr{do_menu_button_action_data_ptr},
          fn_do_menu_button_action{std::move(do_menu_button_action)},
          m_menu_button_label_layout{menue_button_layout} {}

        constexpr MenuButton(const MenuButton &) noexcept = delete;
        constexpr MenuButton(MenuButton &&other) noexcept
        : m_transform_rectangle_area{std::move(other.m_transform_rectangle_area)},
          m_text_label{std::move(other.m_text_label)}, m_icon{std::move(other.m_icon)},
          m_do_menu_button_action_data_ptr{other.m_do_menu_button_action_data_ptr},
          fn_do_menu_button_action{std::move(other.fn_do_menu_button_action)},
          m_menu_button_label_layout{std::move(other.m_menu_button_label_layout)}
        {
                other.m_do_menu_button_action_data_ptr = nullptr;
        }

        constexpr MenuButton &operator=(const MenuButton &) noexcept = delete;
        constexpr MenuButton &operator=(MenuButton &&other) noexcept
        {
                if (this == &other) {
                        return *this;
                }

                this->m_text_label = std::move(other.m_text_label);
                this->m_icon = std::move(other.m_icon);
                this->m_do_menu_button_action_data_ptr = other.m_do_menu_button_action_data_ptr;
                this->fn_do_menu_button_action = std::move(other.fn_do_menu_button_action);
                this->m_menu_button_label_layout = std::move(other.m_menu_button_label_layout);
                other.m_do_menu_button_action_data_ptr = nullptr;
                return *this;
        }

        [[nodiscard]] constexpr std::optional<SelectedSubMenuItem>
        DoSelectionAction(const Lumen::UI::Component::RelativeSelectionAction &relative_selection_action) noexcept
        {
                if (this->fn_do_menu_button_action) {
                        return this->fn_do_menu_button_action(this->m_do_menu_button_action_data_ptr,
                                                              relative_selection_action);
                }
                return std::nullopt;
        }

        constexpr void SetPosition(const Lumen::UI::Component::TransformRectangleArea &rectangle_area) noexcept
        {
                this->m_transform_rectangle_area = rectangle_area;
        }

        constexpr void SetButtonLabelPosition(const Lumen::UI::Component::TransformRectangleArea &rectangle_area) noexcept
        {
                this->m_text_label.setPosition({static_cast<float>(rectangle_area.top_left_position.top_left_position.x),
                                                static_cast<float>(rectangle_area.top_left_position.top_left_position.y)});
        }

        constexpr void SetPosition(const Lumen::UI::Component::TransformTopLeft &transform_top_left,
                                   const Lumen::UI::Component::BoundingBox &bounding_box) noexcept
        {
                this->m_transform_rectangle_area = {transform_top_left, bounding_box};
        }

        constexpr void SetButtonLabelPosition(const Lumen::UI::Component::TransformTopLeft &transform_top_left) noexcept
        {
                this->m_text_label.setPosition({static_cast<float>(transform_top_left.top_left_position.x),
                                                static_cast<float>(transform_top_left.top_left_position.y)});
        }

        constexpr void Move(const Lumen::Core::Math::Vec2i &offset) noexcept
        {
                this->m_transform_rectangle_area.Move(offset);
        }

        constexpr const Lumen::UI::Component::TransformRectangleArea &GetTransformRectangleArea(void) const noexcept
        {
                return this->m_transform_rectangle_area;
        }

        constexpr void SetColor(const sf::Color &color) noexcept
        {
                this->m_outline_color_menu_button = color;
        }

        constexpr const sf::Color &GetColor(void) const noexcept
        {
                return this->m_outline_color_menu_button;
        }

        constexpr void SetText(const sf::Text &text) noexcept
        {
                this->m_text_label = text;
        }

        constexpr const sf::Text &GetText(void) const noexcept
        {
                //std::cout << "MenuButton::SetText() called\n";
                //std::cout << "Text: " << this->m_text_label.getString().toAnsiString() << "\n";
                return this->m_text_label;
        }

        constexpr MenuButtonLabelLayout GetMenuButtonLabelLayout(void) const noexcept
        {
                return this->m_menu_button_label_layout;
        }

        constexpr void SetMenuButtonLabelLayout(MenuButtonLabelLayout menu_button_label_layout) noexcept
        {
                this->m_menu_button_label_layout = menu_button_label_layout;
        }
};

class Menu : public Lumen::UI::Component::BasicUIComponent {
public:
        enum class MenuLayoutTypeTag {
                VERTICAL_UNIFORM_DISTRIBUTION,
                VERTICAL_FIXED_SPACING_AUTO,
                //HORIZONTAL,
        };

        using MenuButtonActionHandler = typename Lumen::UI::Component::MenuButton::MenuButtonActionHandler;
        using SelectedSubMenuItem = Lumen::UI::Component::SelectedSubMenuItem;

        struct MenuLayout {
                MenuLayoutTypeTag menu_layout_type;

                struct Void {};
                struct VerticalFixedSpacingAutoData {
                        int fixed_spacing_header_y;
                        int fixed_spacing_y;
                        int fixed_spacing_footer_y;
                };
                union {
                        Void no_data;
                        VerticalFixedSpacingAutoData vertical_fixed_spacing_auto_data;
                };

                constexpr MenuLayout(void) noexcept : menu_layout_type{
                        MenuLayoutTypeTag::VERTICAL_UNIFORM_DISTRIBUTION}, no_data{} {}

                constexpr MenuLayout(MenuLayoutTypeTag menu_layout_type) noexcept 
                : menu_layout_type{menu_layout_type}, no_data{}
                {
                        assert(MenuLayoutTypeTag::VERTICAL_UNIFORM_DISTRIBUTION == this->menu_layout_type);
                }

                constexpr MenuLayout(const VerticalFixedSpacingAutoData &vertical_fixed_spacing_auto_data) noexcept
                : menu_layout_type{MenuLayoutTypeTag::VERTICAL_FIXED_SPACING_AUTO},
                  vertical_fixed_spacing_auto_data{vertical_fixed_spacing_auto_data} {}
        };

private:

        Lumen::UI::Component::TransformRectangleArea m_transform_rectangle_area;
        Lumen::UI::Component::BoundingBox m_bounding_box_menu;
        Lumen::UI::Component::BoundingBox m_bounding_box_menu_button;
        std::vector<Lumen::UI::Component::MenuButton> m_buttons;
        std::vector<std::unique_ptr<Lumen::UI::Component::Menu> > m_sub_menus;
        SelectedSubMenuItem m_selected_sub_menu;

        MenuLayout m_menu_layout;
        //std::vector<std::unique_ptr<Lumen::UI::Component::Label> > m_lebels;

        sf::Color m_outline_color_menu{sf::Color::Green};
        sf::RectangleShape m_rectangle_shape;

public:
        Menu(void) noexcept = default;

        Menu(const Lumen::UI::Component::TransformTopLeft &transform_top_left,
             const Lumen::UI::Component::BoundingBox &bounding_box_menu,
             const Lumen::UI::Component::BoundingBox &bounding_box_menu_button,
             sf::RenderWindow *window_ptr,
             std::vector<Lumen::UI::Component::MenuButton> &&buttons =
                std::vector<Lumen::UI::Component::MenuButton>{},
             std::vector<std::unique_ptr<Lumen::UI::Component::Menu> > &&sub_menus =
                std::vector<std::unique_ptr<Lumen::UI::Component::Menu> >{},
             MenuLayoutTypeTag menu_layout_type = MenuLayoutTypeTag::VERTICAL_UNIFORM_DISTRIBUTION
                ) noexcept
        : Lumen::UI::Component::BasicUIComponent{Lumen::UI::Component::UIComponentTypeTag::MENU, window_ptr},
          m_transform_rectangle_area{transform_top_left, bounding_box_menu},
          m_bounding_box_menu{bounding_box_menu},
          m_bounding_box_menu_button{bounding_box_menu_button}, m_buttons{std::move(buttons)},
          m_sub_menus{std::move(sub_menus)},
          m_menu_layout{menu_layout_type}
        {
                //std::cout << "))))))))))))))))))))))))))))\n";
                this->Init();

                // std::cout << __FILE__ " :" << __LINE__ << "\n";
                // const sf::Text &text = this->m_buttons[0].GetText();
                // const sf::Font &font = this->m_buttons[0].GetText().getFont();
                //const sf::Text &text_o = this->m_buttons[0].GetText();
//std::cout << __FILE__ " :" << __LINE__ << "\n";
               // const sf::Font &font = text_o.getFont();
//std::cout << __FILE__ " :" << __LINE__ << ", text addr: " << &text << ", font addr: " << &font << "\n";
                // sf::Text text{font, "Hello, World!"};
                // this->m_window_ptr->draw(text);
                // this->m_window_ptr->draw(this->m_buttons[0].GetText());
                // this->m_window_ptr->display();
                // std::string i;
                // std::cin >> i;
        }
        
        Menu(const Lumen::UI::Component::TransformTopLeft &transform_top_left,
             const Lumen::UI::Component::BoundingBox &bounding_box_menu_button,
             sf::RenderWindow *window_ptr,
             std::vector<Lumen::UI::Component::MenuButton> &&buttons =
                std::vector<Lumen::UI::Component::MenuButton>{},
             const MenuLayout::VerticalFixedSpacingAutoData &vertical_fixed_spacing_auto_data = {0, 0, 0},
             std::vector<std::unique_ptr<Lumen::UI::Component::Menu> > &&sub_menus =
                std::vector<std::unique_ptr<Lumen::UI::Component::Menu> >{}
                ) noexcept
        : Lumen::UI::Component::BasicUIComponent{Lumen::UI::Component::UIComponentTypeTag::MENU, window_ptr},
          //m_transform_rectangle_area{transform_top_left},
          //m_transform_top_left{transform_center.center_position - bounding_box_menu.half_size},
          //m_bounding_box_menu{bounding_box_menu},
          m_bounding_box_menu_button{bounding_box_menu_button}, m_buttons{std::move(buttons)},
          m_sub_menus{std::move(sub_menus)},
          m_menu_layout{vertical_fixed_spacing_auto_data}
        {
                assert(MenuLayoutTypeTag::VERTICAL_FIXED_SPACING_AUTO == this->m_menu_layout.menu_layout_type);

                const auto menu_layout = this->m_menu_layout;
                const auto menu_layout_data = menu_layout.vertical_fixed_spacing_auto_data;
                const auto fixed_spacing_header_y = menu_layout_data.fixed_spacing_header_y;
                const auto fixed_spacing_footer_y = menu_layout_data.fixed_spacing_footer_y;
                const auto fixed_spacing_y = menu_layout_data.fixed_spacing_y;

                this->m_bounding_box_menu = {{
                        this->m_bounding_box_menu_button.size.x,
                        fixed_spacing_header_y + fixed_spacing_footer_y +
                        (this->m_bounding_box_menu_button.size.y) * static_cast<int>(this->m_buttons.size()) +
                        (fixed_spacing_y * static_cast<int>(this->m_buttons.size() - 1)),
                }};

                this->m_transform_rectangle_area = {transform_top_left, this->m_bounding_box_menu};

                this->Init();
                
        }

        Menu(const Menu &) noexcept = delete;
        Menu(Menu &&other) noexcept = default;
        
        Menu &operator=(const Menu &) noexcept = delete;
        Menu &operator=(Menu &&other) noexcept = default;

        constexpr void InitMenuButtonPosition(void) noexcept
        {
                for (auto iterator = this->m_buttons.begin(); iterator != this->m_buttons.end(); ++iterator) {
                        const auto index = std::distance(this->m_buttons.begin(), iterator);

                        const auto menu_button_top_left_position = this->GetMenuButtonTopLeftPosition(index);

                        // std::cout << "menu_button_center_position_relative_to_menu_top_left[" << index << "]: { "
                        //           << menu_button_center_position_relative_to_menu_top_left.x << ", "
                        //           << menu_button_center_position_relative_to_menu_top_left.y << " }\n";

                        iterator->SetPosition(menu_button_top_left_position, this->m_bounding_box_menu_button);
                        // iterator->SetButtonLabelPosition({
                        //         menu_button_top_left_position.top_left_position +
                        //         this->m_transform_rectangle_area.top_left_position.top_left_position
                        // });
                        this->UpdateMenuButtonTextLabelPosition(*iterator, menu_button_top_left_position.top_left_position + 
                                                                this->m_transform_rectangle_area.top_left_position.top_left_position );

                }
        }


        constexpr void Init(void) noexcept override
        {
                this->InitMenuButtonPosition();
        }

        constexpr void Update(float delta_time) noexcept override
        {
                if (!this->m_selected_sub_menu.HasSelectedSubMenu() || 
                    this->m_selected_sub_menu.is_selected_in_last_update) {
                        return;
                }

std::cout << "Update: " << __LINE__ << ": delta_time=" << delta_time
          << ", this->m_selected_sub_menu.remain_time_in_second="
          << this->m_selected_sub_menu.remain_time_in_second << "\n";
                this->m_selected_sub_menu.remain_time_in_second -= delta_time;
                if (this->m_selected_sub_menu.remain_time_in_second <= 0.0f) {
                        this->UnSelectSubMenu();
                }
        }

        constexpr void Render(void) noexcept override
        {
                this->DrawMenuBoundingBox();
                this->DrawMenuButtonBoundingBoxForEach();
                this->DrawSelectedSubMenu();
                this->DrawMenuButton();
        }

        constexpr sf::Color GetColorWhenNotSelected(void) const noexcept
        {
                return sf::Color::Green;
        }

        constexpr void SetColor(const sf::Color &color) noexcept
        {
                this->m_outline_color_menu = color;
        }

        constexpr void UnSelectSubMenu(void) noexcept
        {
                //assert(nullptr != this->m_selected_sub_menu.sub_menu_ptr);
                if (!this->m_selected_sub_menu.HasSelectedSubMenu()) {
                        return;
                }
                //std::cout << "UnSelectSubMenu: " << __LINE__ << "\n";
                const auto sub_menu_color_unselected = this->m_selected_sub_menu.sub_menu_ptr->GetColorWhenNotSelected();
                this->m_selected_sub_menu.sub_menu_ptr->SetColor(sub_menu_color_unselected);
                this->m_selected_sub_menu.Clear();
        }

        constexpr void DrawMenuBoundingBox(void) noexcept 
        {
                constexpr const float OUTLINE_THICKNESS_MENU = 3.0f;
                this->m_rectangle_shape.setOutlineThickness(OUTLINE_THICKNESS_MENU);

                this->m_rectangle_shape.setPosition({
                        static_cast<float>(this->m_transform_rectangle_area.top_left_position.top_left_position.x),
                        static_cast<float>(this->m_transform_rectangle_area.top_left_position.top_left_position.y)});

                this->m_rectangle_shape.setSize({
                        static_cast<float>(this->m_bounding_box_menu.size.x) - (OUTLINE_THICKNESS_MENU * 2.0f),
                        static_cast<float>(this->m_bounding_box_menu.size.y) - (OUTLINE_THICKNESS_MENU * 2.0f)
                });
                this->m_rectangle_shape.setOutlineColor(this->m_outline_color_menu);
                this->m_rectangle_shape.setFillColor(sf::Color::Transparent);
                //this->m_rectangle_shape.setOrigin(this->m_rectangle_shape.getSize() / 2.0f);
                this->m_window_ptr->draw(m_rectangle_shape);
        }

        constexpr void  DrawMenuButtonBoundingBoxForEach(void) noexcept
        {
                //for (auto iterator = this->m_buttons.begin(); iterator != this->m_buttons.end(); ++iterator) {
                //        const auto index = std::distance(this->m_buttons.begin(), iterator);
                for (const auto &menu_button : this->m_buttons) {

                        //this->DrawMenuButtonBoundingBox(index, *iterator);
                        this->DrawMenuButtonBoundingBox(menu_button);
                }
        }

        constexpr void DrawMenuButton(void) const noexcept
        {
                //std::cout << "size: " << this->m_buttons.size() << "\n";
                for (const auto &menu_button : this->m_buttons) {
                        //std::cout << "DrawMenuButton: " << __LINE__ << "\n";
                        this->DrawMenuButtonTextLabel(menu_button);
                }
        }

        constexpr void DrawMenuButtonTextLabel(const MenuButton &menu_button) const noexcept
        {
        //         std::cout << "DrawMenuButtonTextLabel: " << __LINE__ << "\n";
        //         std::cout << "DrawMenuButtonTextLabel Text: " << menu_button.GetText().getString().toAnsiString() << "\n";
        // std::cout << "win_ptr:" << this->m_window_ptr << "\n";
        //         const sf::Text &text{menu_button.GetText()};
        // std::cout << "DrawMenuButtonTextLabel: " << __LINE__ << "\n";
        //         const sf::Drawable &drawable{dynamic_cast<const sf::Drawable &>(text)};
        // std::cout << "DrawMenuButtonTextLabel: " << __LINE__ << "\n";
        //         this->m_window_ptr->draw(drawable);
        //std::cout << "DrawMenuButtonTextLabel: " << __LINE__ << "\n";
                this->m_window_ptr->draw(menu_button.GetText());
        }

        constexpr void DrawMenuButtonBoundingBox(
                //const typename std::iterator_traits<decltype(m_buttons)::iterator>::difference_type index,
                const Lumen::UI::Component::MenuButton &menu_button) noexcept
        {
                constexpr const float outline_thickness_menu_button = 1.0f;
                m_rectangle_shape.setOutlineThickness(outline_thickness_menu_button);
                //const auto menu_button_ralative_center_position = this->GetMenuButtonRelativeCenterPosition(index);
                //const auto menu_button_position = this->m_transform_top_left.top_left_position + menu_button_ralative_center_position.center_position;
                const auto menu_button_transform_rectangle_area = menu_button.GetTransformRectangleArea();
                const auto menu_button_relative_top_left_position = menu_button_transform_rectangle_area.top_left_position;
                const auto menu_button_top_left_position =
                        this->m_transform_rectangle_area.top_left_position.top_left_position +
                        menu_button_relative_top_left_position.top_left_position;

                m_rectangle_shape.setPosition({
                        static_cast<float>(menu_button_top_left_position.x),
                        static_cast<float>(menu_button_top_left_position.y)});
                m_rectangle_shape.setSize({
                        static_cast<float>(this->m_bounding_box_menu_button.size.x) - (outline_thickness_menu_button * 2.0f),
                        static_cast<float>(this->m_bounding_box_menu_button.size.y) - (outline_thickness_menu_button * 2.0f)
                });
                m_rectangle_shape.setOutlineColor(menu_button.GetColor());
                m_rectangle_shape.setFillColor(sf::Color::Transparent);
                //m_rectangle_shape.setOrigin(m_rectangle_shape.getSize() / 2.0f);
                this->m_window_ptr->draw(m_rectangle_shape);
        }

        constexpr void DrawSelectedSubMenu(void) noexcept
        {
                if (Lumen::UI::Component::SelectedSubMenuItem::SelectionTypeTag::NONE != this->m_selected_sub_menu.selection_type) {
                        this->m_selected_sub_menu.sub_menu_ptr->Render();
                }
        }

        constexpr void DoWindowResizeAction(const Lumen::Core::Math::Vec2i &window_new_size) noexcept override
        {
                // TODO
                (void)window_new_size;
        }

private:
        constexpr bool IsMenuSelected(
                const Lumen::UI::Component::RelativeSelectionAction &relative_selection_action) const noexcept
        {
                return Lumen::UI::Component::IsIncluded(
                        relative_selection_action.relative_position_to_the_parent_ui_component,
                        this->m_transform_rectangle_area
                );
        }

        constexpr Lumen::UI::Component::TransformTopLeft GetMenuButtonTopLeftPosition(
                const typename std::iterator_traits<decltype(m_buttons)::iterator>::difference_type index) const noexcept
        {
                /*auto calculate_button_relative_position_vertical_uniform_distribution =
                        [&](void) -> Lumen::UI::Component::Transform {
                        const auto menu_top_position_y = this->m_transform.position.y - this->m_bounding_box_menu.half_size.y;
                        const auto menu_button_at_index_zero_position_y = menu_top_position_y + this->m_bounding_box_menu_button.half_size.y;
                        return {{(this->m_transform.position.x),
                                (menu_button_at_index_zero_position_y +
                                 (this->m_bounding_box_menu_button.size.y * static_cast<int>(index)))}};
                };*/
                auto calculate_button_relative_center_position_vertical_uniform_distribution =
                        [this, index](void) -> Lumen::UI::Component::TransformTopLeft {
                        const auto used_space_y = static_cast<int>(this->m_buttons.size()) *
                                                                   this->m_bounding_box_menu_button.size.y;
                        const auto free_space_y = this->m_bounding_box_menu.size.y - used_space_y;
                        assert(free_space_y >= 0);
                        const auto uniform_space = free_space_y / static_cast<int>(this->m_buttons.size() + 1);
                        const auto offset_y = (static_cast<int>(index + 1) * uniform_space) +
                                              (static_cast<int>(index) * this->m_bounding_box_menu_button.size.y);
                        return {{0, offset_y,}};
                };

                auto calculate_button_relative_center_position_vertical_fixed_spacing = 
                        [this, index](void) -> Lumen::UI::Component::TransformTopLeft {
                        const auto &menu_layout = this->m_menu_layout;
                        const auto menu_layout_data = menu_layout.vertical_fixed_spacing_auto_data;
                        const auto fixed_spacing_header_y = menu_layout_data.fixed_spacing_header_y;
                        const auto fixed_spacing_y = menu_layout_data.fixed_spacing_y;

                        const auto offset_y = fixed_spacing_header_y +
                                              ((this->m_bounding_box_menu_button.size.y + fixed_spacing_y) *
                                               static_cast<int>(index));
                        return {{0, offset_y,}};
                };

                switch (this->m_menu_layout.menu_layout_type) {
                case MenuLayoutTypeTag::VERTICAL_UNIFORM_DISTRIBUTION:{
                        return calculate_button_relative_center_position_vertical_uniform_distribution();
                        break;
                }

                case MenuLayoutTypeTag::VERTICAL_FIXED_SPACING_AUTO:{
                        return calculate_button_relative_center_position_vertical_fixed_spacing();
                        break;
                }

                default:
                        break;
                }
                std::abort();
        }

        constexpr bool IsMenuButtonSelected(
                const Lumen::UI::Component::RelativeSelectionAction &relative_selection_action,
                const Lumen::UI::Component::MenuButton &menu_button) const noexcept
        {
                return Lumen::UI::Component::IsIncluded(
                        relative_selection_action.relative_position_to_the_parent_ui_component,
                        menu_button.GetTransformRectangleArea()
                );
        }

        constexpr void SubMenuIsNotSelectedCurrently(void) noexcept
        {
                this->m_selected_sub_menu.is_selected_in_last_update = false;
                this->m_selected_sub_menu.remain_time_in_second = SelectedSubMenuItem::GetRemainDurationInSecond();
        }

        void UpdateMenuButtonTextLabelPosition(MenuButton &button, Lumen::Core::Math::Vec2i button_position) {
                Lumen::Core::Math::Vec2i text_bounds = {static_cast<int>(button.GetText().getLocalBounds().size.x),
                                                        static_cast<int>(button.GetText().getLocalBounds().size.y)};
                Lumen::Core::Math::Vec2i button_size = this->m_bounding_box_menu_button.size;
                Lumen::Core::Math::Vec2i position;

                switch (button.GetMenuButtonLabelLayout()) {
                case MenuButton::MenuButtonLabelLayout::CENTER:
                        std::cout << "Center\n";
                        position = button_position + (button_size - text_bounds) / 2 - 
                                   Lumen::Core::Math::Vec2i{static_cast<int>(button.GetText().getLocalBounds().position.x),
                                   static_cast<int>(button.GetText().getLocalBounds().position.y)};
                        break;
                case MenuButton::MenuButtonLabelLayout::LEFT:
                        std::cout << "Left\n";
                        position.x = button_position.x - static_cast<int>(button.GetText().getLocalBounds().position.x);
                        position.y = button_position.y + (button_size.y - text_bounds.y) / 2 -
                                     static_cast<int>(button.GetText().getLocalBounds().position.y);
                        break;
                case MenuButton::MenuButtonLabelLayout::RIGHT:
                        position.x = button_position.x + button_size.x - text_bounds.x -
                                     static_cast<int>(button.GetText().getLocalBounds().position.x);
                        position.y = button_position.y + (button_size.y - text_bounds.y) / 2 -
                                     static_cast<int>(button.GetText().getLocalBounds().position.y);
                        break;
                case MenuButton::MenuButtonLabelLayout::TOP:
                        position.x = button_position.x + (button_size.x - text_bounds.x) / 2 -
                                     static_cast<int>(button.GetText().getLocalBounds().position.x);
                        position.y = button_position.y - static_cast<int>(button.GetText().getLocalBounds().position.y);
                        break;
                case MenuButton::MenuButtonLabelLayout::BOTTOM:
                        position.x = button_position.x + (button_size.x - text_bounds.x) / 2 -
                                     static_cast<int>(button.GetText().getLocalBounds().position.x);
                        position.y = button_position.y + button_size.y - text_bounds.y -
                                     static_cast<int>(button.GetText().getLocalBounds().position.y);
                        break;
                }
                button.SetButtonLabelPosition(position);
        }

public:
        constexpr void DoSelectionAction(
                const Lumen::UI::Component::RelativeSelectionAction &relative_selection_action_to_parent) noexcept override
        {
                if (this->m_selected_sub_menu.HasSelectedSubMenu() &&
                    this->m_selected_sub_menu.sub_menu_ptr->IsMenuSelected(relative_selection_action_to_parent)) {
                        this->m_selected_sub_menu.is_selected_in_last_update = true;
                        this->m_selected_sub_menu.sub_menu_ptr->DoSelectionAction(relative_selection_action_to_parent);
                        return;
                }

                if (!this->IsMenuSelected(relative_selection_action_to_parent)) {
                        this->m_outline_color_menu = sf::Color::Green;
                        for (auto iterator = this->m_buttons.begin(); iterator != this->m_buttons.end(); ++iterator) {
                                iterator->SetColor(sf::Color::Blue);
                        }
                        if (Lumen::UI::Component::SelectedSubMenuItem::SelectionTypeTag::HOVER == 
                            this->m_selected_sub_menu.selection_type) {
                                //this->m_selected_sub_menu.Clear();
                                //std::cout << "UnSelectSubMenu: " << __LINE__ << "\n";
                                this->SubMenuIsNotSelectedCurrently();
                        }
                        return;
                }
                this->m_outline_color_menu = sf::Color::Red;

                const Lumen::UI::Component::RelativeSelectionAction relative_selection_to_menu{
                        relative_selection_action_to_parent.selection_action,
                        (relative_selection_action_to_parent.relative_position_to_the_parent_ui_component -
                         this->m_transform_rectangle_area.top_left_position.top_left_position),
                };

                bool is_selection_handled{false};
                for (auto &menu_button : this->m_buttons) {
        
                        if (is_selection_handled || (!this->IsMenuButtonSelected(relative_selection_to_menu, menu_button))) {
                                menu_button.SetColor(sf::Color::Blue);
                                continue;
                        }

                        menu_button.SetColor(sf::Color::Yellow);
                        auto selected_sub_menu = menu_button.DoSelectionAction(relative_selection_to_menu);
                        if (selected_sub_menu.has_value()) {
                                this->m_selected_sub_menu = selected_sub_menu.value();
                                this->SetSelectedSubMenuPosition(menu_button);
                        } else {
                                //this->m_selected_sub_menu.Clear();
                                //std::cout << "UnSelectSubMenu: " << __LINE__ << "\n";
                                this->SubMenuIsNotSelectedCurrently();
                        }
                        is_selection_handled = true;

                }

                if (is_selection_handled) {
                        return;
                }
                //this->m_selected_sub_menu.Clear();
                //std::cout << "UnSelectSubMenu: " << __LINE__ << "\n";
                this->SubMenuIsNotSelectedCurrently();
        }

private:

        static constexpr int ComputeSubMenuTopLeftPositionXValue(
                const Lumen::UI::Component::Menu::SelectedSubMenuItem::SpawnSide spawn_side,
                const Lumen::UI::Component::TransformTopLeft &selected_menu_botton_top_left_transform,
                const Lumen::UI::Component::BoundingBox &selected_menu_button_bounding_box,
                const Lumen::UI::Component::BoundingBox &sub_menu_bounding_box
        ) noexcept 
        {
                constexpr const int SUB_MENU_OFFSET_X = 3;
                switch (spawn_side) {
                case Lumen::UI::Component::SelectedSubMenuItem::SpawnSide::LEFT_SIDE:
                        return selected_menu_botton_top_left_transform.top_left_position.x -
                               sub_menu_bounding_box.size.x +
                               SUB_MENU_OFFSET_X;
                case Lumen::UI::Component::SelectedSubMenuItem::SpawnSide::RIGHT_SIDE:
                        return selected_menu_botton_top_left_transform.top_left_position.x +
                               selected_menu_button_bounding_box.size.x -
                               SUB_MENU_OFFSET_X;
                }

                std::abort();
        }

        constexpr int ComputeSelectedSubMenuCenterYPosition(
                const Lumen::UI::Component::SelectedSubMenuItem::SpawnAlignment spawn_alignment,
                const Lumen::UI::Component::TransformTopLeft &selected_menu_botton_top_left_transform,
                const Lumen::UI::Component::BoundingBox &selected_menu_button_bounding_box,
                const Lumen::UI::Component::BoundingBox &sub_menu_bounding_box
        ) noexcept
        {
                switch (spawn_alignment) {
                case Lumen::UI::Component::SelectedSubMenuItem::SpawnAlignment::TOP:
                        return selected_menu_botton_top_left_transform.top_left_position.y;
                case Lumen::UI::Component::SelectedSubMenuItem::SpawnAlignment::BOTTOM:
                        return selected_menu_botton_top_left_transform.top_left_position.y +
                               selected_menu_button_bounding_box.size.y -
                               sub_menu_bounding_box.size.y;
                }

                std::abort();
        }


        constexpr void SetSelectedSubMenuPosition(const Lumen::UI::Component::MenuButton &menu_button) noexcept
        {
                const auto &menu_button_top_left_transform = menu_button.GetTransformRectangleArea();
                //const auto &menu_button_top_left_position = menu_button_top_left_transform.top_left_position;

                const auto &sub_menu_bounding_box = this->m_selected_sub_menu.sub_menu_ptr->GetMenuBoundingBox();

                const Lumen::UI::Component::TransformTopLeft sub_menu_relative_position_top_left = {{
                        this->ComputeSubMenuTopLeftPositionXValue(
                                this->m_selected_sub_menu.spawn_side,
                                menu_button_top_left_transform.top_left_position,
                                this->m_bounding_box_menu_button,
                                sub_menu_bounding_box
                        ),
                        this->ComputeSelectedSubMenuCenterYPosition(
                                this->m_selected_sub_menu.spawn_alignment,
                                menu_button_top_left_transform.top_left_position,
                                this->m_bounding_box_menu_button,
                                sub_menu_bounding_box
                        ),
                }};

                const auto sub_menu_position_top_left = 
                        this->m_transform_rectangle_area.top_left_position.top_left_position +
                        sub_menu_relative_position_top_left.top_left_position;

                this->m_selected_sub_menu.sub_menu_ptr->SetPosition(
                        sub_menu_position_top_left
                );

        }


        constexpr const Lumen::UI::Component::BoundingBox &GetMenuBoundingBox(void) const noexcept
        {
                return this->m_bounding_box_menu;
        }

public:
        constexpr void AddMenuButton(Lumen::UI::Component::MenuButton &&menu_button) noexcept
        {
                this->m_buttons.push_back(std::move(menu_button));
        }

        constexpr void SetPosition(const Lumen::Core::Math::Vec2i &position) noexcept override
        {
                const auto position_move_offset =
                        position - this->m_transform_rectangle_area.top_left_position.top_left_position;
                this->Move(position_move_offset);
        }

        constexpr void Move(const Lumen::Core::Math::Vec2i &position_move_offset) noexcept
        {
                this->m_transform_rectangle_area.Move(position_move_offset);
        }


        constexpr virtual void SetSize(const Lumen::Core::Math::Vec2i &size) noexcept override
        {
                this->m_bounding_box_menu.size = size;
                //this->m_bounding_box_menu.half_size = size / 2;
        }

        //constexpr virtual void SetText([[maybe_unused]]std::string &&text) noexcept {}
        //constexpr virtual void SetIcon([[maybe_unused]]const sf::Sprite &sprite) noexcept {}

        constexpr std::optional<Lumen::Core::Math::Vec2i> GetPosition(void) const noexcept override
        {
                return {this->m_transform_rectangle_area.top_left_position.top_left_position};
        }

        constexpr std::optional<Lumen::Core::Math::Vec2i> GetSize(void) const noexcept override
        {
                return std::nullopt;
        }

        constexpr std::optional<std::string_view> GetText(void) const noexcept override
        {
                return std::nullopt;
        }

        constexpr std::optional<sf::Sprite> GetIcon(void) const noexcept override
        {
                return std::nullopt;
        }
};

} // namespace Component
} // namespace UI        
} // namespace Lumen
