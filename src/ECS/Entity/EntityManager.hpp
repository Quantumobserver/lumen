
#pragma once

#include "Entity.hpp"

#include <cstdlib>
#include <array>
#include <atomic>
#include <algorithm>
#include <memory>
#include <vector>
#include <utility>

namespace Lumen {
namespace ECS {
namespace Entity {

class EntityManager {
public:
        using entity_type = Lumen::ECS::Entity::Entity;
        using entity_vector_type = std::vector<entity_type>;
        using entity_id_type = typename entity_type::entity_id_type;

private:

        std::array<entity_vector_type,
                   static_cast<std::size_t>(entity_type::TagType::NUMBER_OF_TAGS)>
                   m_entities;
        entity_vector_type m_going_to_be_added;
        std::atomic<std::size_t> m_total_number_of_entities_created;

        // player or vehicle(TODO)
        struct EntityCurrentlyControlledByThePlayer {
                entity_type::TagType tag;
                entity_id_type id;

                constexpr EntityCurrentlyControlledByThePlayer(void) noexcept
                : tag{entity_type::TagType::PLAYER}, id{0} {}
        } m_entity_currently_controlled_by_the_player;

        using this_t = EntityManager;
public:
        constexpr EntityManager(void) noexcept : m_entities{}, m_total_number_of_entities_created{0}
        {}

private:

        static constexpr void RemoveDeadEntity(entity_vector_type &entity_vector) noexcept
        {
                entity_vector
                        .erase(std::remove_if(entity_vector.begin(),
                                       entity_vector.end(),
                                       [] (entity_type &entity_ref) {
                                                return !entity_ref.IsAlive();
                                       }),
                               entity_vector.end());
        }

        constexpr void RemoveDeadEntities(void) noexcept
        {
                for (auto &entity_vector : this->m_entities) {
                        this_t::RemoveDeadEntity(entity_vector);
                }
        }

        constexpr void AddGoingToBeAddedToEntities(void) noexcept
        {
                for (auto &&entity : this->m_going_to_be_added) {
                        std::size_t index = static_cast<std::size_t>(entity.GetTag());
                        assert(index < this->m_entities.size());
                        this->m_entities[index].push_back(std::move(entity));
                }
                this->m_going_to_be_added.clear();
        }

public:
        
        constexpr void Clear(void) noexcept
        {
                for (auto &entityVector : this->m_entities) {
                        entityVector.clear();
                }
                this->m_going_to_be_added.clear();
                this->m_total_number_of_entities_created = 0;
        }

        constexpr void Update(void) noexcept
        {
                this->RemoveDeadEntities();
                this->AddGoingToBeAddedToEntities();
        }

        // the reference can no longer be used after Updata() is called
        constexpr entity_type &CreateEntity(const entity_type::TagType tag) noexcept
        {
                const std::size_t entity_id = this->m_total_number_of_entities_created++;
                this->m_going_to_be_added.emplace_back(
                        entity_type{entity_id, tag});

                return this->m_going_to_be_added.back();
        }

        constexpr entity_vector_type &GetEntityVector(const entity_type::TagType tag) noexcept
        {
                std::size_t index = static_cast<std::size_t>(tag);
                assert(index < this->m_entities.size());
                return this->m_entities[index];
        }

        constexpr const entity_vector_type &GetEntityVector(const entity_type::TagType tag) const noexcept
        {
                std::size_t index = static_cast<std::size_t>(tag);
                assert(index < this->m_entities.size());
                return this->m_entities[index];
        }

        constexpr auto GetAllEntities(void) const noexcept
        -> typename std::add_lvalue_reference<typename std::add_const<decltype(this->m_entities)>::type>::type
        {
                return this->m_entities;
        }

        constexpr auto GetAllEntities(void) noexcept
        -> typename std::add_lvalue_reference<decltype(this->m_entities)>::type
        {
                return this->m_entities;
        }

        // TODO: vehicle support
        constexpr Entity &GetEntityCurrentlyControlledByThePlayer(void) noexcept
        {
                for (auto &entity : this->m_entities[static_cast<std::size_t>(entity_type::TagType::PLAYER)]) {
                        if (this->m_entity_currently_controlled_by_the_player.id == entity.m_id) {
                                return entity;
                        }
                }
                std::abort();
        }

        // TODO: vehicle support
        constexpr const Entity &GetEntityCurrentlyControlledByThePlayer(void) const noexcept
        {
                for (auto &entity : this->m_entities[static_cast<std::size_t>(entity_type::TagType::PLAYER)]) {
                        if (this->m_entity_currently_controlled_by_the_player.id == entity.m_id) {
                                return entity;
                        }
                }
                std::abort();
        }
};

} // namespace Entity
} // namespace ECS
} // namespace Lumen
