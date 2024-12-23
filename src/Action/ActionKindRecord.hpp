
#include "Action.hpp"

#include <cassert>
#include <cstddef>
#include <array>
#include <utility>

#ifdef LUMEN_DEBUG_ACTION_FORMAT_ENABLED
        #include <iostream>
#endif // LUMEN_DEBUG_ACTION_FORMAT_ENABLED

namespace Lumen {
namespace Action {

class ActionKindRecord {
private:
        static constexpr const std::size_t NUMBER_OF_ACTIONS = static_cast<std::size_t>(Lumen::Action::ActionName::NUMBER_OF_ACTIONS);

        static constexpr std::array<Lumen::Action::ActionStatus, 2> ACTION_START_STATUS_TABLE{
                Lumen::Action::ActionStatus::BEGIN,
                Lumen::Action::ActionStatus::TRIGGER,
        };

        using this_t = ActionKindRecord;

        std::array<Lumen::Action::ActionKind,
                   NUMBER_OF_ACTIONS
                   > m_action_kind_record;

public:
        constexpr ActionKindRecord(void) noexcept : m_action_kind_record{Lumen::Action::ActionKind::CONTINUOUS}
        {
#ifdef LUMEN_DEBUG_ACTION_FORMAT_ENABLED

                //std::cout << "[ActionKindRecord]: ";
                for (std::size_t i = 0; i < this->m_action_kind_record.size(); ++i) {
                        std::cout << fmt(this->m_action_kind_record[i]) << " ";
                }
                //std::cout << "\n";

#endif // LUMEN_DEBUG_ACTION_FORMAT_ENABLED
        }

        template<typename ...Args>
        constexpr ActionKindRecord(Args &&...args) noexcept : m_action_kind_record{std::forward<Args>(args)...} {}

        constexpr ActionKindRecord(const ActionKindRecord &) noexcept = delete;
        constexpr ActionKindRecord(ActionKindRecord &&other) noexcept : m_action_kind_record{std::move(other.m_action_kind_record)} {}

        constexpr ActionKindRecord &operator=(const ActionKindRecord &) noexcept = delete;
        constexpr ActionKindRecord &operator=(ActionKindRecord &&other) noexcept
        {
                this->m_action_kind_record = std::move(other.m_action_kind_record);
                return *this;
        }

        constexpr Lumen::Action::ActionKind GetActionKind(Lumen::Action::ActionName action_name) const noexcept
        {
                assert(static_cast<std::size_t>(action_name) < this->m_action_kind_record.size());
//std::cout << "[GetActionKind]: " << fmt(this->m_action_kind_record[static_cast<std::size_t>(action_name)]) << "\n";
                return this->m_action_kind_record[static_cast<std::size_t>(action_name)];
        }

        constexpr Lumen::Action::ActionStatus GetActionStartStatus(Lumen::Action::ActionName action_name) const noexcept
        {
                assert(static_cast<std::size_t>(action_name) < this->m_action_kind_record.size());
                Lumen::Action::ActionKind action_kind = this->GetActionKind(action_name);
                assert(static_cast<std::size_t>(action_kind) < this_t::ACTION_START_STATUS_TABLE.size());
                return this_t::ACTION_START_STATUS_TABLE[static_cast<std::size_t>(action_kind)];
        }

        constexpr void SetActionKind(Lumen::Action::ActionName action_name, Lumen::Action::ActionKind action_kind) noexcept
        {
                this->m_action_kind_record[static_cast<std::size_t>(action_name)] = action_kind;
        }
};

} // namespace Action
} // namespace Lumen
