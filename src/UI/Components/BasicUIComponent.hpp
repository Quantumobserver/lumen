#pragma once

#include <Core/Math/Vector.hpp>
#include <Action/Action.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <cassert>
#include <string>
#include <optional>

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
};

struct RelativeSelectionAction {
        const Lumen::Action::SelectionAction &selection_action;
        Lumen::Core::Math::Vec2i relative_position_to_the_parent_ui_component;
};

class BasicUIComponent {
protected:
        UIComponentTypeTag m_ui_component_type;
        sf::RenderWindow *m_window_ptr;
public:
        constexpr BasicUIComponent(void) noexcept : m_ui_component_type{UIComponentTypeTag::NULL_COMPONENT}, m_window_ptr{nullptr} {}
        constexpr BasicUIComponent(UIComponentTypeTag ui_component_type, sf::RenderWindow *window_ptr) noexcept
        : m_ui_component_type{ui_component_type}, m_window_ptr{window_ptr} {}

        constexpr BasicUIComponent(const BasicUIComponent &) noexcept = delete;
        constexpr BasicUIComponent(BasicUIComponent &&other) noexcept
        : m_ui_component_type{other.m_ui_component_type}, m_window_ptr{other.m_window_ptr}
        {
                other.m_ui_component_type = UIComponentTypeTag::NULL_COMPONENT;
                other.m_window_ptr = nullptr;
        }

        constexpr BasicUIComponent &operator=(const BasicUIComponent &) noexcept = delete;
        constexpr BasicUIComponent &operator=(BasicUIComponent &&other) noexcept
        {
                if (this == &other) {
                        return *this;
                }
                this->m_ui_component_type = other.m_ui_component_type;
                this->m_window_ptr = other.m_window_ptr;
                other.m_ui_component_type = UIComponentTypeTag::NULL_COMPONENT;
                other.m_window_ptr = nullptr;
                return *this;
        }

        constexpr virtual ~BasicUIComponent(void) noexcept {}

        constexpr virtual void Init(void) noexcept = 0;
        constexpr virtual void Update(const Lumen::Core::Math::Vec2i &offset) noexcept = 0;
        constexpr virtual void Render(void) noexcept = 0;
        constexpr virtual void DoWindowResizeAction(const Lumen::Core::Math::Vec2i &window_new_size) noexcept = 0;
        constexpr virtual void DoSelectionAction(const Lumen::UI::Component::RelativeSelectionAction &selection_action) noexcept = 0;

        constexpr virtual void AddComponent([[maybe_unused]] Lumen::UI::Component::BasicUIComponent &&basic_ui_component) noexcept {}

        constexpr virtual void SetPosition([[maybe_unused]] const Lumen::Core::Math::Vec2i &position) noexcept {}
        constexpr virtual void SetSize([[maybe_unused]] const Lumen::Core::Math::Vec2i &size) noexcept {}
        constexpr virtual void SetText([[maybe_unused]] std::string &&text) noexcept {}
        constexpr virtual void SetIcon([[maybe_unused]] const sf::Sprite &sprite) noexcept {}

        constexpr virtual std::optional<Lumen::Core::Math::Vec2i> GetPosition(void) const noexcept
        {
                return std::nullopt;
        }

        constexpr virtual std::optional<Lumen::Core::Math::Vec2i> GetSize(void) const noexcept
        {
                return std::nullopt;
        }

        constexpr virtual std::optional<const char *> GetText(void) const noexcept
        {
                return std::nullopt;
        }

        constexpr virtual std::optional<sf::Sprite> GetIcon(void) const noexcept
        {
                return std::nullopt;
        }

        constexpr Lumen::UI::Component::UIComponentTypeTag GetUIType(void) const noexcept
        {
                return this->m_ui_component_type;
        }
};

struct BoundingBox {
        Lumen::Core::Math::Vec2i size;
        Lumen::Core::Math::Vec2i half_size;

        constexpr BoundingBox(void) noexcept = default;
        constexpr BoundingBox(const Lumen::Core::Math::Vec2i &size) noexcept
        : size{size}, half_size{size / 2} {}
        constexpr BoundingBox(const BoundingBox &other) noexcept = default;
        constexpr BoundingBox &operator=(const BoundingBox &other) noexcept = default;
};

struct TransformCenter {
        Lumen::Core::Math::Vec2i center_position;

        constexpr TransformCenter(void) noexcept {}
        constexpr TransformCenter(const Lumen::Core::Math::Vec2i &center_position) noexcept
        : center_position{center_position} {}
        constexpr TransformCenter(const TransformCenter &other) noexcept = default;
        constexpr TransformCenter &operator=(const TransformCenter &other) noexcept = default;
};

struct TransformTopLeft {
        Lumen::Core::Math::Vec2i top_left_position;

        constexpr TransformTopLeft(void) noexcept {}
        constexpr TransformTopLeft(const Lumen::Core::Math::Vec2i &top_left_position) noexcept
        : top_left_position{top_left_position} {}
        constexpr TransformTopLeft(const TransformTopLeft &other) noexcept = default;
        constexpr TransformTopLeft &operator=(const TransformTopLeft &other) noexcept = default;
};

constexpr bool IsSelected(const Lumen::UI::Component::TransformCenter &selection_position,
                          const Lumen::UI::Component::BoundingBox &bounding_box,
                          const Lumen::UI::Component::TransformCenter &transform_center) noexcept
{
        Lumen::Core::Math::Vec2i distance = transform_center.center_position - selection_position.center_position;

        distance.x = Lumen::Core::Math::Abs(distance.x);
        distance.y = Lumen::Core::Math::Abs(distance.y);

        return (distance.x < bounding_box.half_size.x) &&
               (distance.y < bounding_box.half_size.y);

}

} // namespace Component
} // namespace UI
} // namespace Lumen