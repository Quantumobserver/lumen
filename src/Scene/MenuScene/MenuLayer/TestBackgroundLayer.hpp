
#pragma once

#include <Scene/Scene.hpp>
#include <ResourceManager/ResourceManager.hpp>
#include <UI/UIManager.hpp>


#include <SFML/Graphics/Sprite.hpp>

#include <cassert>
#include <iostream>

namespace Lumen {
namespace Scene {
namespace MenuLayer {

class TestBackgroundLayer : public Lumen::LayerStack::BaseLayer {
        private: 
        Lumen::Scene::BaseScene *m_menu_scene;
        sf::Sprite m_background_sprite;

public:
        constexpr TestBackgroundLayer(Lumen::Scene::BaseScene *menu_scene) noexcept
        : m_menu_scene{menu_scene},
          m_background_sprite{this->m_menu_scene->m_resource_manager_ptr->GetSprite(Lumen::ResourceManager::SpriteID::MENU_SCENCE_BACKGROUND_SPRITE)}
        {
                std::cout << "[TestBackgroundLayer] Constructor\n";
                this->ComputeBackgroundSize();
        }

        constexpr void Update(float delta_time) noexcept override
        {
                (void)delta_time;
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

        constexpr void Render(void) noexcept override
        {
                this->m_menu_scene->m_window_ptr->draw(this->m_background_sprite);
        }

        constexpr void ComputeBackgroundSize(void) noexcept
        {
                this->m_background_sprite.setPosition({0, 0});
                auto window_size = this->m_menu_scene->m_window_ptr->getSize();
                this->m_background_sprite.setScale(
                        {static_cast<float>(window_size.x) / static_cast<float>(this->m_background_sprite.getLocalBounds().size.x),
                        static_cast<float>(window_size.y) / static_cast<float>(this->m_background_sprite.getLocalBounds().size.y)}
                );
        }

        [[nodiscard]] constexpr Lumen::LayerStack::BaseLayer::DoActionResult DoWindowResizeAction(void) noexcept override
        {
                auto view = this->m_menu_scene->m_window_ptr->getView();
                view.setSize({static_cast<float>(this->m_menu_scene->m_window_ptr->getSize().x),
                             static_cast<float>(this->m_menu_scene->m_window_ptr->getSize().y)});
                view.setCenter({view.getSize().x / 2.0f, view.getSize().y / 2.0f});
                this->m_menu_scene->m_window_ptr->setView(view);
                this->ComputeBackgroundSize();
                return Lumen::LayerStack::BaseLayer::DoActionResult::NotHandledOrNotBlocked;
        }

};


} // namespace MenuLayer
} // namespace Scene
} // namespace Lumen