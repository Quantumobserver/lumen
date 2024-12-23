
#pragma once

#include <

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
        constexpr virtual ~BaseLayer(void) noexcept {}

        constexpr BaseLayer(const BaseLayer &) noexcept = delete;
        constexpr BaseLayer(BaseLayer &&other) noexcept = default;

        constexpr BaseLayer &operator=(const BaseLayer &) noexcept = delete;
        constexpr BaseLayer &operator=(BaseLayer &&other) noexcept = default;

        constexpr virtual void Attach(void) noexcept {}
        constexpr virtual void Detach(void) noexcept {}
        constexpr virtual void Update(void) noexcept {}
        constexpr virtual void Render(void) noexcept {}

        enum class DoActionResult {
                NotHandedOrNotBlocked,
                HandledOrBlocked,
        };
        
        [[nodiscard]] constexpr virtual DoActionResult DoAction([[maybe_unused]] Lumen::Action::Action action) noexcept
        {
                return DoActionResult::NotHandedOrNotBlocked;
        }

        [[nodiscard]] constexpr virtual DoActionResult DoMovementAction([[maybe_unused]] Lumen::Action::MovementAction movement_action) noexcept
        {
                return DoActionResult::NotHandedOrNotBlocked;
        }
};

class LayerPtr {
private:
        std::unique_ptr<BaseLayer> m_layer_ptr;

        constexpr LayerPtr(std::unique_ptr<BaseLayer> &&layer_ptr) noexcept
         : m_layer_ptr{std::move(layer_ptr)} {}
public:

        constexpr LayerPtr(const LayerPtr &) noexcept = delete;
        constexpr LayerPtr(LayerPtr &&other) noexcept : m_layer_ptr{std::move(other.m_layer_ptr)} {}

        constexpr LayerPtr &operator=(const LayerPtr &) noexcept = delete;
        constexpr LayerPtr &operator=(LayerPtr &&other) noexcept
        {
                this->m_layer_ptr = std::move(other.m_layer_ptr);
                return *this;
        }

        constexpr auto operator->(void) noexcept -> decltype(this->m_layer_ptr.operator->())
        {
                return this->m_layer_ptr.operator->();
        }

        template<typename T>
        friend LayerPtr MakeLayer(void) noexcept;

        template<typename T, typename ...Args>
        friend LayerPtr MakeLayer(Args &&...args) noexcept;
};

template<typename T>
LayerPtr MakeLayer(void) noexcept
{
        static_assert(std::is_base_of<BaseLayer, T>::value, "type T must be a derive class of Layer.");
        return {std::make_unique<T>()};
}

template<typename T, typename ...Args>
LayerPtr MakeLayer(Args &&...args) noexcept
{
        static_assert(std::is_base_of<BaseLayer, T>::value, "type T must be a derive class of Layer.");
        return {std::make_unique<T>(std::forward<Args>(args)...)};
}

} //namespace LayerStack
} // namespace Lumen
