
#pragma once

#include <LumenDef/ConstexprIf.hpp>

#include <type_traits>
#include <memory>
#include <utility>

#include <Action/Action.hpp>

namespace Lumen {
namespace LayerStack {

class BaseLayer {
private:
        /* data */
public:
        constexpr BaseLayer(void) noexcept = default;
        CONSTEXPR_IF_CXX_20 virtual ~BaseLayer(void) noexcept {}

        constexpr BaseLayer(const BaseLayer &) noexcept = delete;
        constexpr BaseLayer(BaseLayer &&other) noexcept = default;

        constexpr BaseLayer &operator=(const BaseLayer &) noexcept = delete;
        constexpr BaseLayer &operator=(BaseLayer &&other) noexcept = default;

        CONSTEXPR_IF_CXX_20 virtual void Attach(void) noexcept {}
        CONSTEXPR_IF_CXX_20 virtual void Detach(void) noexcept {}
        CONSTEXPR_IF_CXX_20 virtual void Update(void) noexcept {}
        CONSTEXPR_IF_CXX_20 virtual void Render(void) noexcept {}

        enum class DoActionResult {
                NotHandledOrNotBlocked,
                HandledOrBlocked,
        };
        
        [[nodiscard]] CONSTEXPR_IF_CXX_20
        virtual DoActionResult DoAction([[maybe_unused]] Lumen::Action::Action action) noexcept
        {
                return DoActionResult::NotHandledOrNotBlocked;
        }

        [[nodiscard]] CONSTEXPR_IF_CXX_20
        virtual DoActionResult DoSelectionAction([[maybe_unused]] Lumen::Action::SelectionAction selection_action) noexcept
        {
                return DoActionResult::NotHandledOrNotBlocked;
        }
        
        [[nodiscard]] CONSTEXPR_IF_CXX_20
        virtual DoActionResult DoMovementAction([[maybe_unused]] Lumen::Action::MovementAction movement_action) noexcept
        {
                return DoActionResult::NotHandledOrNotBlocked;
        }
};

class LayerPtr {
private:
        std::unique_ptr<BaseLayer> m_layer_ptr;

        CONSTEXPR_IF_CXX_23
        LayerPtr(std::unique_ptr<BaseLayer> &&layer_ptr) noexcept
         : m_layer_ptr{std::move(layer_ptr)} {}
public:

        constexpr LayerPtr(const LayerPtr &) noexcept = delete;

        CONSTEXPR_IF_CXX_23
        LayerPtr(LayerPtr &&other) noexcept : m_layer_ptr{std::move(other.m_layer_ptr)} {}

        constexpr LayerPtr &operator=(const LayerPtr &) noexcept = delete;
        CONSTEXPR_IF_CXX_23
        LayerPtr &operator=(LayerPtr &&other) noexcept
        {
                this->m_layer_ptr = std::move(other.m_layer_ptr);
                return *this;
        }

        CONSTEXPR_IF_CXX_23
        auto operator->(void) noexcept -> decltype(this->m_layer_ptr.operator->())
        {
                return this->m_layer_ptr.operator->();
        }

        template<typename T>
        friend CONSTEXPR_IF_CXX_23 LayerPtr MakeLayer(void) noexcept;

        template<typename T, typename ...Args>
        friend CONSTEXPR_IF_CXX_23 LayerPtr MakeLayer(Args &&...args) noexcept;
};

template<typename T>
CONSTEXPR_IF_CXX_23 LayerPtr MakeLayer(void) noexcept
{
        static_assert(std::is_base_of<BaseLayer, T>::value, "type T must be a derive class of Layer.");
        return {std::make_unique<T>()};
}

template<typename T, typename ...Args>
CONSTEXPR_IF_CXX_23 LayerPtr MakeLayer(Args &&...args) noexcept
{
        static_assert(std::is_base_of<BaseLayer, T>::value, "type T must be a derive class of Layer.");
        return {std::make_unique<T>(std::forward<Args>(args)...)};
}

} //namespace LayerStack
} // namespace Lumen
