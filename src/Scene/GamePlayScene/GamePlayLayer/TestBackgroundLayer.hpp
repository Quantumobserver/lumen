
#pragma once

#include <Scene/Scene.hpp>

#include <cassert>
#include <iostream>

namespace Lumen {
namespace Scene {

namespace GamePlayLayer {

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
                        //std::cout << "[GamePlayScene:TestBackgroundLayer]: MOVE_RIGHT\n";
                        return Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked;
                
                default:
                        break;
                }
                return Lumen::LayerStack::BaseLayer::DoActionResult::NotHandedOrNotBlocked;
        }
};

} // namespace GamePlayLayer
} // namespace Scene
} // namespace Lumen