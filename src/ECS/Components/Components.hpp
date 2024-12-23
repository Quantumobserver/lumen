
#pragma once

#include <LumenDef/ConstexprIf.hpp>
#include <Core/Math/Vector.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <cassert>
#include <cstddef>

namespace Lumen {

namespace ECS {

namespace Component {

struct HealthPoint {
        using health_point_type = std::int32_t;

        health_point_type current_health_point;
        health_point_type max_health_point;
};

struct DefensePoint {
        using defense_point_type = std::int32_t;
        
        defense_point_type current_defense_point;
        defense_point_type max_defense_point;
};

struct Transform {
        Lumen::Core::Math::Vec2f32 position;
        Lumen::Core::Math::Vec2f32 previous_position;
        Lumen::Core::Math::Vec2f32 scale;
        Lumen::Core::Math::Vec2f32 velocity;
        //Vec2f32 animationSize;

        constexpr Transform(void) noexcept {}
        constexpr Transform(const Lumen::Core::Math::Vec2f32 &position,
                            const Lumen::Core::Math::Vec2f32 &scale,
                            const Lumen::Core::Math::Vec2f32 &velocity//,
                            //const Vec2f32 &animationSize
                            ) noexcept
        : position{position}, previous_position{position},
          scale{scale}, velocity{velocity}//, animationSize{animationSize}
          {}
        constexpr Transform(const Transform &other) noexcept = default;
        constexpr Transform &operator=(const Transform &other) noexcept = default;
};

struct BoundingBox {
        struct BlockVision {
                bool value;
        };
        struct BlockMovement {
                bool value;
        };
        Lumen::Core::Math::Vec2f32 size;
        Lumen::Core::Math::Vec2f32 half_size;
        bool block_vision : 1;
        bool block_movement : 1;

        constexpr BoundingBox(void) noexcept : block_vision{false}, block_movement{false} {}
        constexpr BoundingBox(const Lumen::Core::Math::Vec2f32 &size,
                              BlockVision block_vision = BlockVision{true}, BlockMovement block_movement = BlockMovement{true}) noexcept
        : size{size}, half_size{size / 2.0f}, block_vision{block_vision.value}, block_movement{block_movement.value} {}
        constexpr BoundingBox(const BoundingBox &other) noexcept = default;
        constexpr BoundingBox &operator=(const BoundingBox &other) noexcept = default;
};

struct Gravity {
        float gravitational_acceleration;
};

struct State {
        enum class StateCode {
                NONE,

                PLAYER_STAND,
                PLAYER_RUN,
                PLAYER_IN_THE_AIR,

                EXPLORATION,
        };

        StateCode state;

        constexpr State(void) noexcept : state{StateCode::NONE} {}
};

struct Score {
        std::size_t value;
};

struct Lifespan {
        float remaining_lifespan;

        constexpr bool ShouldBeDestroyed(void) noexcept
        {
                return this->remaining_lifespan < 0.0f;
        }
};

//#include <iostream>
class Animation {
private:
        std::size_t m_number_of_frames_this_animation_has;
        float m_the_time_of_this_entity_has_lived_in_game;
        float m_frame_duration;
        Lumen::Core::Math::Vec2f32 m_size;
        sf::Sprite m_sprite;
        bool m_is_non_repeating_animation;

public:
        CONSTEXPR_IF_SF_SPRITE_DEFAULT_CONSTRUCTOR Animation(void) noexcept
        : m_number_of_frames_this_animation_has{0}, m_the_time_of_this_entity_has_lived_in_game{0},
          m_frame_duration{0.0f}, m_is_non_repeating_animation{false} {}

        CONSTEXPR_IF_SF_SPRITE_GET_TEXTURE Animation(
                std::size_t number_of_frames_this_animation_has,
                float frame_duration,
                Lumen::Core::Math::Vec2f32 size,
                const sf::Sprite &sprite,
                bool is_non_repeating_animation) noexcept
        : m_number_of_frames_this_animation_has{number_of_frames_this_animation_has},
          m_the_time_of_this_entity_has_lived_in_game{0.0f},
          m_frame_duration{frame_duration}, m_size{size}, m_sprite{sprite},
          m_is_non_repeating_animation{is_non_repeating_animation}
        {
                assert(this->m_frame_duration > 0.0f);
                const sf::Texture &texture = this->m_sprite.getTexture();
                const auto texture_size = texture.getSize();
                const float animation_size_x = static_cast<float>(texture_size.x / this->m_number_of_frames_this_animation_has);
                const float scale_factor_x = this->m_size.x / animation_size_x;
                const float scale_factor_y = this->m_size.y / static_cast<float>(texture_size.y);
                this->m_sprite.setScale({scale_factor_x, scale_factor_y});
                this->m_sprite.setOrigin({animation_size_x / 2.0f, static_cast<float>(texture_size.y) / 2.0f});
        }

        constexpr void ChangeAnimation(const Animation &animation) noexcept
        {
                this->m_number_of_frames_this_animation_has = animation.m_number_of_frames_this_animation_has;
                this->m_frame_duration = animation.m_frame_duration;
                this->m_size = animation.m_size;

                //auto facingDirection = this->m_sprite.getScale();
                //facingDirection.x = (facingDirection.x < 0.0f) ? -1.0f : 1.0f;
                //facingDirection.y = (facingDirection.y < 0.0f) ? -1.0f : 1.0f;
                this->m_sprite = animation.m_sprite;
                //auto scale = this->m_sprite.getScale();
                //scale.x *= facingDirection.x;
                //scale.y *= facingDirection.y;
                //this->m_sprite.setScale(scale);

                this->m_is_non_repeating_animation = animation.m_is_non_repeating_animation;
                assert(this->m_frame_duration > 0.0f);
        }

        constexpr void ResetNumberOfFramesThisEntityHasLivedInGame(void) noexcept
        {
                this->m_the_time_of_this_entity_has_lived_in_game = 0;
        }

        constexpr std::size_t GetAnimationFrame(void) const noexcept
        {
                assert(this->m_frame_duration > 0.0f);
//if (dbg_print) {std::cout << "animation_frame: " << (this->m_the_time_of_this_entity_has_lived_in_game / this->m_frame_duration) % this->m_number_of_frames_this_animation_has << " numberOfFramesThisEntityHasLivedInGame: " << this->m_the_time_of_this_entity_has_lived_in_game << " frame_duration: " << this->m_frame_duration << " number_of_frames_this_animation_has: " << this->m_number_of_frames_this_animation_has << '\n'; }
                return static_cast<std::size_t>(
                        this->m_the_time_of_this_entity_has_lived_in_game /
                        this->m_frame_duration) %
                       this->m_number_of_frames_this_animation_has;
        }

        CONSTEXPR_IF_SF_SPRITE_GET_TEXTURE void Update(float delta_time) noexcept
        {
                this->m_the_time_of_this_entity_has_lived_in_game += delta_time;
                const std::size_t animation_frame = this->GetAnimationFrame();
                const sf::Texture &texture = this->m_sprite.getTexture();
                const sf::Vector2u &texture_size = texture.getSize();
                const auto fram_size_x = texture_size.x / this->m_number_of_frames_this_animation_has;
                sf::IntRect rectangle_to_draw{
                        { static_cast<int>(animation_frame * fram_size_x), 0, },
                        { static_cast<int>(fram_size_x), static_cast<int>(texture_size.y), },
                };
                m_sprite.setTextureRect(rectangle_to_draw);
        }

        CONSTEXPR_IF_SF_SPRITE_SET_TEXTURE void SetPosition(float x, float y) noexcept
        {
                this->m_sprite.setPosition({x, y});
        }

        CONSTEXPR_IF_SF_SPRITE_SET_POSITION void SetPosition(const Lumen::Core::Math::Vec2f32 &position) noexcept
        {
                this->m_sprite.setPosition({position.x, position.y});
        }

        CONSTEXPR_IF_SF_SPRITE_SET_SCALE void SetScale(const Lumen::Core::Math::Vec2f32 &factor) noexcept
        {
                this->m_sprite.setScale({factor.x, factor.y});
        }

        CONSTEXPR_IF_SF_SPRITE_SET_SCALE void ClearSprite(void) noexcept
        {
                //auto color = this->m_sprite.getColor();
                //color.a = 0;
                //this->m_sprite.setColor(color);
                this->m_sprite.setScale({0.0f, 0.0f});
        }

        CONSTEXPR_IF_SF_SPRITE_GET_SCALE Lumen::Core::Math::Vec2f32 GetScale(void) const noexcept
        {
                const auto scale_factor = this->m_sprite.getScale();
                return {scale_factor.x, scale_factor.y};
        }

        constexpr void SetSprite(const sf::Sprite &sprite) noexcept
        {
                this->m_sprite = sprite;
        }

        constexpr void SetSize(const Lumen::Core::Math::Vec2f32 &size) noexcept
        {
                this->m_size = size;
        }

        constexpr const sf::Sprite &GetSprite(void) const noexcept
        {
                return this->m_sprite;
        }

        constexpr const Lumen::Core::Math::Vec2f32 &GetSize(void) const noexcept
        {
                return this->m_size;
        }

        [[nodiscard]] constexpr bool IsLastAnimationFrame(void) const noexcept
        {
                const std::size_t current_animation_frame = this->GetAnimationFrame() + 1;
//std::cout << "current_animation_frame: " << current_animation_frame << "this->m_number_of_frames_this_animation_has: " << this->m_number_of_frames_this_animation_has << "\n";
                return current_animation_frame == this->m_number_of_frames_this_animation_has;
        }

        [[nodiscard]] constexpr bool IsNextUpdateLastAnimationFrame(void) const noexcept
        {
                const std::size_t current_animation_frame = this->GetNextAnimationFrame() + 1;
                return current_animation_frame == this->m_number_of_frames_this_animation_has;
        }

        [[nodiscard]] constexpr bool HasEnded(void) const noexcept
        {
//std::cout << "this->m_is_non_repeating_animation: " << this->m_is_non_repeating_animation << "\n";
                return this->m_is_non_repeating_animation && this->IsLastAnimationFrame();
        }

private:
        constexpr std::size_t GetNextAnimationFrame(void) const noexcept
        {
                assert(0 != this->m_frame_duration);
                return static_cast<std::size_t>(
                        (this->m_the_time_of_this_entity_has_lived_in_game + this->m_frame_duration) /
                        this->m_frame_duration) %
                       this->m_number_of_frames_this_animation_has;
        }
};
//bool dbg_print = false;

} // namespace Component

} // namespace Lumen

} // namespace ECS
