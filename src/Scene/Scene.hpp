
#pragma once

#include <ECS/Entity/EntityManager.hpp>
#include <Action/ActionManager.hpp>
#include <LayerStack/LayerStack.hpp>
#include <ECS/Entity/EntityManager.hpp>
#include <ResourceManager/ResourceManager.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <cstddef>
#include <string>
#include <vector>
#include <utility>
#include <type_traits>
#include <variant>

namespace Lumen {

namespace Scene {
enum class SceneID {
        NULL_SCENE,
        MENU,
        GAME_PLAY,
        NUMBER_OF_SCENES,
};


struct ChangeSceneArgs {
        Lumen::Scene::SceneID from_scene;
        Lumen::Scene::SceneID change_to_this_scene;

        struct Void {};
        using Args = std::variant<Void>;
        Args args;

        constexpr ChangeSceneArgs(void) noexcept
        : from_scene{Lumen::Scene::SceneID::NULL_SCENE},
          change_to_this_scene{Lumen::Scene::SceneID::NULL_SCENE} {}

        constexpr void SetChangeSceneArgs(Lumen::Scene::SceneID from_scene,
                                          Lumen::Scene::SceneID change_to_this_scene) noexcept
        {
                this->from_scene = from_scene;
                this->change_to_this_scene = change_to_this_scene;
                this->args = {};
        }

        constexpr void SetChangeSceneArgs(Lumen::Scene::SceneID from_scene,
                                          Lumen::Scene::SceneID change_to_this_scene,
                                          Args &&args) noexcept
        {
                this->from_scene = from_scene;
                this->change_to_this_scene = change_to_this_scene;
                this->args = std::move(args);
        }

        constexpr void Clear(void) noexcept
        {
                this->from_scene = Lumen::Scene::SceneID::NULL_SCENE;
                this->change_to_this_scene = Lumen::Scene::SceneID::NULL_SCENE;
                this->args = {};
        }
};

struct InterSceneCommunicationData {
        float delta_time;
        ChangeSceneArgs change_scene_args;
        bool change_scene : 1;
        bool running : 1;

        constexpr InterSceneCommunicationData(void) noexcept : delta_time{0.0f}, change_scene{false}, running{true} {}
};

namespace GamePlayLayer {

class TestUILayer;
class GameWorldLayer;
class TestBackgroundLayer;

} // namespace GamePlayLayer

namespace MenuLayer {

class TestUILayer;
class TestBackgroundLayer;

} // namespace MenuLayer

class BaseScene {
        friend Lumen::Scene::GamePlayLayer::TestUILayer;
        friend Lumen::Scene::GamePlayLayer::GameWorldLayer;
        friend Lumen::Scene::GamePlayLayer::TestBackgroundLayer;
        friend Lumen::Scene::MenuLayer::TestUILayer;
        friend Lumen::Scene::MenuLayer::TestBackgroundLayer;
protected:
        Lumen::Scene::SceneID m_scene_id;

        std::size_t m_current_frame;
        Lumen::Action::ActionManager m_action_manager;
        Lumen::LayerStack::LayerStack m_layer_stack;
        sf::RenderWindow *m_window_ptr;
        Lumen::ResourceManager::ResourceManager *m_resource_manager_ptr;
        Lumen::ECS::Entity::EntityManager *m_entity_manager_ptr;
        Lumen::Scene::InterSceneCommunicationData *m_inter_scene_communication_data;

        bool m_view_changed : 1;
        bool m_is_paused : 1;
        bool m_has_ended : 1;

public:
        constexpr BaseScene(Lumen::Scene::SceneID scene_id,
                            sf::RenderWindow *window_ptr,
                            Lumen::ResourceManager::ResourceManager *resource_manager_ptr,
                            Lumen::ECS::Entity::EntityManager *entity_manager_ptr,
                            Lumen::Scene::InterSceneCommunicationData *inter_scene_communication_data) noexcept
        : m_scene_id{scene_id}, m_current_frame{0}, m_window_ptr{window_ptr},
          m_resource_manager_ptr{resource_manager_ptr}, m_entity_manager_ptr{entity_manager_ptr},
          m_inter_scene_communication_data{inter_scene_communication_data},
          m_view_changed{true}, m_is_paused{false}, m_has_ended{false} {}

        constexpr virtual ~BaseScene(void) noexcept {}

        constexpr virtual void Init(void) noexcept = 0;
        constexpr virtual void Update(float delta_time) noexcept = 0;
        constexpr virtual void Render(void) noexcept = 0;
        constexpr virtual void ChangeToThisScene(Lumen::Scene::ChangeSceneArgs &change_scene_args) noexcept = 0;

        // Call derived scene's Update() a given number of times
        //constexpr virtual void Simulate(std::size_t simulate_count);
        //constexpr virtual void DoActionImpl(Lumen::Action::Action &);

        constexpr void RegisteAction(sf::Keyboard::Key key_code, Lumen::Action::ActionName action_name) noexcept
        {
                this->m_action_manager.RegisteAction(key_code, action_name);
        }

        constexpr void RemoveAction(sf::Keyboard::Key key_code) noexcept
        {
                this->m_action_manager.RemoveAction(key_code);
        }

        constexpr void RemoveAction(Lumen::Action::ActionName action_name) noexcept
        {
                this->m_action_manager.RemoveAction(action_name);
        }

        //constexpr virtual void Clear(void);
};

/*struct scene_menu_t {
        std::size_t level_count;
        
        void Update(void);
        void clear(void);
};*/

/*struct scene_play_t {
        std::string level_path;
        EntityManager entity_manager;
        std::size_t frame_count_after_enemy_spawn;

        //using this_t = engine_t;
        using rand_eng_t = std::mt19937;
        rand_eng_t random;

        void init(void);
        void Update(void);

        void animation_tick(void);
        void movement_tick(void);
        void enemy_spawner_tick(void);
        void collision_tick(void);
        void rendering_tick(void);
        void do_action_tick(void);
        void debug_tick(void);

        void clear(void);
};*/
} // namespace Scene
} // namespace Lumen
