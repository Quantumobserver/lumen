
#pragma once

#include <Scene/Scene.hpp>

#include <cassert>
#include <iostream>

namespace Lumen {
namespace Scene {
namespace MenuLayer {

class TestBackgroundLayer : public Lumen::LayerStack::BaseLayer {
public:

        constexpr void Update(void) noexcept override
        {
                //std::cout << "[ExampleLayer2]: Update\n";
        }

        [[nodiscard]] constexpr Lumen::LayerStack::BaseLayer::DoActionResult DoAction(Lumen::Action::Action action) noexcept override
        {
                switch (action.action_name) {
                case Lumen::Action::ActionName::MOVE_RIGHT:
                        std::cout << "[MenuScene:TestBackgroundLayer]: MOVE_RIGHT\n";
                        return Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked;
                
                default:
                        break;
                }
                return Lumen::LayerStack::BaseLayer::DoActionResult::NotHandledOrNotBlocked;
        }
};


} // namespace MenuLayer
} // namespace Scene
} // namespace Lumen