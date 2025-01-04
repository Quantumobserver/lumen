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
        // enum class SpawnSide {
        //         LEFT_SIDE,
        //         RIGHT_SIDE,
        // };
        // enum class SpawnAlignment {
        //         TOP,
        //         BOTTOM,
        // };
        SelectionTypeTag selection_type;
        // SpawnSide spawn_side;
        // SpawnAlignment spawn_alignment;
        Lumen::Core::Memory::ReadWritePtr<Lumen::UI::Component::Menu> sub_menu_ptr;
        const Lumen::UI::Component::MenuButton *menu_button_ptr{nullptr};

        // Menu class will set this value
        float remain_time_in_second;
        Lumen::UI::Component::TransformRectangleArea transform_rectangle_area;

        constexpr SelectedSubMenu(void) noexcept
        : selection_type{SelectionTypeTag::NONE}, //spawn_side{SpawnSide::LEFT_SIDE}, spawn_alignment{SpawnAlignment::TOP}
          remain_time_in_second{SelectedSubMenu::GetRemainDurationInSecond()}
          {}

        constexpr SelectedSubMenu(SelectionTypeTag selection_type, //SpawnSide spawn_side,
                                  //SpawnAlignment spawn_alignment,
                                  Lumen::Core::Memory::ReadWritePtr<Lumen::UI::Component::Menu> sub_menu_ptr,
                                  const Lumen::UI::Component::MenuButton *menu_button_ptr) noexcept
        : selection_type{selection_type}, //spawn_side{spawn_side}, spawn_alignment{spawn_alignment},
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
        //std::string m_text;
        TextLabelSizeTypeTag m_text_label_size_type{TextLabelSizeTypeTag::AUTO};
        std::optional<sf::Text> m_text_label;
        std::optional<Lumen::UI::Component::Sprite> m_background;
        void *m_do_action_data_ptr{nullptr};
        ActionHandler fn_action_hander;
        LabelLayout m_label_layout;

        sf::Color m_outline_color{sf::Color::Cyan};

public:
        constexpr MenuButton(void) noexcept = default;

        // constexpr MenuButton(sf::Text &&text, const Lumen::UI::Component::Sprite &background,
        //                      void *do_menu_button_action_data_ptr = nullptr,
        //                      ButtonActionHandler &&do_menu_button_action = nullptr,
        //                      LabelLayout menue_button_layout = LabelLayout::CENTER) noexcept
        // : m_text_label{std::move(text)}, m_background{background},
        //   m_do_action_data_ptr{do_menu_button_action_data_ptr},
        //   fn_action_hander{std::move(do_menu_button_action)},
        //   m_label_layout{menue_button_layout} {}

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

        // constexpr void SetPosition(const Lumen::UI::Component::TransformTopLeft &transform_top_left,
        //                            const Lumen::UI::Component::BoundingBox &bounding_box) noexcept
        // {
        //         this->m_transform_rectangle_area = {transform_top_left, bounding_box};
        // }

        // constexpr void SetButtonLabelPosition(const Lumen::UI::Component::TransformTopLeft &transform_top_left) noexcept
        // {
        //         this->m_text_label.setPosition({static_cast<float>(transform_top_left.top_left_position.x),
        //                                         static_cast<float>(transform_top_left.top_left_position.y)});
        // }

        // constexpr void MoveRelativePosition(const Lumen::Core::Math::Vec2i &offset) noexcept
        // {
        //         this->m_transform_rectangle_area.MovePosition(offset);
        //         std::cout << "MenuButton::MovePosition() called\n";
        //         this->m_text_label.move({static_cast<float>(offset.x), static_cast<float>(offset.y)});
        //         this->m_background.move({static_cast<float>(offset.x), static_cast<float>(offset.y)});
        // }

        constexpr const Lumen::UI::Component::TransformRectangleArea &GetTransformRectangleArea(void) const noexcept
        {
                return this->m_transform_rectangle_area;
        }

        // constexpr void SetColor(const sf::Color &color) noexcept
        // {
        //         this->m_outline_color = color;
        // }

        // constexpr const sf::Color &GetColor(void) const noexcept
        // {
        //         return this->m_outline_color;
        // }

        //constexpr
        void SetText(const sf::Font *font, const std::string &text_string) noexcept
        {
                this->m_text_label = sf::Text{*font, text_string};
                this->m_text_label_size_type = TextLabelSizeTypeTag::AUTO;
        }

        constexpr TextLabelSizeTypeTag TextSizeIsSet(void) const noexcept
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
                assert(TextLabelSizeTypeTag::AUTO == this->m_text_label_size_type);
// std::cout << "this->m_text_label_size_is_set: " << this->m_text_label_size_is_set << "\n";
                auto button_size = bounding_box.size;
                
                const auto &text_size = this->m_text_label.value().getLocalBounds().size;
                const float scale_x = button_size.x / text_size.x;
                const float scale_y = button_size.y / text_size.y;

                const float scale = std::min(scale_x, scale_y) * 0.9f;
                this->m_text_label.value().setScale({scale, scale});
// std::cout << "Text: " << text_string << "\n";
// std::cout << "Text Size: " << m_text_label.value().getLocalBounds().size.x << ", " << m_text_label.value().getLocalBounds().size.y << "\n";
// std::cout << "scale: " << scale << "\n";
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

        // constexpr LabelLayout GetMenuButtonLabelLayout(void) const noexcept
        // {
        //         return this->m_label_layout;
        // }

        // constexpr void SetMenuButtonLabelLayout(LabelLayout menu_button_label_layout) noexcept
        // {
        //         this->m_label_layout = menu_button_label_layout;
        // }

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

        // constexpr void SetMenuButtonBackgroundPosition(const Lumen::Core::Math::Vec2f32 &transform_top_left) noexcept
        // {
        //         this->m_background.setPosition({static_cast<float>(transform_top_left.top_left_position.x),
        //                                         static_cast<float>(transform_top_left.top_left_position.y)});
        // }

        // constexpr void SetMenuButtonBackgroundPosition(const Lumen::UI::Component::TransformRectangleArea &rectangle_area) noexcept
        // {
        //         this->m_background.setPosition({static_cast<float>(rectangle_area.top_left_position.top_left_position.x),
        //                                         static_cast<float>(rectangle_area.top_left_position.top_left_position.y)});
        // }

        // constexpr void SetMenuButtonBackgroundScale(const Lumen::Core::Math::Vec2f32 &scale) noexcept
        // {
        //         this->m_background.setScale({scale.x, scale.y});
        // }

        // constexpr void MoveMenuButtonBackground(const Lumen::Core::Math::Vec2f32 &offset) noexcept
        // {
        //         this->m_background.move({offset.x, offset.y});
        // }

        // constexpr void MoveMenuButtonTextlabel(const Lumen::Core::Math::Vec2f32 &offset) noexcept
        // {
        //         this->m_text_label.move({offset.x, offset.y});
        // }
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
// std::cout << __FILE__ " :" << __LINE__ << "\n";
                        switch (this->m_layout.direction_type_tag) {
                        case Layout::DirectionTypeTag::VERTICAL:
                                //assert(false);
// std::cout << __FILE__ " :" << __LINE__ << "\n";
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

//                         if (iterator->HasBackground()) {
// std::cout << "MenuButton: " << __LINE__ << "\n";
//                                 iterator->SetDefaultSize(this->m_bounding_box_button);
//                                 // iterator->SetMenuButtonBackgroundPosition(iterator->GetTransformRectangleArea().absolute_rectangle_area.top_left_position);
//                         }
                        // iterator->SetMenuButtonBackgroundPosition(iterator->GetTransformRectangleArea().absolute_rectangle_area.top_left_position);
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
// std::cout << "[+++++++++++++++++++++++++++++++++++]\n";
// std::cout << button_transform_rectangle_area.absolute_rectangle_area.top_left_position.x << ", "
//           << button_transform_rectangle_area.absolute_rectangle_area.top_left_position.y << "\n";
// std::cout << button_transform_rectangle_area.absolute_rectangle_area.bottom_right_position.x << ", "
//           << button_transform_rectangle_area.absolute_rectangle_area.bottom_right_position.y << "\n";
// std::cout << "button_top_left_position_y: " << button_top_left_position_y << "\n";
// std::cout << "button_size.y: " << button_size.y << "\n";
// std::cout << parent_absolute_position.position.x << ", " << parent_absolute_position.position.y << "\n";
// std::cout << "[------------------------------------------]\n";
                button.SetTransformRectangleArea(button_transform_rectangle_area);
        }

        constexpr void ComputeUniformDistributionVerticalMenuBoundingBoxAndTransform(
                const Lumen::UI::Component::AbsoluteTransformTopLeft &parent_absolute_position) noexcept
        {
// std::cout << __FILE__ " :" << __LINE__ << "\n";
                this->ComputeUniformDistributionVerticalMenuTransformRectangleArea(parent_absolute_position);
// std::cout << __FILE__ " :" << __LINE__ << "\n";
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
// std::cout << __FILE__ " :" << __LINE__ << "\n";
                auto &selected_sub_menu = this->m_selected_sub_menu.value();
{
std::cout << __FILE__ " :" << __LINE__ << "\n";
 std::cout << "delta_time=" << delta_time << "\n";
        std::cout << "selected_sub_menu.remain_time_in_second=" << selected_sub_menu.remain_time_in_second << "\n";
const auto &sub_menu = this->GetSelectedSubMenu();
const auto &rec = sub_menu.GetTransformRectangleArea();
std::cout << "rec.abs_top_left.x=" << rec.absolute_rectangle_area.top_left_position.x << "\n";
std::cout << "rec.abs_top_left.y=" << rec.absolute_rectangle_area.top_left_position.y << "\n";
std::cout << "rec.abs_bottom_right.x=" << rec.absolute_rectangle_area.bottom_right_position.x << "\n";
std::cout << "rec.abs_bottom_right.y=" << rec.absolute_rectangle_area.bottom_right_position.y << "\n";
}

                if (selected_sub_menu.remain_time_in_second <= 0.0f) {
// std::cout << __FILE__ " :" << __LINE__ << "\n";
                        this->UnSelectSubMenu();
                        return;
                } else {
                        selected_sub_menu.remain_time_in_second -= delta_time;
                }
                auto &sub_menu = this->GetSelectedSubMenu();
                sub_menu.Update(delta_time);

//                 if (!this->m_selected_sub_menu.HasSelectedSubMenu() || 
//                     this->m_selected_sub_menu.is_selected_in_last_update) {
//                         return;
//                 }
// // std::cout << "Update: " << __LINE__ << ": delta_time=" << delta_time
// //           << ", this->m_selected_sub_menu.remain_time_in_second="
// //           << this->m_selected_sub_menu.remain_time_in_second << "\n";
//                 this->m_selected_sub_menu.remain_time_in_second -= delta_time;
//                 if (this->m_selected_sub_menu.remain_time_in_second <= 0.0f) {
//                         this->UnSelectSubMenu();
//                 }
        }

private:

// std::cout << "Update: " << __LINE__ << ": delta_time=" << delta_time
//           << ", this->m_selected_sub_menu.remain_time_in_second="
//           << this->m_selected_sub_menu.remain_time_in_second << "\n";
//                 this->m_selected_sub_menu.remain_time_in_second -= delta_time;
//                 if (this->m_selected_sub_menu.remain_time_in_second <= 0.0f) {
//                         this->UnSelectSubMenu();
//                 }
//         }
// std::cout << "Update: " << __LINE__ << ": delta_time=" << delta_time
//           << ", this->m_selected_sub_menu.remain_time_in_second="
//           << this->m_selected_sub_menu.remain_time_in_second << "\n";
//                 this->m_selected_sub_menu.remain_time_in_second -= delta_time;
//                 if (this->m_selected_sub_menu.remain_time_in_second <= 0.0f) {
//                         this->UnSelectSubMenu();
//                 }
//         }

        // constexpr void Render(void) noexcept override
        // {
        //         this->DrawMenuBoundingBox();
        //         this->DrawMenuButtonBoundingBoxForEach();
        //         this->DrawSelectedSubMenu();
        //         this->DrawMenuButton();
        // }

        constexpr void UnSelectSubMenu(void) noexcept
        {
                assert(this->HasSelectedSubMenu());
                assert(this->m_selected_sub_menu.has_value());
                assert(this->m_selected_sub_menu.value().remain_time_in_second <= 0.0f);

                this->m_selected_sub_menu = std::nullopt;
                this->SetSelected(false);

                // //std::cout << "UnSelectSubMenu: " << __LINE__ << "\n";
                // const auto sub_menu_color_unselected = this->m_selected_sub_menu.sub_menu_ptr->GetColorWhenNotSelected();
                // this->m_selected_sub_menu.sub_menu_ptr->SetColor(sub_menu_color_unselected);
                // this->m_selected_sub_menu.Clear();
        }

        // constexpr void DrawMenuBoundingBox(void) noexcept 
        // {
        //         constexpr const float OUTLINE_THICKNESS_MENU = 3.0f;
        //         this->m_rectangle_shape.setOutlineThickness(OUTLINE_THICKNESS_MENU);

        //         this->m_rectangle_shape.setPosition({
        //                 static_cast<float>(this->m_transform_rectangle_area.top_left_position.top_left_position.x),
        //                 static_cast<float>(this->m_transform_rectangle_area.top_left_position.top_left_position.y)});

        //         this->m_rectangle_shape.setSize({
        //                 static_cast<float>(this->m_bounding_box_menu.size.x) - (OUTLINE_THICKNESS_MENU * 2.0f),
        //                 static_cast<float>(this->m_bounding_box_menu.size.y) - (OUTLINE_THICKNESS_MENU * 2.0f)
        //         });
        //         this->m_rectangle_shape.setOutlineColor(this->m_outline_color_menu);
        //         this->m_rectangle_shape.setFillColor(sf::Color::Transparent);
        //         //this->m_rectangle_shape.setOrigin(this->m_rectangle_shape.getSize() / 2.0f);
        //         this->m_window_ptr->draw(m_rectangle_shape);
        // }

        // constexpr void  DrawMenuButtonBoundingBoxForEach(void) noexcept
        // {
        //         //for (auto iterator = this->m_buttons.begin(); iterator != this->m_buttons.end(); ++iterator) {
        //         //        const auto index = std::distance(this->m_buttons.begin(), iterator);
        //         for (const auto &menu_button : this->m_buttons) {

        //                 //this->DrawMenuButtonBoundingBox(index, *iterator);
        //                 this->DrawMenuButtonBoundingBox(menu_button);
        //         }
        // }

        // constexpr void DrawMenuButton(void) const noexcept
        // {
        //         //std::cout << "size: " << this->m_buttons.size() << "\n";
        //         for (const auto &menu_button : this->m_buttons) {
        //                 //std::cout << "DrawMenuButton: " << __LINE__ << "\n";
        //                 this->DrawMenuButtonBackground(menu_button);
        //                 this->DrawMenuButtonTextLabel(menu_button);
        //         }
        // }

        // constexpr void DrawMenuButtonTextLabel(const MenuButton &menu_button) const noexcept
        // {

        //         //std::cout << "DrawMenuButtonTextLabel: " << __LINE__ << "\n";
        //         if (&menu_button.GetText() == nullptr)
        //         {
        //                 return;
        //         }
        //         this->m_window_ptr->draw(menu_button.GetText());
        // }

        // constexpr void DrawMenuButtonBoundingBox(
        //         //const typename std::iterator_traits<decltype(m_buttons)::iterator>::difference_type index,
        //         const Lumen::UI::Component::MenuButton &menu_button) noexcept
        // {
        //         constexpr const float outline_thickness_menu_button = 1.0f;
        //         m_rectangle_shape.setOutlineThickness(outline_thickness_menu_button);
        //         //const auto menu_button_ralative_center_position = this->GetMenuButtonRelativeCenterPosition(index);
        //         //const auto menu_button_position = this->m_transform_top_left.top_left_position + menu_button_ralative_center_position.center_position;
        //         const auto menu_button_transform_rectangle_area = menu_button.GetTransformRectangleArea();
        //         const auto menu_button_relative_top_left_position = menu_button_transform_rectangle_area.top_left_position;
        //         const auto menu_button_top_left_position =
        //                 this->m_transform_rectangle_area.top_left_position.top_left_position +
        //                 menu_button_relative_top_left_position.top_left_position;

        //         m_rectangle_shape.setPosition({
        //                 static_cast<float>(menu_button_top_left_position.x),
        //                 static_cast<float>(menu_button_top_left_position.y)});
        //         m_rectangle_shape.setSize({
        //                 static_cast<float>(this->m_bounding_box_menu_button.size.x) - (outline_thickness_menu_button * 2.0f),
        //                 static_cast<float>(this->m_bounding_box_menu_button.size.y) - (outline_thickness_menu_button * 2.0f)
        //         });
        //         m_rectangle_shape.setOutlineColor(menu_button.GetColor());
        //         m_rectangle_shape.setFillColor(sf::Color::Transparent);
        //         //m_rectangle_shape.setOrigin(m_rectangle_shape.getSize() / 2.0f);
        //         this->m_window_ptr->draw(m_rectangle_shape);
        // }

        // constexpr void DrawMenuButtonBackground(const MenuButton &menu_button) const noexcept
        // {
        //         if (&menu_button.GetMenuButtonBackground() == nullptr)
        //         {
        //                 return;
        //         }
        //         this->m_window_ptr->draw(menu_button.GetMenuButtonBackground());
        // }

        // constexpr void DrawSelectedSubMenu(void) noexcept
        // {
        //         if (Lumen::UI::Component::SelectedSubMenu::SelectionTypeTag::NONE != this->m_selected_sub_menu.selection_type) {
        //                 this->m_selected_sub_menu.sub_menu_ptr->Render();
        //         }
        // }

        // constexpr void DoWindowResizeAction(const Lumen::Core::Math::Vec2i &window_new_size) noexcept override
        // {
        //         // TODO
        //         (void)window_new_size;
        // }

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

        // constexpr Lumen::UI::Component::TransformTopLeft GetMenuButtonTopLeftPosition(
        //         const typename std::iterator_traits<decltype(m_buttons)::iterator>::difference_type index) const noexcept
        // {
        //         /*auto calculate_button_relative_position_vertical_uniform_distribution =
        //                 [&](void) -> Lumen::UI::Component::Transform {
        //                 const auto menu_top_position_y = this->m_transform.position.y - this->m_bounding_box_menu.half_size.y;
        //                 const auto menu_button_at_index_zero_position_y = menu_top_position_y + this->m_bounding_box_menu_button.half_size.y;
        //                 return {{(this->m_transform.position.x),
        //                         (menu_button_at_index_zero_position_y +
        //                          (this->m_bounding_box_menu_button.size.y * static_cast<int>(index)))}};
        //         };*/
        //         auto calculate_button_relative_center_position_vertical_uniform_distribution =
        //                 [this, index](void) -> Lumen::UI::Component::TransformTopLeft {
        //                 const auto used_space_y = static_cast<int>(this->m_buttons.size()) *
        //                                                            this->m_bounding_box_menu_button.size.y;
        //                 const auto free_space_y = this->m_bounding_box_menu.size.y - used_space_y;
        //                 assert(free_space_y >= 0);
        //                 const auto uniform_space = free_space_y / static_cast<int>(this->m_buttons.size() + 1);
        //                 const auto offset_y = (static_cast<int>(index + 1) * uniform_space) +
        //                                       (static_cast<int>(index) * this->m_bounding_box_menu_button.size.y);
        //                 return {{0, offset_y,}};
        //         };

        //         auto calculate_button_relative_center_position_vertical_fixed_spacing = 
        //                 [this, index](void) -> Lumen::UI::Component::TransformTopLeft {
        //                 const auto &menu_layout = this->m_menu_layout;
        //                 const auto menu_layout_data = menu_layout.fixed_spacing_data;
        //                 const auto fixed_spacing_header = menu_layout_data.fixed_spacing_header;
        //                 const auto fixed_spacing = menu_layout_data.fixed_spacing;

        //                 const auto offset_y = fixed_spacing_header +
        //                                       ((this->m_bounding_box_menu_button.size.y + fixed_spacing) *
        //                                        static_cast<int>(index));
        //                 return {{0, offset_y,}};
        //         };

        //         switch (this->m_menu_layout.menu_layout_type) {
        //         case MenuLayoutTypeTag::VERTICAL_UNIFORM_DISTRIBUTION:{
        //                 return calculate_button_relative_center_position_vertical_uniform_distribution();
        //                 break;
        //         }

        //         case MenuLayoutTypeTag::VERTICAL_FIXED_SPACING_AUTO:{
        //                 return calculate_button_relative_center_position_vertical_fixed_spacing();
        //                 break;
        //         }

        //         default:
        //                 break;
        //         }
        //         std::abort();
        // }

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

//         constexpr void UpdateMenuButtonTextLabelPosition(MenuButton &button, Lumen::Core::Math::Vec2i button_position) noexcept {
//                 Lumen::Core::Math::Vec2i text_bounds = {static_cast<int>(button.GetText().getLocalBounds().size.x),
//                                                         static_cast<int>(button.GetText().getLocalBounds().size.y)};
//                 Lumen::Core::Math::Vec2i button_size = this->m_bounding_box_menu_button.size;
//                 Lumen::Core::Math::Vec2i position;

//                 switch (button.GetMenuButtonLabelLayout()) {
//                 case MenuButton::LabelLayout::CENTER:
//                         std::cout << "Center\n";
//                         position = button_position + (button_size - text_bounds) / 2 - 
//                                    Lumen::Core::Math::Vec2i{static_cast<int>(button.GetText().getLocalBounds().position.x),
//                                    static_cast<int>(button.GetText().getLocalBounds().position.y)};
//                         break;
//                 case MenuButton::LabelLayout::LEFT:
//                         std::cout << "Left\n";
//                         position.x = button_position.x - static_cast<int>(button.GetText().getLocalBounds().position.x);
//                         position.y = button_position.y + (button_size.y - text_bounds.y) / 2 -
//                                      static_cast<int>(button.GetText().getLocalBounds().position.y);
//                         break;
//                 case MenuButton::LabelLayout::RIGHT:
//                         position.x = button_position.x + button_size.x - text_bounds.x -
//                                      static_cast<int>(button.GetText().getLocalBounds().position.x);
//                         position.y = button_position.y + (button_size.y - text_bounds.y) / 2 -
//                                      static_cast<int>(button.GetText().getLocalBounds().position.y);
//                         break;
//                 case MenuButton::LabelLayout::TOP:
//                         position.x = button_position.x + (button_size.x - text_bounds.x) / 2 -
//                                      static_cast<int>(button.GetText().getLocalBounds().position.x);
//                         position.y = button_position.y - static_cast<int>(button.GetText().getLocalBounds().position.y);
//                         break;
//                 case MenuButton::LabelLayout::BOTTOM:
//                         position.x = button_position.x + (button_size.x - text_bounds.x) / 2 -
//                                      static_cast<int>(button.GetText().getLocalBounds().position.x);
//                         position.y = button_position.y + button_size.y - text_bounds.y -
//                                      static_cast<int>(button.GetText().getLocalBounds().position.y);
//                         break;
//                 }
//                 button.SetButtonLabelPosition(position);
//         }

//         constexpr void UpdateMenuButtonBackgroundPosition(MenuButton &button, Lumen::Core::Math::Vec2i button_position) noexcept {
//                 button.SetMenuButtonBackgroundPosition(button_position);
//         }

//         constexpr void UpdateMenuButtonBackgroundSize(MenuButton &button) noexcept {
//                 auto button_size = this->m_bounding_box_menu_button.size;
//                 auto background_sprite = button.GetMenuButtonBackground();
//                 auto background_size = background_sprite.getTextureRect().size;
//                 auto scale = Lumen::Core::Math::Vec2f32{static_cast<float>(button_size.x) / static_cast<float>(background_size.x),
//                                                         static_cast<float>(button_size.y) / static_cast<float>(background_size.y)};
//                 button.SetMenuButtonBackgroundScale(scale);
//         }

public:

constexpr Lumen::Core::Math::Vec2f32 ComputeSubMenuRelativePosition(
        const Lumen::UI::Component::MenuButton &menu_button
        // const Lumen::UI::Component::Menu &sub_menu
        ) const noexcept
        {
                const auto &button_right_position = 
                        menu_button.GetTransformRectangleArea().relative_rectangle_area.bottom_right_position;
                return {button_right_position.x,
                        0};
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
                        // this->m_outline_color_menu = sf::Color::Green;
                        // for (auto iterator = this->m_buttons.begin(); iterator != this->m_buttons.end(); ++iterator) {
                        //         iterator->SetColor(sf::Color::Blue);
                        // }
                        // if (this->HasSelectedSubMenu() &&
                        //     (Lumen::UI::Component::SelectedSubMenu::SelectionTypeTag::HOVER == 
                        //      this->m_selected_sub_menu.value().selection_type)) {
                        //         //this->m_selected_sub_menu.Clear();
                        //         //std::cout << "UnSelectSubMenu: " << __LINE__ << "\n";
                        //         this->SubMenuIsSelected();
                        // }

                        return DoActionResult::NotHandledOrNotBlocked;
                }
                // this->m_outline_color_menu = sf::Color::Red;

                // const Lumen::UI::Component::RelativeSelectionAction relative_selection_to_menu{
                //         relative_selection_action_to_parent.selection_action,
                //         (relative_selection_action_to_parent.relative_position_to_the_parent_ui_component -
                //          this->m_transform_rectangle_area.top_left_position.top_left_position),
                // };
// std::cout << __FILE__ " :" << __LINE__ << "\n";
                // bool is_selection_handled{false};
                for (auto &menu_button : this->m_buttons) {
        
                        // if (is_selection_handled || (!this->IsMenuButtonSelected(relative_selection_to_menu, menu_button))) {
                        //         menu_button.SetColor(sf::Color::Blue);
                        //         continue;
                        // }

                        if (!this->IsMenuButtonSelected(relative_selection_action_to_parent, menu_button)) {
                                // menu_button.SetColor(sf::Color::Blue);
                                continue;
                        }

                        menu_button.SetSelected(true);
                        // menu_button.SetColor(sf::Color::Yellow);
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

                // if (is_selection_handled) {
                //         return;
                // }
                // //this->m_selected_sub_menu.Clear();
                // //std::cout << "UnSelectSubMenu: " << __LINE__ << "\n";
                // this->SubMenuIsSelected();
                return DoActionResult::HandledOrBlocked;
        }

// private:

//         static constexpr int ComputeSubMenuTopLeftPositionXValue(
//                 const Lumen::UI::Component::Menu::SelectedSubMenu::SpawnSide spawn_side,
//                 const Lumen::UI::Component::TransformTopLeft &selected_menu_botton_top_left_transform,
//                 const Lumen::UI::Component::BoundingBox &selected_menu_button_bounding_box,
//                 const Lumen::UI::Component::BoundingBox &sub_menu_bounding_box
//         ) noexcept 
//         {
//                 constexpr const int SUB_MENU_OFFSET_X = 3;
//                 switch (spawn_side) {
//                 case Lumen::UI::Component::SelectedSubMenu::SpawnSide::LEFT_SIDE:
//                         return selected_menu_botton_top_left_transform.top_left_position.x -
//                                sub_menu_bounding_box.size.x +
//                                SUB_MENU_OFFSET_X;
//                 case Lumen::UI::Component::SelectedSubMenu::SpawnSide::RIGHT_SIDE:
//                         return selected_menu_botton_top_left_transform.top_left_position.x +
//                                selected_menu_button_bounding_box.size.x -
//                                SUB_MENU_OFFSET_X;
//                 }

//                 std::abort();
//         }

//         constexpr int ComputeSelectedSubMenuCenterYPosition(
//                 const Lumen::UI::Component::SelectedSubMenu::SpawnAlignment spawn_alignment,
//                 const Lumen::UI::Component::TransformTopLeft &selected_menu_botton_top_left_transform,
//                 const Lumen::UI::Component::BoundingBox &selected_menu_button_bounding_box,
//                 const Lumen::UI::Component::BoundingBox &sub_menu_bounding_box
//         ) noexcept
//         {
//                 switch (spawn_alignment) {
//                 case Lumen::UI::Component::SelectedSubMenu::SpawnAlignment::TOP:
//                         return selected_menu_botton_top_left_transform.top_left_position.y;
//                 case Lumen::UI::Component::SelectedSubMenu::SpawnAlignment::BOTTOM:
//                         return selected_menu_botton_top_left_transform.top_left_position.y +
//                                selected_menu_button_bounding_box.size.y -
//                                sub_menu_bounding_box.size.y;
//                 }

//                 std::abort();
//         }


//         constexpr void SetSelectedSubMenuPosition(const Lumen::UI::Component::MenuButton &menu_button) noexcept
//         {
//                 const auto &menu_button_top_left_transform = menu_button.GetTransformRectangleArea();
//                 //const auto &menu_button_top_left_position = menu_button_top_left_transform.top_left_position;

//                 const auto &sub_menu_bounding_box = this->m_selected_sub_menu.sub_menu_ptr->GetMenuBoundingBox();

//                 const Lumen::UI::Component::TransformTopLeft sub_menu_relative_position_top_left = {{
//                         this->ComputeSubMenuTopLeftPositionXValue(
//                                 this->m_selected_sub_menu.spawn_side,
//                                 menu_button_top_left_transform.top_left_position,
//                                 this->m_bounding_box_menu_button,
//                                 sub_menu_bounding_box
//                         ),
//                         this->ComputeSelectedSubMenuCenterYPosition(
//                                 this->m_selected_sub_menu.spawn_alignment,
//                                 menu_button_top_left_transform.top_left_position,
//                                 this->m_bounding_box_menu_button,
//                                 sub_menu_bounding_box
//                         ),
//                 }};

//                 const auto sub_menu_position_top_left = 
//                         this->m_transform_rectangle_area.top_left_position.top_left_position +
//                         sub_menu_relative_position_top_left.top_left_position;

//                 this->m_selected_sub_menu.sub_menu_ptr->SetPosition(
//                         sub_menu_position_top_left
//                 );

//         }


//         constexpr const Lumen::UI::Component::BoundingBox &GetMenuBoundingBox(void) const noexcept
//         {
//                 return this->m_bounding_box_menu;
//         }

// public:
//         constexpr void AddMenuButton(Lumen::UI::Component::MenuButton &&menu_button) noexcept
//         {
//                 this->m_buttons.push_back(std::move(menu_button));
//         }

//         constexpr void SetPosition(const Lumen::Core::Math::Vec2i &position) noexcept override
//         {
//                 const auto position_move_offset =
//                         position - this->m_transform_rectangle_area.top_left_position.top_left_position;
//                 this->MovePosition(position_move_offset);
//         }

//         constexpr void MovePosition(const Lumen::Core::Math::Vec2i &position_move_offset) noexcept
//         {
//                 this->m_transform_rectangle_area.MovePosition(position_move_offset);
//                 for(auto &menu_button : this->m_buttons) {
//                         menu_button.MoveMenuButtonBackground({static_cast<float>(position_move_offset.x),
//                                                              static_cast<float>(position_move_offset.y)});
//                         menu_button.MoveMenuButtonTextlabel({static_cast<float>(position_move_offset.x),
//                                                              static_cast<float>(position_move_offset.y)});
//                 }
//         }


//         constexpr virtual void SetSize(const Lumen::Core::Math::Vec2i &size) noexcept override
//         {
//                 this->m_bounding_box_menu.size = size;
//                 //this->m_bounding_box_menu.half_size = size / 2;
//         }

        //constexpr virtual void SetText([[maybe_unused]]std::string &&text) noexcept {}
        //constexpr virtual void SetIcon([[maybe_unused]]const sf::Sprite &sprite) noexcept {}

        // constexpr std::optional<Lumen::Core::Math::Vec2i> GetPosition(void) const noexcept override
        // {
        //         return {this->m_transform_rectangle_area.top_left_position.top_left_position};
        // }

        // constexpr std::optional<Lumen::Core::Math::Vec2i> GetSize(void) const noexcept override
        // {
        //         return std::nullopt;
        // }

        // constexpr std::optional<std::string_view> GetText(void) const noexcept override
        // {
        //         return std::nullopt;
        // }

        // constexpr std::optional<sf::Sprite> GetIcon(void) const noexcept override
        // {
        //         return std::nullopt;
        // }
};

} // namespace Component
} // namespace UI        
} // namespace Lumen
