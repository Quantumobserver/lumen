
#pragma once

#include <Core/Math/Vector.hpp>

#ifdef LUMEN_DEBUG_ACTION_FORMAT_ENABLED
        #include <string>
        #include <cstdlib>
#endif // LUMEN_DEBUG_ACTION_FORMAT_ENABLED

namespace Lumen {
namespace Action {

enum class ActionName {
        NONE,

        PREVIOUS_LEVEL,
        NEXT_LEVEL,
        START_SELECTED_LEVEL,

        MOVE_UP,
        MOVE_DOWN,
        MOVE_LEFT,
        MOVE_RIGHT,
        JUMP,
        SQUAT,
        SHOOT,

        QUIT,
        PAUSE,

        TOGGLE_DRAWING_BOUNDING_BOX,
        TOGGLE_DRAWING_TEXTURE,
        TOGGLE_DRAWING_GRID,

        NUMBER_OF_ACTIONS,
};

#ifdef LUMEN_DEBUG_ACTION_FORMAT_ENABLED

std::string fmt(ActionName an) noexcept
{
        switch (an) {
#define FMT_CASE(NAME) \
        case ActionName::NAME: \
                return #NAME
        
        FMT_CASE(NONE);

        FMT_CASE(PREVIOUS_LEVEL);
        FMT_CASE(NEXT_LEVEL);
        FMT_CASE(START_SELECTED_LEVEL);

        FMT_CASE(MOVE_UP);
        FMT_CASE(MOVE_DOWN);
        FMT_CASE(MOVE_LEFT);
        FMT_CASE(MOVE_RIGHT);
        FMT_CASE(JUMP);
        FMT_CASE(SQUAT);
        FMT_CASE(SHOOT);

        FMT_CASE(QUIT);
        FMT_CASE(PAUSE);

        FMT_CASE(TOGGLE_DRAWING_BOUNDING_BOX);
        FMT_CASE(TOGGLE_DRAWING_TEXTURE);
        FMT_CASE(TOGGLE_DRAWING_GRID);

        FMT_CASE(NUMBER_OF_ACTIONS);

#undef FMT_CASE
        }

        std::abort();
}

#endif // LUMEN_DEBUG_ACTION_FORMAT_ENABLED

enum class ActionKind {
        CONTINUOUS,
        TRIGGER,
};

#ifdef LUMEN_DEBUG_ACTION_FORMAT_ENABLED

std::string fmt(ActionKind ak) noexcept
{
        switch (ak) {
        case ActionKind::CONTINUOUS:
                return "CONTINUOUS";
        
        case ActionKind::TRIGGER:
                return "TRIGGER";
        }

        std::abort();
}

#endif // LUMEN_DEBUG_ACTION_FORMAT_ENABLED

// There are two kinds of action: continuous action and trigger action.
//
// Continuous actions have two status: BEGIN and END, when player press
// a key, the continuous action BEGIN, it will happen every update until
// the key been released, it become END status.
//
// Trigger actions have three status: TRIGGER, TRIGGER_END and END,
// trigger action only happen once when key is pressed, it will not repeat.
// Its status will be reset when key is released, it can be triggered again.
// When player press a key, the trigger action TRIGGER, when the action is
// polled from the ActionManager, its status will be set to TRIGGER_END,
// when the key been released, it become END status.
enum class ActionStatus {
        BEGIN,
        TRIGGER,
        TRIGGER_END,
        END,
};

#ifdef LUMEN_DEBUG_ACTION_FORMAT_ENABLED

std::string fmt(ActionStatus as) noexcept
{
        switch (as) {
        case ActionStatus::BEGIN:
                return "BEGIN";
        
        case ActionStatus::TRIGGER:
                return "TRIGGER";

        case ActionStatus::TRIGGER_END:
                return "TRIGGER_END";

        case ActionStatus::END:
                return "END";
        }

        std::abort();
}

#endif // LUMEN_DEBUG_ACTION_FORMAT_ENABLED

struct Action {
        ActionName action_name;
        ActionStatus action_status;
};

struct MovementAction {
        bool is_move_up : 1;
        bool is_move_down : 1;
        bool is_move_left : 1;
        bool is_move_right : 1;
        bool has_movement : 1;
};

struct SelectionAction {
        enum class SelectionActionTypeTag {
                NONE,
                PRESS,
                RELEASE,
                WHEEL_SCROLL,
                CURSOR_MOVEMENT,
        } selection_action_type;

        Lumen::Core::Math::Vec2i position;

        struct Void {};
        enum class ClickTypeTag {
                UNKNOWN,
                LEFT_CLICK,
                RIGHT_CLICK,
                MIDDLE_CLICK,
        };
        struct WheelScroll {
                enum class ScrollDirectionTag {
                        VERTICAL,
                        HORIZONTAL,
                } scroll_direction;

                float wheel_offset;
        };
        union {
                Void no_data;

                ClickTypeTag click_type;

                WheelScroll wheel_scroll;
        };

        constexpr SelectionAction(void) noexcept
        : selection_action_type{SelectionActionTypeTag::NONE}, no_data{} {}
        constexpr SelectionAction(SelectionActionTypeTag selection_action_type,
                                  const Lumen::Core::Math::Vec2i &position) noexcept
        : selection_action_type{selection_action_type}, position{position}
        {
                assert(SelectionActionTypeTag::CURSOR_MOVEMENT == selection_action_type);
        }

        constexpr SelectionAction(SelectionActionTypeTag selection_action_type,
                                  const Lumen::Core::Math::Vec2i &position,
                                  ClickTypeTag click_type) noexcept
        : selection_action_type{selection_action_type}, position{position}, click_type{click_type}
        {
                assert((SelectionActionTypeTag::PRESS == selection_action_type) ||
                       (SelectionActionTypeTag::RELEASE == selection_action_type));
        }
        
        constexpr SelectionAction(SelectionActionTypeTag selection_action_type,
                                  const Lumen::Core::Math::Vec2i &position,
                                  const WheelScroll &wheel_scroll) noexcept
        : selection_action_type{selection_action_type}, position{position}, wheel_scroll{wheel_scroll}
        {
                assert(SelectionActionTypeTag::WHEEL_SCROLL == selection_action_type);
        }

        constexpr void Clear(void) noexcept
        {
                this->selection_action_type = SelectionActionTypeTag::NONE;
                this->no_data = Void{};
        }

        constexpr bool HasSelectionAction(void) const noexcept
        {
                return SelectionActionTypeTag::NONE != this->selection_action_type;
        }
};

#ifdef LUMEN_DEBUG_ACTION_FORMAT_ENABLED

std::string fmt(SelectionAction::SelectionActionTypeTag sst) noexcept
{
        switch (sst) {
        case SelectionAction::SelectionActionTypeTag::NONE:
                return "NONE";
        
        case SelectionAction::SelectionActionTypeTag::PRESS:
                return "PRESS";

        case SelectionAction::SelectionActionTypeTag::RELEASE:
                return "RELEASE";

        case SelectionAction::SelectionActionTypeTag::WHEEL_SCROLL:
                return "WHEEL_SCROLL";

        case SelectionAction::SelectionActionTypeTag::CURSOR_MOVEMENT:
                return "CURSOR_MOVEMENT";
        }

        std::abort();
}

std::string fmt(SelectionAction::ClickTypeTag ctt) noexcept
{
        switch (ctt) {
        case SelectionAction::ClickTypeTag::UNKNOWN:
                return "UNKNOWN";
        
        case SelectionAction::ClickTypeTag::LEFT_CLICK:
                return "LEFT_CLICK";

        case SelectionAction::ClickTypeTag::RIGHT_CLICK:
                return "RIGHT_CLICK";

        case SelectionAction::ClickTypeTag::MIDDLE_CLICK:
                return "MIDDLE_CLICK";
        }

        std::abort();
}

std::string fmt(SelectionAction::WheelScroll::ScrollDirectionTag sdt) noexcept
{
        switch (sdt) {
        case SelectionAction::WheelScroll::ScrollDirectionTag::VERTICAL:
                return "VERTICAL";
        
        case SelectionAction::WheelScroll::ScrollDirectionTag::HORIZONTAL:
                return "HORIZONTAL";
        }

        std::abort();
}


#endif // LUMEN_DEBUG_ACTION_FORMAT_ENABLED

} // namespace Action
} // namespace Lumen
