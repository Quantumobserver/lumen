
#pragma once

#include <LumenDef/ConstexprIf.hpp>

#include <cassert>
#include <cstddef>
#include <vector>

#include "Layer.hpp"

namespace Lumen {
namespace LayerStack {

class LayerStack {
private:
        std::vector<Lumen::LayerStack::LayerPtr> m_layers;
        std::vector<Lumen::LayerStack::LayerPtr> m_popped_but_not_destroyed_layers;

public:
        using MutableIterator = std::vector<Lumen::LayerStack::LayerPtr>::iterator;
        using ConstIterator = std::vector<Lumen::LayerStack::LayerPtr>::const_iterator;
        using MutableReverseIterator = std::vector<Lumen::LayerStack::LayerPtr>::reverse_iterator;
        using ConstReverseIterator = std::vector<Lumen::LayerStack::LayerPtr>::const_reverse_iterator;

        CONSTEXPR_IF_CXX_20 LayerStack(void) noexcept {}
        //constexpr ~LayerStack(void) noexcept
        //{
                // TODO: 
        //}

        constexpr LayerStack(const Lumen::LayerStack::LayerStack &) noexcept = delete;
        CONSTEXPR_IF_CXX_23 LayerStack(Lumen::LayerStack::LayerStack &&other) noexcept = default;

        constexpr Lumen::LayerStack::LayerStack &operator=(const Lumen::LayerStack::LayerStack &) noexcept = delete;
        CONSTEXPR_IF_CXX_23 Lumen::LayerStack::LayerStack &operator=(Lumen::LayerStack::LayerStack &&other) noexcept = default;

        CONSTEXPR_IF_CXX_23
        void PushBackLayer(Lumen::LayerStack::LayerPtr &&layer) noexcept
        {
                this->m_layers.push_back(std::move(layer));
        }

        template<typename Layer, typename ...Args>
        CONSTEXPR_IF_CXX_23
        void EmplaceBackLayer(Args &&...args) noexcept
        {
                this->m_layers.emplace_back(MakeLayer<Layer>(std::forward<Args>(args)...));
        }

        CONSTEXPR_IF_CXX_23 Lumen::LayerStack::LayerPtr &PopBackLayer(void) noexcept
        {
                assert(!this->m_layers.empty());

                auto last = std::move(this->m_layers.back());
                this->m_popped_but_not_destroyed_layers.push_back(std::move(last));
                this->m_layers.pop_back();

                return this->m_popped_but_not_destroyed_layers.back();
        }

        constexpr void PopBackAndDestroyLayer(void) noexcept
        {
                assert(!this->m_layers.empty());

                this->m_layers.pop_back();
        }

        constexpr Lumen::LayerStack::LayerPtr &PopLayer(Lumen::LayerStack::LayerPtr &target_layer) noexcept
        {
                assert(!this->m_layers.empty());

                this->m_popped_but_not_destroyed_layers.push_back(std::move(target_layer));
                this->m_layers.erase(
                        std::remove_if(this->m_layers.begin(), this->m_layers.end(),
                               [&target_layer](const Lumen::LayerStack::LayerPtr &layer_from_vector){
                                return ((&target_layer) == (&layer_from_vector));
                               })
                );
                return this->m_popped_but_not_destroyed_layers.back();
        }

        constexpr void PopAndDestroyLayer(Lumen::LayerStack::LayerPtr &target_layer) noexcept
        {
                assert(!this->m_layers.empty());

                this->m_layers.erase(
                        std::remove_if(this->m_layers.begin(), this->m_layers.end(),
                               [&target_layer](Lumen::LayerStack::LayerPtr &layer_from_vector){
                                return ((&target_layer) == (&layer_from_vector));
                               })
                );
        }

        constexpr void DestroyAllPoppedLayer(void) noexcept
        {
                this->m_popped_but_not_destroyed_layers.clear();
        }

        constexpr MutableIterator begin(void) noexcept
        {
                return this->m_layers.begin();
        }

        constexpr ConstIterator begin(void) const noexcept
        {
                return this->m_layers.begin();
        }

        constexpr ConstIterator cbegin(void) const noexcept
        {
                return this->m_layers.cbegin();
        }

        constexpr MutableIterator end(void) noexcept
        {
                return this->m_layers.end();
        }

        constexpr ConstIterator end(void) const noexcept
        {
                return this->m_layers.end();
        }

        constexpr ConstIterator cend(void) const noexcept
        {
                return this->m_layers.cend();
        }

        constexpr MutableReverseIterator rbegin(void) noexcept
        {
                return this->m_layers.rbegin();
        }

        constexpr ConstReverseIterator rbegin(void) const noexcept
        {
                return this->m_layers.rbegin();
        }

        constexpr ConstReverseIterator crbegin(void) const noexcept
        {
                return this->m_layers.crbegin();
        }

        constexpr MutableReverseIterator rend(void) noexcept
        {
                return this->m_layers.rend();
        }

        constexpr ConstReverseIterator rend(void) const noexcept
        {
                return this->m_layers.rend();
        }

        constexpr ConstReverseIterator crend(void) const noexcept
        {
                return this->m_layers.crend();
        }
};

} //namespace LayerStack
} // namespace Lumen
