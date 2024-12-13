
#pragma once

#include "Scene.hpp"
#include "MenuScene/MenuScene.hpp"
#include "GamePlayScene/GamePlayScene.hpp"

#include <cassert>
#include <cstdlib>
#include <vector>
#include <memory>

namespace Lumen {
namespace Scene {

class SceneManager {
private:
        using this_t = SceneManager;

        sf::RenderWindow *m_window_ptr;
        Lumen::ECS::Entity::EntityManager *m_entity_manager_ptr;

        Lumen::Scene::InterSceneCommunicationData m_inter_scene_communication_data;
        std::vector<std::unique_ptr<Lumen::Scene::BaseScene> > m_scene_vector;
        Lumen::Scene::BaseScene *m_current_scene_ptr;

        bool m_is_initialized{false};

        constexpr Lumen::Scene::BaseScene *GetScene(Lumen::Scene::SceneID scene_id) noexcept
        {
                assert(static_cast<std::size_t>(scene_id) < this->m_scene_vector.size());
                if (nullptr == this->m_scene_vector[static_cast<std::size_t>(scene_id)]) {
                        this->m_scene_vector[static_cast<std::size_t>(scene_id)] = this_t::CreateScene(scene_id);
                        this->m_scene_vector[static_cast<std::size_t>(scene_id)]->Init();
                }
                return this->m_scene_vector[static_cast<std::size_t>(scene_id)].get();
        }

        constexpr void DestroyScene(Lumen::Scene::SceneID scene_id) noexcept
        {
                this->m_scene_vector[static_cast<std::size_t>(scene_id)] = nullptr;
        }

        constexpr std::unique_ptr<Lumen::Scene::BaseScene> CreateScene(Lumen::Scene::SceneID scene_id) noexcept
        {
                switch (scene_id) {
                case Lumen::Scene::SceneID::MENU: {
                        return std::make_unique<Lumen::Scene::MenuScene>(this->m_window_ptr,
                                                                         this->m_entity_manager_ptr,
                                                                         &this->m_inter_scene_communication_data);
                }
                
                case Lumen::Scene::SceneID::GAME_PLAY:
                        return std::make_unique<Lumen::Scene::GamePlayScene>(this->m_window_ptr,
                                                                             this->m_entity_manager_ptr,
                                                                             &this->m_inter_scene_communication_data);

                default:
                        break;
                }
                std::abort();
        }
        

public:
        constexpr SceneManager(void) noexcept
        : m_window_ptr{nullptr}, m_entity_manager_ptr{nullptr}, m_current_scene_ptr{nullptr} {}

        constexpr void Init(sf::RenderWindow *window_ptr,
                            Lumen::ECS::Entity::EntityManager *entity_manager_ptr) noexcept
        {
                assert(nullptr != window_ptr);
                assert(nullptr != entity_manager_ptr);

                this->m_window_ptr = window_ptr;
                this->m_entity_manager_ptr = entity_manager_ptr;

                this->m_inter_scene_communication_data.running = true;
                this->m_inter_scene_communication_data.change_scene = false;

                this->m_scene_vector.resize(static_cast<std::size_t>(Lumen::Scene::SceneID::NUMBER_OF_SCENES));

                this->m_current_scene_ptr = this->GetScene(Lumen::Scene::SceneID::MENU);

                this->m_is_initialized = true;

        }

        constexpr void SetDeltaTime(float delta_time) noexcept
        {
                this->m_inter_scene_communication_data.delta_time = delta_time;
        }

        /*constexpr SceneType CurrentScene(void) const noexcept
        {
                return this->m_sceneData.currentScene;
        }*/

        /*constexpr void ClearAllScenes(void) noexcept
        {
                this->m_menuScene.Clear(this->m_sceneData);
                this->m_gamePlayScene.Clear(this->m_sceneData);
        }*/


        /*constexpr void ClearScene(SceneType scene) noexcept
        {
                switch (scene) {
                case SceneType::Null:
                        break;

                case SceneType::Menu:
                        this->m_menuScene.Clear(this->m_sceneData);
                        break;

                case SceneType::GamePlay:
                        this->m_gamePlayScene.Clear(this->m_sceneData);
                        break;
                }
        }*/

        constexpr void Update(void) noexcept
        {
                assert(this->m_is_initialized);
                if (this->m_inter_scene_communication_data.change_scene) {std::cout << "[SceneManager] change scene\n";
                        this->ChangeScene();
                }

                this->m_current_scene_ptr->Update();
        }

        constexpr void Render(void) noexcept
        {
                this->m_current_scene_ptr->Render();
                this->m_window_ptr->display();
        }

        constexpr bool IsRunning(void) const noexcept
        {
                assert(this->m_is_initialized);
                return this->m_inter_scene_communication_data.running;
        }

private:
        constexpr void ChangeScene(void) noexcept
        {
                assert(this->m_is_initialized);
                assert(this->m_inter_scene_communication_data.change_scene);

                this->m_current_scene_ptr = this->GetScene(this->m_inter_scene_communication_data.change_scene_args.change_to_this_scene);
                this->m_current_scene_ptr->ChangeToThisScene(this->m_inter_scene_communication_data.change_scene_args);

                this->m_inter_scene_communication_data.change_scene = false;
        }
};

} // namespace Scene
} // namespace Lumen
