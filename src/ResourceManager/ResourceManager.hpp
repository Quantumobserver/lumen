#pragma once

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
        constexpr ResourceManager(void) noexcept = default;
        constexpr ResourceManager(const ResourceManager &) noexcept = delete;
        constexpr ResourceManager(ResourceManager &&other) noexcept : m_textures{std::move(other.m_textures)} {}

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
        constexpr ResourceManager &operator=(ResourceManager &&other) noexcept
        {
                this->m_textures = std::move(other.m_textures);
                return *this;
        }

        constexpr void AddTexture(sf::Texture &&texture, Lumen::ResourceManager::TextureID texture_id) noexcept
        {
                assert(this->m_is_initialized);
                this->m_textures[static_cast<std::size_t>(texture_id)] = std::move(texture);
        }

        constexpr const sf::Texture &GetTexture(const Lumen::ResourceManager::TextureID texture_id) const noexcept
        {
                assert(this->m_is_initialized);
                return this->m_textures[static_cast<std::size_t>(texture_id)];
        }

        constexpr sf::Texture &GetTexture(const Lumen::ResourceManager::TextureID texture_id) noexcept
        {
                assert(this->m_is_initialized);
                return this->m_textures[static_cast<std::size_t>(texture_id)];
        }

        constexpr void LoadTextureFromFile(const std::string &file_name, Lumen::ResourceManager::TextureID texture_id)
        {
                assert(this->m_is_initialized);
                sf::Texture texture;
                bool is_succeed = texture.loadFromFile(file_name);
                assert(is_succeed);
                this->AddTexture(std::move(texture), texture_id);
        }

        constexpr void AddSprite(sf::Sprite &&sprite, Lumen::ResourceManager::SpriteID sprite_id) noexcept
        {
                assert(this->m_is_initialized);
                this->m_sprites[static_cast<std::size_t>(sprite_id)] = std::move(sprite);
        }

        constexpr const sf::Sprite &GetSprite(const Lumen::ResourceManager::SpriteID sprite_id) const noexcept
        {
                assert(this->m_is_initialized);
                return this->m_sprites[static_cast<std::size_t>(sprite_id)];
        }

        constexpr sf::Sprite &GetSprite(const Lumen::ResourceManager::SpriteID sprite_id) noexcept
        {
                assert(this->m_is_initialized);
                return this->m_sprites[static_cast<std::size_t>(sprite_id)];
        }

        // ~ResourceManager();
};


} // namespace ResourceManager
} // namespace Lumen