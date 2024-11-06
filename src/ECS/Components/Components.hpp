
#pragma once

#include <Core/Math/Vector.hpp>

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
        Lumen::Core::Math::Vec2f32 size;
        Lumen::Core::Math::Vec2f32 half_size;

        constexpr BoundingBox(void) noexcept {}
        constexpr BoundingBox(const Lumen::Core::Math::Vec2f32 &size) noexcept : size{size}, half_size{size / 2.0f} {}
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
        std::size_t remaining_lifespan;
};

//#include <iostream>
/*template<typename Rectangle, typename Sprite>
class BasicAnimation {
private:
        std::size_t m_numberOfFramesThisAnimationHas;
        std::size_t m_numberOfFramesThisEntityHasLivedInGame;
        std::size_t m_frameDuration;
        Vec2f32 m_size;
        Sprite m_sprite;
        bool m_isNonRepeatingAnimation;

public:
        constexpr BasicAnimation(void) noexcept
        : m_numberOfFramesThisAnimationHas{0}, m_numberOfFramesThisEntityHasLivedInGame{0},
          m_frameDuration{0}, m_isNonRepeatingAnimation{false} {}
        constexpr BasicAnimation(
                std::size_t numberOfFramesThisAnimationHas,
                std::size_t frameDuration,
                Vec2f32 size,
                const Sprite &sprite,
                bool isNonRepeatingAnimation) noexcept
        : m_numberOfFramesThisAnimationHas{numberOfFramesThisAnimationHas},
          m_numberOfFramesThisEntityHasLivedInGame{0},
          m_frameDuration{frameDuration}, m_size{size}, m_sprite{sprite},
          m_isNonRepeatingAnimation{isNonRepeatingAnimation}
        {
                assert(0 != this->m_frameDuration);
                const auto &texture = this->m_sprite.getTexture();
                const auto textureSize = texture.getSize();
                const float animationSizeX = static_cast<float>(textureSize.x / this->m_numberOfFramesThisAnimationHas);
                const float scaleFactorX = this->m_size.x / animationSizeX;
                const float scaleFactorY = this->m_size.y / static_cast<float>(textureSize.y);
                this->m_sprite.setScale({scaleFactorX, scaleFactorY});
                this->m_sprite.setOrigin({animationSizeX / 2.0f, static_cast<float>(textureSize.y) / 2.0f});
        }

        constexpr void ChangeAnimation(const BasicAnimation &animation) noexcept
        {
                this->m_numberOfFramesThisAnimationHas = animation.m_numberOfFramesThisAnimationHas;
                this->m_frameDuration = animation.m_frameDuration;
                this->m_size = animation.m_size;

                //auto facingDirection = this->m_sprite.getScale();
                //facingDirection.x = (facingDirection.x < 0.0f) ? -1.0f : 1.0f;
                //facingDirection.y = (facingDirection.y < 0.0f) ? -1.0f : 1.0f;
                this->m_sprite = animation.m_sprite;
                //auto scale = this->m_sprite.getScale();
                //scale.x *= facingDirection.x;
                //scale.y *= facingDirection.y;
                //this->m_sprite.setScale(scale);

                this->m_isNonRepeatingAnimation = animation.m_isNonRepeatingAnimation;
                assert(0 != this->m_frameDuration);
        }

        constexpr void ResetNumberOfFramesThisEntityHasLivedInGame(void) noexcept
        {
                this->m_numberOfFramesThisEntityHasLivedInGame = 0;
        }

        constexpr std::size_t GetAnimationFrame(void) const noexcept
        {
                assert(0 != this->m_frameDuration);
//if (dbg_print) {std::cout << "animationFrame: " << (this->m_numberOfFramesThisEntityHasLivedInGame / this->m_frameDuration) % this->m_numberOfFramesThisAnimationHas << " numberOfFramesThisEntityHasLivedInGame: " << this->m_numberOfFramesThisEntityHasLivedInGame << " frameDuration: " << this->m_frameDuration << " numberOfFramesThisAnimationHas: " << this->m_numberOfFramesThisAnimationHas << '\n'; }
                return (this->m_numberOfFramesThisEntityHasLivedInGame /
                        this->m_frameDuration) %
                       this->m_numberOfFramesThisAnimationHas;
        }
        constexpr void Update(void) noexcept
        {
                ++this->m_numberOfFramesThisEntityHasLivedInGame;
                const std::size_t animationFrame = this->GetAnimationFrame();
                const auto &texture = this->m_sprite.getTexture();
                const auto &textureSize = texture.getSize();
                const auto framSizeX = textureSize.x / this->m_numberOfFramesThisAnimationHas;
                Rectangle rectangleToDraw{
                        { static_cast<int>(animationFrame * framSizeX), 0, },
                        { static_cast<int>(framSizeX), static_cast<int>(textureSize.y), },
                };
                m_sprite.setTextureRect(rectangleToDraw);
        }

        constexpr void SetPosition(float x, float y) noexcept
        {
                this->m_sprite.setPosition({x, y});
        }

        constexpr void SetPosition(const Vec2f32 &position) noexcept
        {
                this->m_sprite.setPosition(position);
        }

        constexpr void SetScale(const Vec2f32 &factor) noexcept
        {
                this->m_sprite.setScale(factor);
        }

        constexpr void ClearSprite(void) noexcept
        {
                //auto color = this->m_sprite.getColor();
                //color.a = 0;
                //this->m_sprite.setColor(color);
                this->m_sprite.setScale({0.0f, 0.0f});
        }

        constexpr Vec2f32 GetScale(void) const noexcept
        {
                const auto scaleFactor = this->m_sprite.getScale();
                return Vec2f32{scaleFactor.x, scaleFactor.y};
        }

        constexpr void SetSprite(const Sprite &sprite) noexcept
        {
                this->m_sprite = sprite;
        }

        constexpr void SetSize(const Vec2f32 &size) noexcept
        {
                this->m_size = size;
        }

        constexpr const Sprite &GetSprite(void) const noexcept
        {
                return this->m_sprite;
        }

        constexpr const Vec2f32 &GetSize(void) const noexcept
        {
                return this->m_size;
        }

        [[nodiscard]] constexpr bool IsLastAnimationFrame(void) const noexcept
        {
                const std::size_t currentAnimationFrame = this->GetAnimationFrame() + 1;
//std::cout << "currentAnimationFrame: " << currentAnimationFrame << "this->m_numberOfFramesThisAnimationHas: " << this->m_numberOfFramesThisAnimationHas << "\n";
                return currentAnimationFrame == this->m_numberOfFramesThisAnimationHas;
        }

        [[nodiscard]] constexpr bool IsNextUpdateLastAnimationFrame(void) const noexcept
        {
                const std::size_t currentAnimationFrame = this->GetNextAnimationFrame() + 1;
                return currentAnimationFrame == this->m_numberOfFramesThisAnimationHas;
        }

        [[nodiscard]] constexpr bool HasEnded(void) const noexcept
        {
//std::cout << "this->m_isNonRepeatingAnimation: " << this->m_isNonRepeatingAnimation << "\n";
                return this->m_isNonRepeatingAnimation && this->IsLastAnimationFrame();
        }

private:
        constexpr std::size_t GetNextAnimationFrame(void) const noexcept
        {
                assert(0 != this->m_frameDuration);
                return ((this->m_numberOfFramesThisEntityHasLivedInGame + 1) /
                        this->m_frameDuration) %
                       this->m_numberOfFramesThisAnimationHas;
        }
};*/
//bool dbg_print = false;

} // namespace Component

} // namespace Lumen

} // namespace ECS
