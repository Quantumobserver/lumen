
#pragma once

#include "Components/BasicUIComponent.hpp"
#include "Components/Menu.hpp"
#include <ResourceManager/ResourceManager.hpp>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <array>
#include <memory>
#include <vector>

#include <iostream>

namespace Lumen {
namespace UI {

class UIManager {
private:
        struct View {
                Lumen::Core::Math::Vec2f32 size;
        } m_current_view;

        bool m_initialized;
        bool m_draw_bounding_box;
        const sf::Texture *m_ui_texture;
        const sf::Font *m_default_font;
        //const sf::Sprite *m_default_sprite;
        Lumen::UI::Component::Sprite m_default_sprite;

        sf::VertexArray m_vertex_array;
        sf::VertexArray m_vertex_array_bounding_box;
        std::vector<const sf::Text *> m_texts_needed_to_render;
        sf::Color m_bounding_box_color;
        sf::Color m_bounding_box_color_selected;
        Lumen::UI::Component::BasicUIComponent *m_selected_ui_component;

        std::vector<
                std::vector<std::unique_ptr<Lumen::UI::Component::BasicUIComponent> >
                > m_ui_components;

sf::RenderTarget *m_debug_render_target;
public:

        using DoActionResult = Lumen::UI::Component::BasicUIComponent::DoActionResult;

        constexpr UIManager(void) noexcept = default;

        constexpr UIManager(const UIManager &) noexcept = delete;
        constexpr UIManager(UIManager &&) noexcept = default;

        constexpr UIManager &operator=(const UIManager &) noexcept = delete;
        constexpr UIManager &operator=(UIManager &&) noexcept = default;

        constexpr void Init(const Lumen::ResourceManager::ResourceManager &resource_manager) noexcept
        {
                if (this->m_initialized) {
                        return;
                }

                this->m_initialized = true;

                this->m_draw_bounding_box = true;
                this->m_ui_texture = &resource_manager.GetTexture(Lumen::ResourceManager::TextureID::UI_TEXTURE);

                this->m_default_font = &resource_manager.GetFont(static_cast<Lumen::ResourceManager::FontID>(0));

                {
                        const auto default_sprite_id = static_cast<Lumen::ResourceManager::SpriteID>(0);
                        const auto texture_rect = resource_manager.GetSprite(default_sprite_id).getTextureRect();

                        this->m_default_sprite = Lumen::UI::Component::Sprite{
                                Lumen::UI::Component::AbsoluteTransformRectangleArea{
                                        Lumen::UI::Component::AbsoluteTransformTopLeft{{
                                                static_cast<float>(texture_rect.position.x),
                                                static_cast<float>(texture_rect.position.y)
                                        }},

                                        Lumen::UI::Component::BoundingBox{{
                                                static_cast<float>(texture_rect.size.x),
                                                static_cast<float>(texture_rect.size.y)
                                        }}
                                }
                        };
                }

                this->m_ui_components.resize(
                        static_cast<std::size_t>(Lumen::UI::Component::UIComponentTypeTag::NUMBER_OF_BASIC_UI_COMPONENTS));
        
                this->m_vertex_array.setPrimitiveType(sf::PrimitiveType::Triangles);
                this->m_vertex_array_bounding_box.setPrimitiveType(sf::PrimitiveType::Lines);

                this->m_bounding_box_color = sf::Color::Green;
                this->m_bounding_box_color_selected = sf::Color::Red;
                this->m_selected_ui_component = nullptr;
        }

        void Update(float delta_time) noexcept
        {
                for (auto &ui_component : this->m_ui_components) {
                        for (auto &component : ui_component) {
                                component->Update(delta_time);
                        }
                }
        }

        void Render(sf::RenderTarget &render_target) noexcept
        {
                assert(this->m_initialized);
this->m_debug_render_target = &render_target;

                this->m_vertex_array.clear();
                this->m_vertex_array.setPrimitiveType(sf::PrimitiveType::Triangles);

// std::cout << __FILE__ " :" << __LINE__ << "\n";
                if (this->m_draw_bounding_box) {
// std::cout << __FILE__ " :" << __LINE__ << "\n";
                        this->m_vertex_array_bounding_box.clear();
                        this->m_vertex_array_bounding_box.setPrimitiveType(sf::PrimitiveType::Lines);
                }

                this->m_texts_needed_to_render.clear();

// std::cout << __FILE__ " :" << __LINE__ << "\n";
                for (const auto &ui_component : this->m_ui_components) {
// std::cout << __FILE__ " :" << __LINE__ << "\n";
                        for (const auto &component : ui_component) {
// std::cout << __FILE__ " :" << __LINE__ << "\n";
                                this->RenderUIComponent(*component);
                        }
                }
// std::cout << __FILE__ " :" << __LINE__ << "\n";
                render_target.draw(this->m_vertex_array, this->m_ui_texture);
                for (const auto &text : this->m_texts_needed_to_render) {
// std::cout << __FILE__ " :" << __LINE__ << "\n";
                        render_target.draw(*text);
                }
                if (this->m_draw_bounding_box) {
// std::cout << __FILE__ " :" << __LINE__ << "\n";
                        render_target.draw(this->m_vertex_array_bounding_box);
                }
// std::cout << __FILE__ " :" << __LINE__ << "\n";
        }

        // void shutdown();
        // bool isInitialized() const;

//         constexpr DoActionResult DoAction(
//                 const Lumen::UI::Component::RelativeSelectionAction &relative_selection_action) noexcept
//         {
//                 for (auto &ui_component : this->m_ui_components) {
//                         for (auto &component : ui_component) {
// std::cout << __FILE__ " :" << __LINE__ << "\n";
//                                 if (DoActionResult::HandledOrBlocked ==
//                                     component->DoSelectionAction(relative_selection_action)) {
// std::cout << __FILE__ " :" << __LINE__ << "\n";
//                                         component->SetSelected(true);
//                                         return DoActionResult::HandledOrBlocked;
//                                 }
//                         }
//                 }
//                 return DoActionResult::NotHandledOrNotBlocked;
//         }


        constexpr void SetDrawBoundingBox(bool draw_bounding_box = true) noexcept
        {
                this->m_draw_bounding_box = draw_bounding_box;
        }

        
        constexpr const sf::Font *GetDefaultFont(void) const noexcept
        {
                return this->m_default_font;
        }

        constexpr const Lumen::UI::Component::Sprite &GetDefaultSprite(void) const noexcept
        {
                return this->m_default_sprite;
        }

        constexpr void SetDefaultFont(const sf::Font *font) noexcept
        {
                this->m_default_font = font;
        }

        constexpr void SetDefaultSprite(const Lumen::UI::Component::Sprite &sprite) noexcept
        {
                this->m_default_sprite = sprite;
        }

        template<typename ...Args>
        constexpr Lumen::UI::Component::Menu &AddMenu(Args &&...args) noexcept
        {
                assert(this->m_initialized);
                assert(this->m_ui_components.size() > static_cast<std::size_t>(Lumen::UI::Component::UIComponentTypeTag::MENU));

                const std::size_t index = static_cast<std::size_t>(Lumen::UI::Component::UIComponentTypeTag::MENU);

                auto &menus = this->m_ui_components[index];
                auto &menu = menus.emplace_back(std::make_unique<Lumen::UI::Component::Menu>(std::forward<Args>(args)...));

                return *dynamic_cast<Lumen::UI::Component::Menu *>(menu.get());
        }


        constexpr void AddMenu(std::unique_ptr<Lumen::UI::Component::Menu> &&menu) noexcept
        {
                assert(this->m_initialized);
                assert(this->m_ui_components.size() > static_cast<std::size_t>(Lumen::UI::Component::UIComponentTypeTag::MENU));

                const std::size_t index = static_cast<std::size_t>(Lumen::UI::Component::UIComponentTypeTag::MENU);

                auto &menus = this->m_ui_components[index];

                menus.push_back(std::move(menu));
        }

        constexpr Lumen::UI::Component::Menu &CreateMenu(void) noexcept
        {
                assert(this->m_initialized);
                assert(this->m_ui_components.size() > static_cast<std::size_t>(Lumen::UI::Component::UIComponentTypeTag::MENU));

                auto &menus = this->m_ui_components[static_cast<std::size_t>(Lumen::UI::Component::UIComponentTypeTag::MENU)];
                auto &menu = menus.emplace_back(std::make_unique<Lumen::UI::Component::Menu>());
// std::cout << __FILE__ " :" << __LINE__ << " : " << int(menu.get()->GetUIType()) << "\n";
                
                return *dynamic_cast<Lumen::UI::Component::Menu *>(menu.get());
        }

        constexpr void DoAction(Lumen::Action::Action action) noexcept
        {
                (void)action;
                // for (auto &ui_component : this->m_ui_components) {
                //         for (auto &component : ui_component) {
                //                 component->DoAction(action);
                //         }
                // }
        }

        constexpr DoActionResult DoSelectionAction(const Lumen::UI::Component::RelativeSelectionAction &relative_selection_action) noexcept
        {
                for (auto &ui_component : this->m_ui_components) {
                        for (auto &component : ui_component) {
// std::cout << __FILE__ " :" << __LINE__ << "\n";
                                if (DoActionResult::HandledOrBlocked ==
                                    component->DoSelectionAction(relative_selection_action)) {
// std::cout << __FILE__ " :" << __LINE__ << "\n";
                                        component->SetSelected(true);
                                        return DoActionResult::HandledOrBlocked;
                                }
                        }
                }
                return DoActionResult::NotHandledOrNotBlocked;
        }

private:
        constexpr void RenderUIComponent(const Lumen::UI::Component::BasicUIComponent &ui_component) noexcept
        {
// std::cout << __FILE__ " :" << __LINE__ << "\n";
                switch (ui_component.GetUIType()) {
                case Lumen::UI::Component::UIComponentTypeTag::MENU:
// std::cout << __FILE__ " :" << __LINE__ << "\n";
                        this->RenderMenu(dynamic_cast<const Lumen::UI::Component::Menu &>(ui_component));
                        break;
                
                default:
                        break;
                }
// std::cout << __FILE__ " :" << __LINE__ << "\n";
                ui_component.SetSelected(false);
        }


        constexpr void RenderMenu(const Lumen::UI::Component::Menu &menu) noexcept
        {
                if (!menu.IsVisible()) {
                        return;
                }
                if (this->m_draw_bounding_box) {
// std::cout << __FILE__ " :" << __LINE__ << "\n";
                        const auto color = (menu.IsSelected()) ? this->m_bounding_box_color_selected :
                                           this->m_bounding_box_color;
                        this->RenderDrawBoundingBox(menu.GetTransformRectangleArea().absolute_rectangle_area, color);
                        this->RenderDrawBoundingBoxMenuButton(menu);
                }

// std::cout << __FILE__ " :" << __LINE__ << "\n";
                this->RenderDrawMenuButton(menu);
                this->RenderDrawSubMenu(menu);
        }

        constexpr void RenderDrawSubMenu(const Lumen::UI::Component::Menu &menu) noexcept
        {
                // for (const auto &menu_button : menu.GetMenuButtons()) {
                //         if (menu_button.IsSelected() && menu_button.HasSubMenu()) {
                //                 this->RenderMenu(menu_button.GetSubMenu());
                //         }
                // }
                if (!menu.HasSelectedSubMenu()) {
// std::cout << __FILE__ " :" << __LINE__ << "\n";
                        return;
                }
                const auto &sub_menu = menu.GetSelectedSubMenu();
std::cout << __FILE__ " :" << __LINE__ << "\n";
                this->RenderMenu(sub_menu);
        }

        constexpr void RenderDrawMenuButton(const Lumen::UI::Component::Menu &menu) noexcept
        {
// std::cout << __FILE__ " :" << __LINE__ << ": menu.size(): " << menu.GetMenuButtons().size() << "\n";
                for (const auto &menu_button : menu.GetMenuButtons()) {
// std::cout << __FILE__ " :" << __LINE__ << "\n";
                        if (!menu_button.IsVisible()) {
// std::cout << __FILE__ " :" << __LINE__ << "\n";
                                continue;
                        }

                        if (menu_button.HasText()) {
// std::cout << __FILE__ " :" << __LINE__ << "\n";
                                this->m_texts_needed_to_render.push_back(&menu_button.GetText());
// sf::RectangleShape rectangle_shape;
// rectangle_shape.setPosition(menu_button.GetText().getGlobalBounds().position);
// std::cout << __FILE__ " :" << __LINE__ << ": {"
//           << menu_button.GetText().getGlobalBounds().position.x << ", "
//           << menu_button.GetText().getGlobalBounds().position.y << " }\n";
// rectangle_shape.setSize({
//         menu_button.GetText().getLocalBounds().size.x * menu_button.GetText().getScale().x,
//         menu_button.GetText().getLocalBounds().size.y * menu_button.GetText().getScale().y});
// rectangle_shape.setOutlineColor(sf::Color::Red);
// rectangle_shape.setOutlineThickness(1.0f);
// rectangle_shape.setFillColor(sf::Color::Transparent);
// this->m_debug_render_target->draw(rectangle_shape);
                        }

                        if (menu_button.HasBackground()) {
                                //this->m_debug_render_target->draw(menu_button.GetBackground());
                                this->RenderDrawSprite(
                                        menu_button.GetTransformRectangleArea().absolute_rectangle_area,
                                        menu_button.GetBackground());
                        }

                        menu_button.SetSelected(false);
                }
        }

        constexpr void RenderDrawBoundingBoxMenuButton(const Lumen::UI::Component::Menu &menu) noexcept
        {
                for (const auto &menu_button : menu.GetMenuButtons()) {
                        const auto color = (menu_button.IsSelected()) ? this->m_bounding_box_color_selected :
                                           this->m_bounding_box_color;
                        this->RenderDrawBoundingBox(menu_button.GetTransformRectangleArea().absolute_rectangle_area, color);
                }
        }

        constexpr void RenderDrawBoundingBox(
                const Lumen::UI::Component::AbsoluteTransformRectangleArea &rectangle_area,
                const sf::Color color = sf::Color::White) noexcept
        {
                std::array<sf::Vertex, 8> bounding_box_vertices;
// std::cout << __FILE__ " :" << __LINE__ << ": {"
//           << rectangle_area.top_left_position.x << ", "
//           << rectangle_area.top_left_position.y << " }\n";
// std::cout << __FILE__ " :" << __LINE__ << ": {"
//           << rectangle_area.bottom_right_position.x << ", "
//           << rectangle_area.bottom_right_position.y << " }\n";
                bounding_box_vertices[0].position = {rectangle_area.top_left_position.x, rectangle_area.top_left_position.y};
                bounding_box_vertices[1].position = {rectangle_area.bottom_right_position.x, rectangle_area.top_left_position.y};

                bounding_box_vertices[2].position = {rectangle_area.top_left_position.x, rectangle_area.top_left_position.y};
                bounding_box_vertices[3].position = {rectangle_area.top_left_position.x, rectangle_area.bottom_right_position.y};

                bounding_box_vertices[4].position = {rectangle_area.bottom_right_position.x, rectangle_area.top_left_position.y};
                bounding_box_vertices[5].position = {rectangle_area.bottom_right_position.x, rectangle_area.bottom_right_position.y};

                bounding_box_vertices[6].position = {rectangle_area.top_left_position.x, rectangle_area.bottom_right_position.y};
                bounding_box_vertices[7].position = {rectangle_area.bottom_right_position.x, rectangle_area.bottom_right_position.y};

                for (auto &vertex : bounding_box_vertices) {
                        vertex.color = color;
                }

                for (const auto &vertex : bounding_box_vertices) {
                        this->m_vertex_array_bounding_box.append(vertex);
                }
        }

        constexpr void RenderDrawSprite(
                const Lumen::UI::Component::AbsoluteTransformRectangleArea &rectangle_area,
                const Lumen::UI::Component::Sprite &sprite) noexcept
        {
                std::array<sf::Vertex, 6> sprite_vertices;

                const auto &bounds = rectangle_area;
                const auto &texture_area = sprite.texture_rectangle_area;

                sprite_vertices[0].position = { bounds.top_left_position.x, bounds.top_left_position.y };
                sprite_vertices[1].position = { bounds.top_left_position.x,
                                                bounds.bottom_right_position.y };
                sprite_vertices[2].position = { bounds.bottom_right_position.x,
                                                bounds.top_left_position.y };
                
                sprite_vertices[0].texCoords = { texture_area.top_left_position.x, texture_area.top_left_position.y };
                sprite_vertices[1].texCoords = { texture_area.top_left_position.x,
                                                 texture_area.bottom_right_position.y };
                sprite_vertices[2].texCoords = { texture_area.bottom_right_position.x,
                                                 texture_area.top_left_position.y };

                sprite_vertices[3].position = { bounds.top_left_position.x,
                                                bounds.bottom_right_position.y };
                sprite_vertices[4].position = { bounds.bottom_right_position.x,
                                                bounds.top_left_position.y };
                sprite_vertices[5].position = { bounds.bottom_right_position.x,
                                                bounds.bottom_right_position.y };
                
                sprite_vertices[3].texCoords = { texture_area.top_left_position.x,
                                                 texture_area.bottom_right_position.y };
                sprite_vertices[4].texCoords = { texture_area.bottom_right_position.x,
                                                 texture_area.top_left_position.y };
                sprite_vertices[5].texCoords = { texture_area.bottom_right_position.x,
                                                 texture_area.bottom_right_position.y };

                for (auto &vertex : sprite_vertices) {
                        this->m_vertex_array.append(vertex);
                }
        }

};

} // namespace UI
} // namespace Lumen