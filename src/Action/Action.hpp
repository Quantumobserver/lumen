
#pragma once

#ifdef LUMEN_DEBUG_ACTION_FORMAT_ENABLED
        #include <string>
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
}

#endif // LUMEN_DEBUG_ACTION_FORMAT_ENABLED

struct Action {
        ActionName action_name;
        ActionStatus action_status;
};

} // namespace Action
} // namespace Lumen
