
#pragma once

#include <Core/Math/Vector.hpp>

#include <cassert>

namespace Lumen {

namespace Physics {

namespace AABB {

template<typename Transform, typename BoundingBox, typename Entity>
constexpr Lumen::Core::Math::Vec2f32 GetOverlap(const Entity &entity0, const Entity &entity1) noexcept
{
        assert(entity0.template HasComponent<Transform>());
        assert(entity1.template HasComponent<Transform>());
        assert(entity0.template HasComponent<BoundingBox>());
        assert(entity1.template HasComponent<BoundingBox>());

        /* AABB collision */
        Lumen::Core::Math::Vec2f32 distance =
                entity0.template GetComponent<Transform>().position -
                entity1.template GetComponent<Transform>().position;

        distance.x = Lumen::Core::Math::Abs(distance.x);
        distance.y = Lumen::Core::Math::Abs(distance.y);

        Lumen::Core::Math::Vec2f32 boundingbox_size =
                entity0.template GetComponent<BoundingBox>().half_size +
                entity1.template GetComponent<BoundingBox>().half_size;
        return boundingbox_size - distance;
}

template<typename Transform, typename BoundingBox, typename Entity>
constexpr Lumen::Core::Math::Vec2f32 GetPrevOverlap(const Entity &entity0, const Entity &entity1) noexcept
{
        assert(entity0.template HasComponent<Transform>());
        assert(entity1.template HasComponent<Transform>());
        assert(entity0.template HasComponent<BoundingBox>());
        assert(entity1.template HasComponent<BoundingBox>());

        /* AABB collision */
        Lumen::Core::Math::Vec2f32 previous_distance =
                entity0.template GetComponent<Transform>().previous_position -
                entity1.template GetComponent<Transform>().previous_position;
        previous_distance.x = Lumen::Core::Math::Abs(previous_distance.x);
        previous_distance.y = Lumen::Core::Math::Abs(previous_distance.y);

        Lumen::Core::Math::Vec2f32 boundingbox_size =
                entity0.template GetComponent<BoundingBox>().half_size +
                entity1.template GetComponent<BoundingBox>().half_size;
        return boundingbox_size - previous_distance;
}

constexpr bool IsCollide(const Lumen::Core::Math::Vec2f32 &overlap) noexcept
{
        return (overlap.x > 0.0f) && (overlap.y > 0.0f);
}

template<typename Transform, typename BoundingBox, typename Entity>
class BasicGetOverlapFunctor {
public:
        constexpr BasicGetOverlapFunctor(void) = default;

        constexpr Lumen::Core::Math::Vec2f32 operator()(const Entity &entity0, const Entity &entity1) const noexcept
        {
                return GetOverlap<Transform, BoundingBox, Entity>(entity0, entity1);
        }
};

template<typename Transform, typename BoundingBox, typename Entity>
class BasicGetPrevOverlapFunctor {
public:
        constexpr BasicGetPrevOverlapFunctor(void) = default;

        constexpr Lumen::Core::Math::Vec2f32 operator()(const Entity &entity0, const Entity &entity1) const noexcept
        {
                return GetPrevOverlap<Transform, BoundingBox, Entity>(entity0, entity1);
        }
};

class IsCollideFunctor {
public:
        constexpr IsCollideFunctor(void) = default;

        constexpr bool operator()(const Lumen::Core::Math::Vec2f32 &overlap) const noexcept
        {
                return IsCollide(overlap);
        }
};

} // namespace AABB

} // namespace Physics

} // namespace Lumen
