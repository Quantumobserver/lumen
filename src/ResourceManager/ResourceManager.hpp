#pragma once

#include <LumenDef/ConstexprIf.hpp>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "ResourceDef.hpp"

#include <cassert>
#include <vector>
#include <utility>
#include <string>

namespace Lumen {
namespace ResourceManager {

class ResourceManager {
private:
        bool m_is_initialized{false};
        std::vector<sf::Texture> m_textures;
        //std::vector<sf::Font> m_fonts;
        std::vector<sf::Sprite> m_sprites;
public:
        CONSTEXPR_IF_CXX_20 ResourceManager(void) noexcept = default;
        constexpr ResourceManager(const ResourceManager &) noexcept = delete;
        CONSTEXPR_IF_CXX_20 ResourceManager(ResourceManager &&other) noexcept
         : m_textures{std::move(other.m_textures)} {}

        constexpr void Init(void) noexcept
        {
                if (this->m_is_initialized) {
                        return;
                }
                this->m_textures.resize(static_cast<std::size_t>(Lumen::ResourceManager::TextureID::NUMBER_OF_TEXTURES));
                this->m_sprites.resize(static_cast<std::size_t>(Lumen::ResourceManager::SpriteID::NUMBER_OF_SPRITES));
                this->m_is_initialized = true;
        }

        constexpr ResourceManager &operator=(const ResourceManager &) noexcept = delete;
        CONSTEXPR_IF_STD_VECTOR_MOVE_ASSIGNMENT
        ResourceManager &operator=(ResourceManager &&other) noexcept
        {
                this->m_textures = std::move(other.m_textures);
                return *this;
        }

        CONSTEXPR_IF_SF_TEXTURE_MOVE_ASSIGNMENT
        void AddTexture(sf::Texture &&texture, Lumen::ResourceManager::TextureID texture_id) noexcept
        {
                assert(this->m_is_initialized);
                this->m_textures[static_cast<std::size_t>(texture_id)] = std::move(texture);
        }

        CONSTEXPR_IF_CXX_20
        const sf::Texture &GetTexture(const Lumen::ResourceManager::TextureID texture_id) const noexcept
        {
                assert(this->m_is_initialized);
                return this->m_textures[static_cast<std::size_t>(texture_id)];
        }

        CONSTEXPR_IF_CXX_20
        sf::Texture &GetTexture(const Lumen::ResourceManager::TextureID texture_id) noexcept
        {
                assert(this->m_is_initialized);
                return this->m_textures[static_cast<std::size_t>(texture_id)];
        }

        CONSTEXPR_IF_SF_TEXTURE_DEFAULT_CONSTRUCTOR
        void LoadTextureFromFile(const std::string &file_name, Lumen::ResourceManager::TextureID texture_id)
        {
                assert(this->m_is_initialized);
                sf::Texture texture;
                bool is_succeed = texture.loadFromFile(file_name);
                assert(is_succeed);
                this->AddTexture(std::move(texture), texture_id);
        }

        CONSTEXPR_IF_SF_SPRITE_MOVE_ASSIGNMENT
        void AddSprite(sf::Sprite &&sprite, Lumen::ResourceManager::SpriteID sprite_id) noexcept
        {
                assert(this->m_is_initialized);
                this->m_sprites[static_cast<std::size_t>(sprite_id)] = std::move(sprite);
        }

        CONSTEXPR_IF_CXX_20
        const sf::Sprite &GetSprite(const Lumen::ResourceManager::SpriteID sprite_id) const noexcept
        {
                assert(this->m_is_initialized);
                return this->m_sprites[static_cast<std::size_t>(sprite_id)];
        }

        CONSTEXPR_IF_CXX_20
        sf::Sprite &GetSprite(const Lumen::ResourceManager::SpriteID sprite_id) noexcept
        {
                assert(this->m_is_initialized);
                return this->m_sprites[static_cast<std::size_t>(sprite_id)];
        }

        // ~ResourceManager();
};


} // namespace ResourceManager
} // namespace Lumen