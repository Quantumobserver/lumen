#pragma once

#include <Core/Memory/ReadWritePtr.hpp>
#include "BasicUIComponent.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include <cassert>
#include <cstddef>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <functional>
#include <optional>

namespace Lumen {
namespace UI {
namespace Component {

class MenuButton;
class Menu;

struct SelectedSubMenu {
        enum class SelectionTypeTag {
                NONE,
                HOVER,
                PRESS,
        };

        SelectionTypeTag selection_type;
        Lumen::Core::Memory::ReadWritePtr<Lumen::UI::Component::Menu> sub_menu_ptr;
        const Lumen::UI::Component::MenuButton *menu_button_ptr{nullptr};

        // Menu class will set this value
        float remain_time_in_second;
        Lumen::UI::Component::TransformRectangleArea transform_rectangle_area;

        constexpr SelectedSubMenu(void) noexcept
        : selection_type{SelectionTypeTag::NONE},
          remain_time_in_second{SelectedSubMenu::GetRemainDurationInSecond()}
          {}

        constexpr SelectedSubMenu(SelectionTypeTag selection_type,
                                  Lumen::Core::Memory::ReadWritePtr<Lumen::UI::Component::Menu> sub_menu_ptr,
                                  const Lumen::UI::Component::MenuButton *menu_button_ptr) noexcept
        : selection_type{selection_type},
          sub_menu_ptr{sub_menu_ptr}, menu_button_ptr{menu_button_ptr},
          remain_time_in_second{SelectedSubMenu::GetRemainDurationInSecond()} {}

        constexpr bool IsSelectedButton(const Lumen::UI::Component::MenuButton &menu_button) const noexcept
        {
                return this->menu_button_ptr == &menu_button;
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

        constexpr void ResetRemainTime(void) noexcept
        {
                this->remain_time_in_second = SelectedSubMenu::GetRemainDurationInSecond();
        }

};

class MenuButton {
public:
        enum class LabelLayout {
                CENTER,
                LEFT,
                RIGHT,
                TOP,
                BOTTOM,
        };

        enum class TextLabelSizeTypeTag {
                USER_DEFINED,
                AUTO,
        };

        struct ActionResult {
                bool is_handled{false};
                bool is_blocked{false};
                std::optional<Lumen::UI::Component::SelectedSubMenu> selected_sub_menu;

                static constexpr ActionResult NoResult(void) noexcept
                {
                        return ActionResult{false, false, std::nullopt};
                }
        };

        using ActionHandler =
                std::function<ActionResult(void *, const Lumen::UI::Component::RelativeSelectionAction &)>;
private:
        bool m_is_visible{true};
        mutable bool m_is_selected;
        Lumen::UI::Component::TransformRectangleArea m_transform_rectangle_area;
        TextLabelSizeTypeTag m_text_label_size_type{TextLabelSizeTypeTag::AUTO};
        std::optional<sf::Text> m_text_label;
        std::optional<Lumen::UI::Component::Sprite> m_background;
        void *m_do_action_data_ptr{nullptr};
        ActionHandler fn_action_hander;
        LabelLayout m_label_layout;

public:
        MenuButton(void) noexcept = default;

        constexpr MenuButton(const MenuButton &) noexcept = delete;
        constexpr MenuButton(MenuButton &&other) noexcept
        : m_is_visible{other.m_is_visible}, m_is_selected{other.m_is_selected},
          m_transform_rectangle_area{std::move(other.m_transform_rectangle_area)},
          m_text_label_size_type{other.m_text_label_size_type},
          m_text_label{std::move(other.m_text_label)}, m_background{std::move(other.m_background)},
          m_do_action_data_ptr{other.m_do_action_data_ptr},
          fn_action_hander{std::move(other.fn_action_hander)},
          m_label_layout{std::move(other.m_label_layout)}
        {
                other.m_do_action_data_ptr = nullptr;
        }

        constexpr MenuButton &operator=(const MenuButton &) noexcept = delete;
        constexpr MenuButton &operator=(MenuButton &&other) noexcept
        {
                if (this == &other) {
                        return *this;
                }

                this->m_is_visible = std::move(other.m_is_visible);
                this->m_is_selected = std::move(other.m_is_selected);
                this->m_transform_rectangle_area = std::move(other.m_transform_rectangle_area);
                this->m_text_label_size_type = other.m_text_label_size_type;
                this->m_text_label = std::move(other.m_text_label);
                this->m_background = std::move(other.m_background);
                this->m_do_action_data_ptr = other.m_do_action_data_ptr;
                this->fn_action_hander = std::move(other.fn_action_hander);
                this->m_label_layout = std::move(other.m_label_layout);
                other.m_do_action_data_ptr = nullptr;
                return *this;
        }

        [[nodiscard]] constexpr ActionResult DoSelectionAction(
                const Lumen::UI::Component::RelativeSelectionAction &relative_selection_action) noexcept
        {
                if (this->fn_action_hander) {
                        return this->fn_action_hander(this->m_do_action_data_ptr,
                                                      relative_selection_action);
                }
                return ActionResult::NoResult();
        }

        constexpr void SetVisible(bool is_visible = true) noexcept
        {
                this->m_is_visible = is_visible;
        }

        constexpr bool IsVisible(void) const noexcept
        {
                return this->m_is_visible;
        }

        constexpr bool IsSelected(void) const noexcept
        {
                return this->m_is_selected;
        }

        constexpr void SetSelected(bool is_selected) const noexcept
        {
                this->m_is_selected = is_selected;
        }

        constexpr void SetTransformRectangleArea(const Lumen::UI::Component::TransformRectangleArea &rectangle_area) noexcept
        {
                this->m_transform_rectangle_area = rectangle_area;
        }

        constexpr void SetButtonLabelPosition(const Lumen::UI::Component::TransformRectangleArea &rectangle_area,
                                              const Lumen::UI::Component::BoundingBox &bounding_box) noexcept
        {
                assert(this->m_text_label.has_value());
                auto &text = this->m_text_label.value();

                sf::Vector2f position{};

                switch (this->m_label_layout) {
                case LabelLayout::CENTER: {
                        const auto &button_top_left_position = rectangle_area.absolute_rectangle_area.top_left_position;
                        const auto &label_bounds = text.getLocalBounds();
                        const auto &button_size = bounding_box.size;
                        const auto current_label_half_size = Lumen::Core::Math::Vec2f32{
                                label_bounds.size.x * text.getScale().x,
                                label_bounds.size.y * text.getScale().y
                        } / 2.0f;
                        const auto current_label_position = Lumen::Core::Math::Vec2f32{
                                label_bounds.position.x * text.getScale().x,
                                label_bounds.position.y * text.getScale().y
                        };

                        position = {
                                button_top_left_position.x - current_label_position.x - current_label_half_size.x +
                                button_size.x / 2.0f,
                                button_top_left_position.y - current_label_position.y - current_label_half_size.y +
                                button_size.y / 2.0f,
                        };

                        break;
                }
                case LabelLayout::LEFT:
                        // TODO:
                        std::abort();
                        break;
                case LabelLayout::RIGHT:
                        //TODO:
                        std::abort();
                        break;
                case LabelLayout::TOP:     
                        // TODO:
                        std::abort();
                        break;
                case LabelLayout::BOTTOM:
                        // TODO:
                        std::abort();
                        break;
                }

                text.setPosition(position);
        }

        constexpr void SetActionHandler(ActionHandler &&action_handler, void *do_action_data_ptr) noexcept
        {
                this->fn_action_hander = std::move(action_handler);
                this->m_do_action_data_ptr = do_action_data_ptr;
        }

        constexpr const Lumen::UI::Component::TransformRectangleArea &GetTransformRectangleArea(void) const noexcept
        {
                return this->m_transform_rectangle_area;
        }

        //constexpr
        void SetText(const sf::Font *font, const std::string &text_string, const sf::Color color = sf::Color::White) noexcept
        {
                this->m_text_label = sf::Text{*font, text_string};
                this->m_text_label.value().setFillColor(color);
                this->m_text_label_size_type = TextLabelSizeTypeTag::AUTO;
        }

        constexpr TextLabelSizeTypeTag GetTextLabelSizeTypeTag(void) const noexcept
        {
                return this->m_text_label_size_type;
        }

        constexpr bool UseDefaultTextSize(void) const noexcept
        {
                return TextLabelSizeTypeTag::AUTO == this->m_text_label_size_type;
        }

        //constexpr 
        void SetDefaultTextSize(const Lumen::UI::Component::BoundingBox &bounding_box) noexcept
        {
                assert(this->m_text_label.has_value());
                assert(TextLabelSizeTypeTag::AUTO == this->m_text_label_size_type);                auto button_size = bounding_box.size;
                
                const auto &text_size = this->m_text_label.value().getLocalBounds().size;
                const float scale_x = button_size.x / text_size.x;
                const float scale_y = button_size.y / text_size.y;

                const float scale = std::min(scale_x, scale_y) * 0.9f;
                this->m_text_label.value().setScale({scale, scale});
        }

        //constexpr 
        void SetText(sf::Text &&text) noexcept
        {
                this->m_text_label = text;
                this->m_text_label_size_type = TextLabelSizeTypeTag::USER_DEFINED;
        }

        constexpr bool HasText(void) const noexcept
        {
                return this->m_text_label.has_value();
        }

        constexpr const sf::Text &GetText(void) const noexcept
        {
                assert(this->m_text_label.has_value());
                //std::cout << "MenuButton::SetText() called\n";
                //std::cout << "Text: " << this->m_text_label.getString().toAnsiString() << "\n";
                return this->m_text_label.value();
        }

        constexpr bool HasBackground(void) const noexcept
        {
                return this->m_background.has_value();
        }

        constexpr void SetBackground(const Lumen::UI::Component::Sprite &background) noexcept
        {
                this->m_background = background;
        }

        constexpr const Lumen::UI::Component::Sprite &GetBackground(void) const noexcept
        {
                assert(this->m_background.has_value());
                return this->m_background.value();
        }
};

class Menu : public Lumen::UI::Component::BasicUIComponent {
public:
        struct Layout {
                enum class DirectionTypeTag {
                        VERTICAL,
                        HORIZONTAL,
                };

                enum class SpacingTypeTag {
                        FIXED,
                        UNIFORM_DISTRIBUTION,
                };

                enum class AlignmentTypeTag {
                        TOP_LEFT,
                        TOP_CENTER,
                        TOP_RIGHT,
                        LEFT_CENTER,
                        CENTER,
                        RIGHT_CENTER,
                        BOTTOM_LEFT,
                        BOTTOM_CENTER,
                        BOTTOM_RIGHT,
                };

                DirectionTypeTag direction_type_tag;
                SpacingTypeTag spacing_type_tag;
                AlignmentTypeTag alignment_type_tag;

                struct Void {};
                struct FixedSpacingData {
                        float fixed_spacing_header;
                        float fixed_spacing;
                        float fixed_spacing_footer;
                };

                union {
                        Void no_data;
                        FixedSpacingData fixed_spacing_data;
                };

                Lumen::Core::Math::Vec2f32 alignment_offset;

                constexpr Layout(void) noexcept
                : direction_type_tag{DirectionTypeTag::VERTICAL},
                  spacing_type_tag{SpacingTypeTag::UNIFORM_DISTRIBUTION},
                  alignment_type_tag{AlignmentTypeTag::TOP_LEFT}, no_data{},
                  alignment_offset{0.0f, 0.0f} {}
        };

        using ButtonActionHandler = typename Lumen::UI::Component::MenuButton::ActionHandler;
        using SelectedSubMenu = Lumen::UI::Component::SelectedSubMenu;
private:

        Lumen::UI::Component::TransformRectangleArea m_transform_rectangle_area;
        Lumen::UI::Component::BoundingBox m_bounding_box;
        Lumen::UI::Component::BoundingBox m_bounding_box_button;
        Layout m_layout;
        std::optional<Lumen::UI::Component::Sprite> m_background;
        std::vector<Lumen::UI::Component::MenuButton> m_buttons;
        std::vector<std::unique_ptr<Lumen::UI::Component::Menu> > m_sub_menus;
        std::optional<SelectedSubMenu> m_selected_sub_menu;

public:
        Menu(void) noexcept
        : Lumen::UI::Component::BasicUIComponent{Lumen::UI::Component::UIComponentTypeTag::MENU} {}


        Menu(const Menu &) noexcept = delete;
        Menu(Menu &&other) noexcept = default;
        
        Menu &operator=(const Menu &) noexcept = delete;
        Menu &operator=(Menu &&other) noexcept = default;

        template <typename ...Args>
        constexpr void AddMenuButton(Args &&...args) noexcept
        {
                this->m_buttons.emplace_back(std::forward<Args>(args)...);
        }

        constexpr void AddMenuButton(Lumen::UI::Component::MenuButton &&menu_button) noexcept
        {
                this->m_buttons.push_back(std::move(menu_button));
        }

        constexpr Lumen::UI::Component::MenuButton &CreateMenuButton(void) noexcept
        {
                return this->m_buttons.emplace_back();
        }

        template<typename ...Args>
        constexpr void AddSubMenu(Args &&...args) noexcept
        {
                this->m_sub_menus.emplace_back(std::forward<Args>(args)...);
        }

        constexpr void AddSubMenu(std::unique_ptr<Lumen::UI::Component::Menu> &&sub_menu) noexcept
        {
                this->m_sub_menus.push_back(std::move(sub_menu));
        }

        constexpr Lumen::UI::Component::Menu &CreateSubMenu(void) noexcept
        {
                return *this->m_sub_menus.emplace_back(std::make_unique<Lumen::UI::Component::Menu>());
        }

        constexpr const Lumen::UI::Component::TransformRectangleArea &GetTransformRectangleArea(void) const noexcept
        {
                return this->m_transform_rectangle_area;
        }

        constexpr const Lumen::Core::Math::Vec2f32 &GetMenuRelativeTopLeftPosition(void) const noexcept
        {
                return this->m_transform_rectangle_area.relative_rectangle_area.top_left_position;
        }

        constexpr const Lumen::Core::Math::Vec2f32 &GetMenuRelativeBottomRightPosition(void) const noexcept
        {
                return this->m_transform_rectangle_area.relative_rectangle_area.bottom_right_position;
        }

        constexpr const Lumen::Core::Math::Vec2f32 &GetMenuAbsoluteTopLeftPosition(void) const noexcept
        {
                return this->m_transform_rectangle_area.absolute_rectangle_area.top_left_position;
        }

        constexpr const std::vector<Lumen::UI::Component::MenuButton> &GetMenuButtons(void) const noexcept
        {
                return this->m_buttons;
        }

        constexpr const Lumen::Core::Math::Vec2f32 &GetMenuAbsoluteBottomRightPosition(void) const noexcept
        {
                return this->m_transform_rectangle_area.absolute_rectangle_area.bottom_right_position;
        }

        constexpr void SetMenuRelativeTopLeftPosition(const Lumen::Core::Math::Vec2f32 &top_left_position) noexcept
        {
                this->m_transform_rectangle_area.relative_rectangle_area.top_left_position = top_left_position;
        }

        constexpr const Lumen::UI::Component::BoundingBox &GetMenuButtonBoundingBox(void) const noexcept
        {
                return this->m_bounding_box_button;
        }

        constexpr const Lumen::UI::Component::BoundingBox &GetMenuBoundingBox(void) const noexcept
        {
                return this->m_bounding_box;
        }

        constexpr void SetMenuButtonBoundingBox(const Lumen::UI::Component::BoundingBox &bounding_box) noexcept
        {
                this->m_bounding_box_button = bounding_box;
        }

        constexpr void SetMenuBoundingBox(const Lumen::UI::Component::BoundingBox &bounding_box) noexcept
        {
                this->m_bounding_box = bounding_box;
        }

        constexpr bool HasBackground(void) const noexcept
        {
                return this->m_background.has_value();
        }

        constexpr const Lumen::UI::Component::Sprite &GetBackground(void) const noexcept
        {
                assert(this->m_background.has_value());
                return this->m_background.value();
        }

        constexpr void SetBackground(const Lumen::UI::Component::Sprite &background) noexcept
        {
                this->m_background = background;
        }

        constexpr const Layout &GetLayout(void) const noexcept
        {
                return this->m_layout;
        }

        constexpr void SetLayout(const Layout &layout) noexcept
        {
                this->m_layout = layout;
        }

        constexpr void ComputeBoundingBoxAndTransform(
                const Lumen::UI::Component::AbsoluteTransformTopLeft &parent_absolute_position) noexcept
        {
                switch (this->m_layout.spacing_type_tag) {
                case Layout::SpacingTypeTag::FIXED:
                        switch (this->m_layout.direction_type_tag) {
                        case Layout::DirectionTypeTag::VERTICAL:
                                this->ComputeFixedVerticalMenuBoundingBoxAndTransform(parent_absolute_position);
                                break;
                        case Layout::DirectionTypeTag::HORIZONTAL:
                                assert(false);
                                //this->ComputeHorizontalMenuBoundingBox();
                                break;
                        }
                        break;

                case Layout::SpacingTypeTag::UNIFORM_DISTRIBUTION:
                        switch (this->m_layout.direction_type_tag) {
                        case Layout::DirectionTypeTag::VERTICAL:
                                this->ComputeUniformDistributionVerticalMenuBoundingBoxAndTransform(
                                        parent_absolute_position);
                                break;
                        case Layout::DirectionTypeTag::HORIZONTAL:
                                assert(false);
                                //this->ComputeHorizontalMenuBoundingBox();
                                break;
                        }
                }

                this->m_transform_rectangle_area.absolute_rectangle_area.top_left_position =
                        parent_absolute_position.position +
                        this->m_transform_rectangle_area.relative_rectangle_area.top_left_position;

                this->m_transform_rectangle_area.absolute_rectangle_area.bottom_right_position =
                        parent_absolute_position.position +
                        this->m_transform_rectangle_area.relative_rectangle_area.bottom_right_position;
        }


        //constexpr void UpdatePosition(....) {}

private:

        constexpr void ComputeFixedVerticalMenuBoundingBoxAndTransform(
                const Lumen::UI::Component::AbsoluteTransformTopLeft &parent_absolute_position) noexcept
        {
                this->ComputeFixedVerticalMenuBoundingBox();

                this->ComputeFixedVerticalMenuTransformRectangleArea(parent_absolute_position);
                

                this->ComputeFixedVerticalMenuButtonTransform();
        }

        constexpr void ComputeFixedVerticalMenuBoundingBox(void) noexcept
        {
                const auto &fixed_spacing = this->m_layout.fixed_spacing_data;
                const auto &menu_button_size = this->m_bounding_box_button.size;

                auto &menu_size = this->m_bounding_box.size;
                menu_size.x = menu_button_size.x;
                menu_size.y = fixed_spacing.fixed_spacing_header + fixed_spacing.fixed_spacing_footer +
                              (menu_button_size.y * static_cast<float>(this->m_buttons.size())) +
                              (fixed_spacing.fixed_spacing * static_cast<float>(this->m_buttons.size() - 1));
        }

        constexpr void ComputeFixedVerticalMenuTransformRectangleArea(
                const Lumen::UI::Component::AbsoluteTransformTopLeft &parent_absolute_position) noexcept
        {
                const auto &menu_size = this->m_bounding_box.size;
                auto &relative_rectangle_area = this->m_transform_rectangle_area.relative_rectangle_area;
                const auto &relative_top_left_position = relative_rectangle_area.top_left_position;
                relative_rectangle_area.bottom_right_position = relative_top_left_position + menu_size;

                this->m_transform_rectangle_area.absolute_rectangle_area.top_left_position =
                        parent_absolute_position.position + relative_top_left_position;
                        this->m_transform_rectangle_area.absolute_rectangle_area.bottom_right_position =
                        parent_absolute_position.position + relative_rectangle_area.bottom_right_position;
        }

        constexpr void ComputeFixedVerticalMenuButtonTransform(void) noexcept
        {
                const auto &fixed_spacing = this->m_layout.fixed_spacing_data;
                const auto &button_size = this->m_bounding_box_button.size;
                const auto spacing = fixed_spacing.fixed_spacing + button_size.y;

                for (auto iterator = this->m_buttons.begin(); iterator != this->m_buttons.end(); ++iterator) {
                        const auto index = std::distance(this->m_buttons.begin(), iterator);

                        const auto button_top_left_position_y =
                                fixed_spacing.fixed_spacing_header +
                                (static_cast<float>(index) * spacing);

                        this->ComputeFixedVerticalMenuButtonTransformRectangleArea(
                                *iterator,
                                {this->m_transform_rectangle_area.absolute_rectangle_area.top_left_position},
                                button_top_left_position_y);

                        const auto &button_transform_rectangle_area = iterator->GetTransformRectangleArea();


                        if (iterator->HasText()) {
                                if (iterator->UseDefaultTextSize()) {
                                        iterator->SetDefaultTextSize(this->m_bounding_box_button);
                                }
                                iterator->SetButtonLabelPosition(button_transform_rectangle_area,
                                                                 this->m_bounding_box_button);
                        }
                }
        }

        constexpr void ComputeFixedVerticalMenuButtonTransformRectangleArea(
                Lumen::UI::Component::MenuButton &button,
                const Lumen::UI::Component::AbsoluteTransformTopLeft &parent_absolute_position,
                const float button_top_left_position_y) const noexcept
        {
                const auto &button_size = this->m_bounding_box_button.size;

                const Lumen::UI::Component::RelativeTransformTopLeft button_top_left_position{{
                        0.0f, button_top_left_position_y
                }};
                const Lumen::UI::Component::RelativeTransformBottomRight button_bottom_right_position{
                        button_top_left_position.position + button_size
                };
                const Lumen::UI::Component::RelativeTransformRectangleArea button_relative_rectangle_area{
                        button_top_left_position, button_bottom_right_position
                };

                const Lumen::UI::Component::TransformRectangleArea button_transform_rectangle_area{
                        button_relative_rectangle_area, parent_absolute_position.position
                };

                button.SetTransformRectangleArea(button_transform_rectangle_area);
        }

        constexpr void ComputeUniformDistributionVerticalMenuBoundingBoxAndTransform(
                const Lumen::UI::Component::AbsoluteTransformTopLeft &parent_absolute_position) noexcept
        {

                this->ComputeUniformDistributionVerticalMenuTransformRectangleArea(parent_absolute_position);

                this->ComputeUniformDistributionVerticalMenuButtonTransform(
                        {this->m_transform_rectangle_area.absolute_rectangle_area.top_left_position});

        }

        constexpr void ComputeUniformDistributionVerticalMenuTransformRectangleArea(
                const Lumen::UI::Component::AbsoluteTransformTopLeft &parent_absolute_position) noexcept
        {
                auto &relative_rectangle_area = this->m_transform_rectangle_area.relative_rectangle_area;
                const auto &menu_size = this->m_bounding_box.size;
                const auto &relative_top_left_position = relative_rectangle_area.top_left_position;
                relative_rectangle_area.bottom_right_position = relative_top_left_position + menu_size;

                this->m_transform_rectangle_area.absolute_rectangle_area = {
                        relative_rectangle_area, parent_absolute_position.position
                };
        }

        constexpr void ComputeUniformDistributionVerticalMenuButtonTransform(
                const Lumen::UI::Component::AbsoluteTransformTopLeft &parent_absolute_position) noexcept
        {
                const auto &menu_size = this->m_bounding_box.size;
                const auto &button_size = this->m_bounding_box_button.size;

                const auto used_space_y = static_cast<float>(this->m_buttons.size()) * button_size.y;
                const auto free_space_y = menu_size.y - used_space_y;
                assert(free_space_y >= 0);
                const auto uniform_space_y = free_space_y / static_cast<float>(this->m_buttons.size() + 1);

                std::size_t index = 0;
                for (auto &menu_button : this->m_buttons) {
                        const auto offset_y = (static_cast<float>(index + 1) * uniform_space_y) +
                                              (static_cast<float>(index) * this->m_bounding_box_button.size.y);
                        
                        Lumen::UI::Component::RelativeTransformTopLeft button_top_left_position{{0.0f, offset_y}};
                        Lumen::UI::Component::RelativeTransformRectangleArea button_relative_rectangle_area{
                                button_top_left_position,
                                this->m_bounding_box_button,
                        };
                        Lumen::UI::Component::TransformRectangleArea button_transform_rectangle_area{
                                button_relative_rectangle_area, parent_absolute_position.position
                        };
                        
                        menu_button.SetTransformRectangleArea(button_transform_rectangle_area);

                        if (menu_button.HasText()) {
                                if (menu_button.UseDefaultTextSize()) {
                                        menu_button.SetDefaultTextSize(this->m_bounding_box_button);
                                }
                                menu_button.SetButtonLabelPosition(button_transform_rectangle_area,
                                                                   this->m_bounding_box_button);
                        }

                        ++index;
                }
        }

public:
        constexpr bool HasSelectedSubMenu(void) const noexcept
        {
                return this->m_selected_sub_menu.has_value();
        }

        constexpr Lumen::UI::Component::Menu &GetSelectedSubMenu(void) noexcept
        {
                assert(this->m_selected_sub_menu.has_value());
                return *this->m_selected_sub_menu.value().sub_menu_ptr;
        }

        constexpr const Lumen::UI::Component::Menu &GetSelectedSubMenu(void) const noexcept
        {
                assert(this->m_selected_sub_menu.has_value());
                return *this->m_selected_sub_menu.value().sub_menu_ptr;
        }

        constexpr void Update(float delta_time) noexcept override
        {
                if (!this->HasSelectedSubMenu()) {
                        return;
                }

                auto &selected_sub_menu = this->m_selected_sub_menu.value();

                if (selected_sub_menu.remain_time_in_second <= 0.0f) {

                        this->UnSelectSubMenu();
                        return;
                } else {
                        selected_sub_menu.remain_time_in_second -= delta_time;
                }
                auto &sub_menu = this->GetSelectedSubMenu();
                sub_menu.Update(delta_time);

        }

private:

        constexpr void UnSelectSubMenu(void) noexcept
        {
                assert(this->HasSelectedSubMenu());
                assert(this->m_selected_sub_menu.has_value());
                assert(this->m_selected_sub_menu.value().remain_time_in_second <= 0.0f);

                this->m_selected_sub_menu = std::nullopt;
                this->SetSelected(false);
        }

private:
        constexpr bool IsMenuSelected(
                const Lumen::UI::Component::RelativeSelectionAction &relative_selection_action) const noexcept
        {
                return Lumen::UI::Component::IsIncluded(
                        Lumen::UI::Component::AbsoluteSelectedPosition{{
                                static_cast<float>(relative_selection_action.selection_action.position.x),
                                static_cast<float>(relative_selection_action.selection_action.position.y),
                        }},
                        this->m_transform_rectangle_area.absolute_rectangle_area
                );
        }

        static constexpr bool IsMenuButtonSelected(
                const Lumen::UI::Component::RelativeSelectionAction &relative_selection_action,
                const Lumen::UI::Component::MenuButton &menu_button) noexcept
        {
                return Lumen::UI::Component::IsIncluded(
                        Lumen::UI::Component::AbsoluteSelectedPosition{{
                                static_cast<float>(relative_selection_action.selection_action.position.x),
                                static_cast<float>(relative_selection_action.selection_action.position.y),
                        }},
                        Lumen::UI::Component::AbsoluteTransformRectangleArea{
                                menu_button.GetTransformRectangleArea().absolute_rectangle_area
                        }
                );
        }

        constexpr void SubMenuIsSelected(void) noexcept
        {
                assert(this->m_selected_sub_menu.has_value());
                this->m_selected_sub_menu.value().ResetRemainTime();
        }

public:

        constexpr Lumen::Core::Math::Vec2f32 ComputeSubMenuRelativePosition(
                const Lumen::UI::Component::MenuButton &menu_button
                // const Lumen::UI::Component::Menu &sub_menu
        ) const noexcept
        {
                const auto &button_right_position = 
                        menu_button.GetTransformRectangleArea().relative_rectangle_area.bottom_right_position;
                return { button_right_position.x, 0 };
        }

        constexpr DoActionResult DoSelectionAction(
                const Lumen::UI::Component::RelativeSelectionAction &relative_selection_action_to_parent) noexcept override
        {
                if (this->HasSelectedSubMenu()) {
                        assert(this->m_selected_sub_menu.has_value());
                        auto &selected_sub_menu = this->m_selected_sub_menu.value();
                        assert(nullptr != selected_sub_menu.sub_menu_ptr);
                        auto &sub_menu = *selected_sub_menu.sub_menu_ptr;
        
                        if (this->GetSelectedSubMenu().IsMenuSelected(relative_selection_action_to_parent)) {
                                sub_menu.SetSelected(true);
                                selected_sub_menu.ResetRemainTime();
                                return this->GetSelectedSubMenu().DoSelectionAction(relative_selection_action_to_parent);
                        } else {
                                sub_menu.SetSelected(false);
                        }
                }

                if (!this->IsMenuSelected(relative_selection_action_to_parent)) {
                        return DoActionResult::NotHandledOrNotBlocked;
                }

                for (auto &menu_button : this->m_buttons) {

                        if (!this->IsMenuButtonSelected(relative_selection_action_to_parent, menu_button)) {
                                continue;
                        }

                        menu_button.SetSelected(true);

                        auto action_result = menu_button.DoSelectionAction(relative_selection_action_to_parent);
                        if (!action_result.is_handled) {
                                continue;
                        }
                        if (!action_result.is_blocked) {
                                continue;
                        }
                        this->m_selected_sub_menu = action_result.selected_sub_menu;
                        if(this->m_selected_sub_menu.has_value()) {
                                auto &sub_menu = *this->m_selected_sub_menu.value().sub_menu_ptr;
                                // TODO: Set sub menu position
                                sub_menu.SetMenuRelativeTopLeftPosition(
                                        this->ComputeSubMenuRelativePosition(menu_button)
                                );
                                auto sub_menu_position = menu_button.GetTransformRectangleArea().absolute_rectangle_area.top_left_position;
                                sub_menu.ComputeBoundingBoxAndTransform({sub_menu_position});
                        }
                        return DoActionResult::HandledOrBlocked;
                }

                return DoActionResult::HandledOrBlocked;
        }

};

} // namespace Component
} // namespace UI        
} // namespace Lumen
