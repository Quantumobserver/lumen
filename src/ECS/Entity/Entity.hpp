
#pragma once

#include <ECS/Components/Components.hpp>
//#include <Asset/AssertId.hpp>

#include <cstddef>
#include <bitset>
#include <tuple>
#include <utility>
#include <type_traits>

//#include <typeinfo>
//#include <iostream>
//using Animation = BasicAnimation<sf::IntRect, sf::Sprite>;

namespace Lumen {
namespace ECS {
namespace Entity {

namespace EntityInternal {

template<typename ...Components>
class ComponentRecord {
        std::bitset<sizeof...(Components)> m_bitset;

        using this_t = ComponentRecord;

        template<std::size_t Index, typename Component, bool, typename Frist, typename ...Others>
        struct GetComponentIndexImpl;

        template<std::size_t Index, typename Component, typename Frist, typename ...Others>
        struct GetComponentIndexImpl<Index, Component, true, Frist, Others...> {
                static constexpr std::size_t index = Index;
        };

        template<std::size_t Index, typename Component, typename Frist, typename ...Others>
        struct GetComponentIndexImpl<Index, Component, false, Frist, Others...> {
                static constexpr std::size_t index = GetComponentIndexImpl<Index + 1, Component, std::is_same<Component, Frist>::value, Others...>::index;
        };

        template<typename Component, typename Frist, typename ...Others>
        static constexpr std::size_t GetComponentIndex(void) noexcept
        {
                struct Last {};
                return GetComponentIndexImpl<0, Component, std::is_same<Component, Frist>::value, Others..., Last>::index;
        }

public:
        constexpr ComponentRecord(void) noexcept : m_bitset{false} {}
        constexpr ComponentRecord(const ComponentRecord &other) noexcept = default;

        constexpr ComponentRecord &operator=(const ComponentRecord &other) noexcept = default;

        template<typename Component>
        static constexpr std::size_t GetComponentIndex(void) noexcept
        {
                return this_t::GetComponentIndex<Component, Components...>();
        }

        static constexpr std::size_t NumberOfComponents(void) noexcept
        {
                return sizeof...(Components);
        }

        template<typename Component>
        constexpr bool HasComponent(void) const noexcept
        {
                constexpr const std::size_t index = this_t::GetComponentIndex<Component>();
                return this->m_bitset[index];
        }

        template<typename Component>
        constexpr void AddComponent(void) noexcept
        {
                constexpr const std::size_t index = this_t::GetComponentIndex<Component>();
                this->m_bitset.set(index, true);
        }

        template<typename Component>
        constexpr void RemoveComponent(void) noexcept
        {
                constexpr const std::size_t index = this_t::GetComponentIndex<Component>();
                this->m_bitset.set(index, false);
        }
};

} // namespace EntityInternal

class EntityManager;

template<typename ...Components>
class BasicEntity {
private:
        friend EntityManager;

        bool m_is_alive;

public:
        enum class TagType {
                PLAYER,
                ENEMY,
                BULLET,
                TILE, /* brick_tile, question_tile */
                DECORATION,
                NUMBER_OF_TAGS,
        };
        //using AnimationType = AnimationTypeInternal;
        using entity_id_type = std::size_t;

private:
        entity_id_type m_id;
        TagType m_tag;
        //AnimationType m_animation_type;

        std::tuple<Components...> m_components;

        EntityInternal::ComponentRecord<Components...> m_component_record;

private:
        constexpr BasicEntity(void) noexcept = delete;
        constexpr BasicEntity(const entity_id_type id, const enum TagType tag //,
                         //const AnimationType animation_type
                         ) noexcept
        : m_is_alive{true}, m_id{id}, m_tag{tag}//, m_animation_type{animation_type}
        {}

public:

        constexpr BasicEntity(const BasicEntity &) noexcept = delete;
        constexpr BasicEntity &operator=(const BasicEntity &) noexcept = delete;

        constexpr BasicEntity(BasicEntity &&other) noexcept
        : m_is_alive{other.m_is_alive}, m_id{other.m_id}, m_tag{other.m_tag},
          //m_animation_type{other.m_animation_type},
          m_components{std::move(other.m_components)},
          m_component_record{other.m_component_record}
        {
                other.m_is_alive = false;
        }

        constexpr BasicEntity &operator=(BasicEntity &&other) noexcept
        {
                if (this == &other) {
                        return *this;
                }
                this->m_is_alive = other.m_is_alive;
                other.m_is_alive = false;
                this->m_id = other.m_id;
                this->m_tag = other.m_tag;
                //this->m_animation_type = other.m_animation_type;
                this->m_components = std::move(other.m_components);
                this->m_component_record = other.m_component_record;
                return *this;
        }

        constexpr entity_id_type GetId(void) const noexcept
        {
                return this->m_id;
        }
        constexpr BasicEntity::TagType GetTag(void) const noexcept
        {
                return this->m_tag;
        }
        /*constexpr Asset::AnimationType GetAnimationType(void) const noexcept
        {
                return this->m_animation_type;
        }*/

        constexpr void Destroy(void) noexcept
        {
                this->m_is_alive = false;
        }

        constexpr bool IsAlive(void) const noexcept
        {
                return this->m_is_alive;
        }

        template<typename T>
        constexpr typename std::add_lvalue_reference<T>::type
        GetComponent(void) noexcept
        {
                assert(this->HasComponent<T>());
                return this->GetInternalComponentMutableRefNoCheck<T>();
        }

        template<typename T>
        constexpr typename std::add_lvalue_reference<typename std::add_const<T>::type>::type
        GetComponent(void) const noexcept
        {
                assert(this->HasComponent<T>());
                return this->GetInternalComponentRefNoCheck<T>();
        }

        template<typename T>
        constexpr bool HasComponent(void) const noexcept
        {
                return this->m_component_record.template HasComponent<T>();
        }

        template<typename T, typename ...Args>
        constexpr T &AddComponent(Args &&...args) noexcept
        {
                auto &component = this->GetInternalComponentMutableRefNoCheck<T>();
                component = T{std::forward<Args>(args)...};
                this->m_component_record.template AddComponent<T>();
                return component;
        }

        template<typename T>
        constexpr T &AddComponent(T &&other) noexcept
        {
                auto &component = this->GetInternalComponentMutableRefNoCheck<T>();
                component = std::forward<T>(other);
                this->m_component_record.template AddComponent<T>();
                return component;
        }

        template<typename T>
        constexpr void RemoveComponent(void) noexcept
        {
                this->m_component_record.template RemoveComponent<T>();
        }

private:
        template<typename T>
        constexpr typename std::add_lvalue_reference<T>::type
        GetInternalComponentMutableRefNoCheck(void) noexcept
        {
                return std::get<T>(this->m_components);
        }

        template<typename T>
        constexpr typename std::add_lvalue_reference<typename std::add_const<T>::type>::type
        GetInternalComponentRefNoCheck(void) const noexcept
        {
                return std::get<T>(this->m_components);
        }
};

using Entity = BasicEntity<Lumen::ECS::Component::HealthPoint,
                           Lumen::ECS::Component::DefensePoint,
                           Lumen::ECS::Component::Transform,
                           Lumen::ECS::Component::BoundingBox,
                           Lumen::ECS::Component::Gravity,
                           Lumen::ECS::Component::State,
                           Lumen::ECS::Component::Score,
                           Lumen::ECS::Component::Lifespan>;

} // namespace Entity
} // namespace ECS
} // namespace Lumen
