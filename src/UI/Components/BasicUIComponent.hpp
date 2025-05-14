#pragma once

#include <Core/Math/Vector.hpp>
#include <Action/Action.hpp>
#include <LayerStack/Layer.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <cassert>
#include <string>
#include <optional>
#include <string_view>

namespace Lumen {
namespace UI {
namespace Component {

enum class UIComponentTypeTag {
        NULL_COMPONENT,
        BUTTON,
        LABEL,
        TEXT_FIELD,
        TEXT_AREA,
        CHECKBOX,
        MENU,
        PANEL,
        NUMBER_OF_BASIC_UI_COMPONENTS,
};

struct RelativeSelectionAction {
        const Lumen::Action::SelectionAction &selection_action;
        Lumen::Core::Math::Vec2i relative_position_to_the_parent_ui_component;
};

class BasicUIComponent {
protected:
        UIComponentTypeTag m_ui_component_type;
        bool m_is_visible;
        mutable bool m_is_selected;

public:
        using DoActionResult = Lumen::LayerStack::BaseLayer::DoActionResult;

        constexpr BasicUIComponent(void) noexcept
        : m_ui_component_type{UIComponentTypeTag::NULL_COMPONENT}, m_is_visible{true}, m_is_selected{false} {}
        constexpr BasicUIComponent(UIComponentTypeTag ui_component_type, bool is_visable = true) noexcept
        : m_ui_component_type{ui_component_type}, m_is_visible{is_visable}, m_is_selected{false} {}

        constexpr BasicUIComponent(const BasicUIComponent &) noexcept = delete;
        constexpr BasicUIComponent(BasicUIComponent &&other) noexcept = default;

        constexpr BasicUIComponent &operator=(const BasicUIComponent &) noexcept = delete;
        constexpr BasicUIComponent &operator=(BasicUIComponent &&other) noexcept = default;

        constexpr virtual ~BasicUIComponent(void) noexcept {}

        constexpr virtual void Init(void) noexcept {} // = 0;
        constexpr virtual void Update(float delta_time) noexcept {(void)delta_time;} // = 0;
        constexpr virtual void Render(void) noexcept {} // = 0;
        constexpr virtual DoActionResult DoWindowResizeAction(
                const Lumen::Core::Math::Vec2i &window_new_size) noexcept {
                        (void)window_new_size;
                        return DoActionResult::NotHandledOrNotBlocked;} // = 0;
        constexpr virtual DoActionResult DoSelectionAction(
                const Lumen::UI::Component::RelativeSelectionAction &selection_action) noexcept
        {(void)selection_action;return DoActionResult::NotHandledOrNotBlocked;} // = 0;

        constexpr bool IsSelected(void) const noexcept
        {
                return this->m_is_selected;
        }

        constexpr void SetSelected(bool is_selected) const noexcept
        {
                this->m_is_selected = is_selected;
        }

        constexpr void SetVisible(bool is_visible) noexcept
        {
                this->m_is_visible = is_visible;
        }

        constexpr bool IsVisible(void) const noexcept
        {
                return this->m_is_visible;
        }

        constexpr Lumen::UI::Component::UIComponentTypeTag GetUIType(void) const noexcept
        {
                return this->m_ui_component_type;
        }
};

struct BoundingBox {
        Lumen::Core::Math::Vec2f32 size;
};

struct RelativeTransformTopLeft {
        Lumen::Core::Math::Vec2f32 position;
};

struct RelativeTransformBottomRight {
        Lumen::Core::Math::Vec2f32 position;
};

struct AbsoluteTransformTopLeft {
        Lumen::Core::Math::Vec2f32 position;
};

struct AbsoluteTransformBottomRight {
        Lumen::Core::Math::Vec2f32 position;
};

struct RelativeTransformRectangleArea {
        Lumen::Core::Math::Vec2f32 top_left_position;
        Lumen::Core::Math::Vec2f32 bottom_right_position;

        constexpr RelativeTransformRectangleArea(void) noexcept {}

        constexpr RelativeTransformRectangleArea(
                const Lumen::UI::Component::RelativeTransformTopLeft &top_left_position,
                const Lumen::UI::Component::RelativeTransformBottomRight &bottom_right_position) noexcept
        : top_left_position{top_left_position.position}, bottom_right_position{bottom_right_position.position} {}

        constexpr RelativeTransformRectangleArea(
                const Lumen::UI::Component::RelativeTransformTopLeft &top_left_position,
                const Lumen::UI::Component::BoundingBox &bounding_box) noexcept
        : top_left_position{top_left_position.position},
          bottom_right_position{top_left_position.position + bounding_box.size} {}

        constexpr void MovePosition(const Lumen::Core::Math::Vec2f32 &offset) noexcept
        {
                this->top_left_position += offset;
                this->bottom_right_position += offset;
        }

        constexpr void operator+=(const Lumen::Core::Math::Vec2f32 &offset) noexcept
        {
                this->MovePosition(offset);
        }
};

struct AbsoluteTransformRectangleArea {
        Lumen::Core::Math::Vec2f32 top_left_position;
        Lumen::Core::Math::Vec2f32 bottom_right_position;

        constexpr AbsoluteTransformRectangleArea(void) noexcept {}

        constexpr AbsoluteTransformRectangleArea(
                const Lumen::UI::Component::AbsoluteTransformTopLeft &top_left_position,
                const Lumen::UI::Component::AbsoluteTransformBottomRight &bottom_right_position) noexcept
        : top_left_position{top_left_position.position}, bottom_right_position{bottom_right_position.position} {}

        constexpr AbsoluteTransformRectangleArea(
                const Lumen::UI::Component::AbsoluteTransformTopLeft &top_left_position,
                const Lumen::UI::Component::BoundingBox &bounding_box) noexcept
        : top_left_position{top_left_position.position},
          bottom_right_position{top_left_position.position + bounding_box.size} {}
        
        constexpr AbsoluteTransformRectangleArea(
                const Lumen::UI::Component::RelativeTransformRectangleArea &relative_rectangle_area,
                const Lumen::Core::Math::Vec2f32 &offset) noexcept
        : top_left_position{relative_rectangle_area.top_left_position + offset},
          bottom_right_position{relative_rectangle_area.bottom_right_position + offset} {}

        constexpr void MovePosition(const Lumen::Core::Math::Vec2f32 &offset) noexcept
        {
                this->top_left_position += offset;
                this->bottom_right_position += offset;
        }

        constexpr void operator+=(const Lumen::Core::Math::Vec2f32 &offset) noexcept
        {
                this->MovePosition(offset);
        }
};

struct TransformRectangleArea {
        RelativeTransformRectangleArea relative_rectangle_area;
        AbsoluteTransformRectangleArea absolute_rectangle_area;

        constexpr TransformRectangleArea(void) noexcept {}

        constexpr TransformRectangleArea(
                const Lumen::UI::Component::RelativeTransformRectangleArea &relative_rectangle_area,
                const Lumen::UI::Component::AbsoluteTransformRectangleArea &absolute_rectangle_area) noexcept
        : relative_rectangle_area{relative_rectangle_area}, absolute_rectangle_area{absolute_rectangle_area} {}

        constexpr TransformRectangleArea(
                const Lumen::UI::Component::RelativeTransformRectangleArea &relative_rectangle_area,
                const Lumen::Core::Math::Vec2f32 offset) noexcept
        : relative_rectangle_area{relative_rectangle_area},
          absolute_rectangle_area{
                Lumen::UI::Component::AbsoluteTransformTopLeft{relative_rectangle_area.top_left_position + offset},
                Lumen::UI::Component::AbsoluteTransformBottomRight{relative_rectangle_area.bottom_right_position + offset}} {}

        constexpr void MovePosition(const Lumen::Core::Math::Vec2f32 &offset) noexcept
        {
                this->relative_rectangle_area += offset;
                this->absolute_rectangle_area += offset;
        }
};

struct RelativeSelectedPosition {
        Lumen::Core::Math::Vec2f32 position;
};

struct  AbsoluteSelectedPosition {
        Lumen::Core::Math::Vec2f32 position;
};

constexpr bool IsIncluded(const Lumen::UI::Component::RelativeSelectedPosition &relative_selected_position,
                          const Lumen::UI::Component::RelativeTransformRectangleArea &relative_rectangle_area) noexcept
{
        return (relative_selected_position.position.x >= relative_rectangle_area.top_left_position.x) &&
               (relative_selected_position.position.x <= relative_rectangle_area.bottom_right_position.x) &&
               (relative_selected_position.position.y >= relative_rectangle_area.top_left_position.y) &&
               (relative_selected_position.position.y <= relative_rectangle_area.bottom_right_position.y);
}

constexpr bool IsIncluded(const Lumen::UI::Component::AbsoluteSelectedPosition &absolute_selected_position,
                          const Lumen::UI::Component::AbsoluteTransformRectangleArea &absolute_rectangle_area) noexcept
{
        return (absolute_selected_position.position.x >= absolute_rectangle_area.top_left_position.x) &&
               (absolute_selected_position.position.x <= absolute_rectangle_area.bottom_right_position.x) &&
               (absolute_selected_position.position.y >= absolute_rectangle_area.top_left_position.y) &&
               (absolute_selected_position.position.y <= absolute_rectangle_area.bottom_right_position.y);
}

struct Sprite {
        Lumen::UI::Component::AbsoluteTransformRectangleArea texture_rectangle_area;
};

} // namespace Component
} // namespace UI
} // namespace Lumen