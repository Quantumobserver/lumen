
#pragma once

#include <Scene/Scene.hpp>

#include <cassert>
#include <iostream>

namespace Lumen {
namespace Scene {

namespace GamePlayLayer {

class GameWorldLayer : public Lumen::LayerStack::BaseLayer {
private:
        Lumen::Scene::BaseScene *m_game_play_scene;
public:
        constexpr GameWorldLayer(Lumen::Scene::BaseScene *game_play_scene) noexcept
        : m_game_play_scene{game_play_scene}
        {
                assert(nullptr != this->m_game_play_scene);
        }

        constexpr void Update(void) noexcept override
        {

        }

        constexpr void Render(void) noexcept override
        {

        }

        [[nodiscard]] constexpr Lumen::LayerStack::BaseLayer::DoActionResult DoAction([[maybe_unused]] Lumen::Action::Action action) noexcept override
        {
                switch (action.action_name) {

                //case Lumen::Action::ActionName::JUMP:
                //        return Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked;

                case Lumen::Action::ActionName::SQUAT:
                        // TODO:
                        return Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked;

                case Lumen::Action::ActionName::SHOOT:
                        // TODO:
                        return Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked;

                case Lumen::Action::ActionName::TOGGLE_DRAWING_BOUNDING_BOX:
                        // TODO:
                        return Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked;

                case Lumen::Action::ActionName::TOGGLE_DRAWING_TEXTURE:
                        // TODO:
                        return Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked;

                default:
                        break;
                }
                return Lumen::LayerStack::BaseLayer::DoActionResult::NotHandedOrNotBlocked;
        }

        [[nodiscard]] constexpr Lumen::LayerStack::BaseLayer::DoActionResult
        DoMovementAction([[maybe_unused]] Lumen::Action::MovementAction movement_action) noexcept override
        {
                auto &entity_manager = *this->m_game_play_scene->m_entity_manager_ptr;//std::cout << "[GameWorldLayer] " << __LINE__ << "\n";
                float speed = entity_manager.GetSpeedOfEntityCurrentlyControlledByThePlayer();//std::cout << "[GameWorldLayer] " << __LINE__ << "\n";
                auto &player = entity_manager.GetEntityCurrentlyControlledByThePlayer();//std::cout << "[GameWorldLayer] " << __LINE__ << "\n";
                auto &transform = player.GetComponent<Lumen::ECS::Component::Transform>();//std::cout << "[GameWorldLayer] " << __LINE__ << "\n";
                if (!movement_action.has_movement) {
                        transform.velocity = {0.0f, 0.0f};//std::cout << "[GameWorldLayer] " << __LINE__ << "\n";
                        return Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked;
                }
                Lumen::Core::Math::Vec2f32 velocity{0.0f, 0.0f};
                if (movement_action.is_move_up) {
                        velocity += {0.0f, -1.0f};
                }
                if (movement_action.is_move_down) {
                        velocity += {0.0f, 1.0f};
                }
                if (movement_action.is_move_left) {
                        velocity += {-1.0f, 0.0f};
                }
                if (movement_action.is_move_right) {
                        velocity += {1.0f, 0.0f};
                }
                //std::cout << "[GameWorldLayer] velocity{ " << velocity.x << ", " << velocity.y << "}\n";
                constexpr float FLOAT_ZERO = 0.001f;
                if (((velocity.x < FLOAT_ZERO) && (velocity.x > -FLOAT_ZERO)) &&
                        (velocity.y < FLOAT_ZERO) && (velocity.y > -FLOAT_ZERO)) {
                        return Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked;
                }
                
                velocity.Normalize();
                velocity *= speed;//std::cout << "[GameWorldLayer] " << __LINE__ << "\n";
                //std::cout << "[GameWorldLayer] velocity{ " << velocity.x << ", " << velocity.y << "}\n";
                transform.velocity = velocity;//std::cout << "[GameWorldLayer] " << __LINE__ << "\n";
                //std::cout << "[GameWorldLayer] " << __LINE__ << "\n";
                this->m_game_play_scene->m_view_changed = true;
                return Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked;
        }

private:
};

} // namespace GamePlayLayer
} // namespace Scene
} // namespace Lumen