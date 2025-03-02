
#pragma once

#include "GamePlayLayer/TestUILayer.hpp"
#include "GamePlayLayer/TestBackgroundLayer.hpp"
#include "GamePlayLayer/GameWorldLayer.hpp"

#include <Scene/Scene.hpp>
#include <ECS/System/Debug/DrawBoundingBox.hpp>
#include <ECS/System/Movement.hpp>
#include <ECS/System/LifespanUpdate.hpp>
#include <ECS/System/Collision.hpp>

namespace Lumen {
namespace Scene {

class TestUILayer;

class GamePlayScene : public Lumen::Scene::BaseScene {
private:
        bool m_is_initialized{false};
public:
        constexpr GamePlayScene(sf::RenderWindow *window_ptr,
                                Lumen::ResourceManager::ResourceManager *resource_manager,
                                Lumen::ECS::Entity::EntityManager *entity_manager_ptr,
                                Lumen::Scene::InterSceneCommunicationData *inter_scene_communication_data) noexcept
         : Lumen::Scene::BaseScene{Lumen::Scene::SceneID::GAME_PLAY,
                                   window_ptr,
                                   resource_manager,
                                   entity_manager_ptr,
                                   inter_scene_communication_data},
           m_is_initialized{false} {}

        constexpr void Init(void) noexcept override
        {
                if (this->m_is_initialized) {
                        return;
                }

                this->InitActionManager();
                this->InitLayerStack();
                this->InitSpawnePlayer();
                this->InitSpawneBlock();
                this->InitSpawneLifesanBlock();

                auto &entity_manager = *Lumen::Scene::BaseScene::m_entity_manager_ptr;
                entity_manager.Update();

                this->m_is_initialized = true;
        }

        constexpr void Update(float delta_time) noexcept override
        {
                assert(this->m_is_initialized);
                this->CreateActions();
                this->DoActions();
                this->Collision();
                this->Movement();
                this->LifespanUpdate();
                
                (void)delta_time;

                auto &entity_manager = *Lumen::Scene::BaseScene::m_entity_manager_ptr;
                entity_manager.Update();
                //auto &e = this->m_entity_manager_ptr->GetEntityCurrentlyControlledByThePlayer();
                //auto &t = e.GetComponent<Lumen::ECS::Component::Transform>();
                //std::cout << "[GamePlayScene] t.position{ " << t.position.x << ", " << t.position.y << "}\n";
        }

        constexpr void Render(void) noexcept override
        {
                assert(this->m_is_initialized);

                if (Lumen::Scene::BaseScene::m_view_changed) {
                        this->SetView();
                        Lumen::Scene::BaseScene::m_view_changed = false;
                }

                for (auto &layer : Lumen::Scene::BaseScene::m_layer_stack) {
                        layer->Render();
                }

                //std::cout << "[GamePlayScene] Render\n";
                Lumen::ECS::System::Debug::DrawBoundingBoxForEach(*Lumen::Scene::BaseScene::m_window_ptr, *Lumen::Scene::BaseScene::m_entity_manager_ptr);
                //std::cout << "[GamePlayScene] Render end\n";
        }

        constexpr void ChangeToThisScene(Lumen::Scene::ChangeSceneArgs &change_scene_args) noexcept override
        {
                assert(this->m_is_initialized);
                (void)change_scene_args;
        }

private:

        constexpr void Collision(void) noexcept
        {
                assert(this->m_is_initialized);
                //std::cout << "[GamePlayScene] Collision\n";
                Lumen::ECS::System::CollisionForEach(*Lumen::Scene::BaseScene::m_entity_manager_ptr);
        }

        constexpr void LifespanUpdate(void) noexcept
        {
                assert(this->m_is_initialized);
                float delta_time = Lumen::Scene::BaseScene::m_inter_scene_communication_data->delta_time;
                Lumen::ECS::System::LifespanUpdateForEach(*Lumen::Scene::BaseScene::m_entity_manager_ptr, delta_time);
        }

        constexpr void Movement(void) noexcept
        {
                assert(this->m_is_initialized);
                //std::cout << "[GamePlayScene] Movement\n";
                float delta_time = Lumen::Scene::BaseScene::m_inter_scene_communication_data->delta_time;
                Lumen::ECS::System::MovementForEach(*Lumen::Scene::BaseScene::m_entity_manager_ptr, delta_time);
        }

        constexpr void SetView(void) noexcept
        {
                auto &window = *Lumen::Scene::BaseScene::m_window_ptr;
                auto view = window.getView();
                const auto window_size = window.getSize();
                view.setSize({static_cast<float>(window_size.x), static_cast<float>(window_size.y)});

                const auto player_position = GetPlayerPosition();
                view.setCenter({player_position.x, player_position.y});
                window.setView(view);
        }

        constexpr Lumen::Core::Math::Vec2f32 GetPlayerPosition(void) const noexcept
        {
                auto &entity_manager = *Lumen::Scene::BaseScene::m_entity_manager_ptr;
                auto &player = entity_manager.GetEntityCurrentlyControlledByThePlayer();
                assert(player.HasComponent<Lumen::ECS::Component::Transform>());
                const auto transform = player.GetComponent<Lumen::ECS::Component::Transform>();
                return transform.position;
        }

        constexpr void InitActionManager(void) noexcept
        {
                assert(!this->m_is_initialized);
                Lumen::Scene::BaseScene::m_action_manager.Init();

                // TODO: read action map data from file
                Lumen::Scene::BaseScene::RegisteAction(sf::Keyboard::Key::W, Lumen::Action::ActionName::MOVE_UP);
                Lumen::Scene::BaseScene::RegisteAction(sf::Keyboard::Key::A, Lumen::Action::ActionName::MOVE_LEFT);
                Lumen::Scene::BaseScene::RegisteAction(sf::Keyboard::Key::S, Lumen::Action::ActionName::MOVE_DOWN);
                Lumen::Scene::BaseScene::RegisteAction(sf::Keyboard::Key::D, Lumen::Action::ActionName::MOVE_RIGHT);

                Lumen::Scene::BaseScene::RegisteAction(sf::Keyboard::Key::Up, Lumen::Action::ActionName::MOVE_UP);
                Lumen::Scene::BaseScene::RegisteAction(sf::Keyboard::Key::Left, Lumen::Action::ActionName::MOVE_LEFT);
                Lumen::Scene::BaseScene::RegisteAction(sf::Keyboard::Key::Down, Lumen::Action::ActionName::MOVE_DOWN);
                Lumen::Scene::BaseScene::RegisteAction(sf::Keyboard::Key::Right, Lumen::Action::ActionName::MOVE_RIGHT);

                Lumen::Scene::BaseScene::RegisteAction(sf::Keyboard::Key::Escape, Lumen::Action::ActionName::QUIT);
                Lumen::Scene::BaseScene::m_action_manager.SetActionKind(Lumen::Action::ActionName::QUIT, Lumen::Action::ActionKind::TRIGGER);
                //for (auto &actionKeyCodePair : menuSceneConfigData.actionKeyCodePairVector) {
                //        this->m_action_manager.RegisteAction(actionKeyCodePair.keyCode, actionKeyCodePair.actionName);
                //}
        }

        constexpr void InitLayerStack(void) noexcept
        {
                assert(!this->m_is_initialized);
                Lumen::Scene::BaseScene::m_layer_stack.PushBackLayer(Lumen::LayerStack::MakeLayer<GamePlayLayer::TestUILayer>(this));
                Lumen::Scene::BaseScene::m_layer_stack.PushBackLayer(Lumen::LayerStack::MakeLayer<GamePlayLayer::GameWorldLayer>(this));
                Lumen::Scene::BaseScene::m_layer_stack.PushBackLayer(Lumen::LayerStack::MakeLayer<GamePlayLayer::TestBackgroundLayer>(this));
        }

        constexpr void InitSpawnePlayer(void) noexcept
        {
                assert(!this->m_is_initialized);
                auto &entity_manager = *Lumen::Scene::BaseScene::m_entity_manager_ptr;
                auto &player_entity = entity_manager.CreateEntity(Lumen::ECS::Entity::Entity::TagType::PLAYER);
                player_entity.AddComponent<Lumen::ECS::Component::Transform>(
                        Lumen::Core::Math::Vec2f32{0.0f, 0.0f},
                        Lumen::Core::Math::Vec2f32{0.0f, 0.0f},
                        Lumen::Core::Math::Vec2f32{15.0f, 16.0f});

                player_entity.AddComponent<Lumen::ECS::Component::BoundingBox>(
                        Lumen::Core::Math::Vec2f32{30.0f, 30.0f}
                );

                entity_manager.SetEntityCurrentlyControlledByThePlayer(player_entity.GetTag(), player_entity.GetId());
                std::cout << "[InitSpawnePlayer]\n";
        }

        constexpr void InitSpawneBlock(void) noexcept
        {
                this->SpawneBlock({50.0f, 70.0f}, {{20.0f, 80.0f}});
                this->SpawneBlock({100.0f, 50.0f}, {{80.0f, 20.0f}});
        }

        constexpr void InitSpawneLifesanBlock(void) noexcept
        {
                auto &entity_manager = *Lumen::Scene::BaseScene::m_entity_manager_ptr;
                auto &tile_entity = entity_manager.CreateEntity(Lumen::ECS::Entity::Entity::TagType::TILE);
                tile_entity.AddComponent<Lumen::ECS::Component::Transform>(
                        Lumen::Core::Math::Vec2f32{200.0f, 50.0f},
                        Lumen::Core::Math::Vec2f32{0.0f, 0.0f},
                        Lumen::Core::Math::Vec2f32{0.0f, 0.0f}
                );

                tile_entity.AddComponent<Lumen::ECS::Component::BoundingBox>(
                        {Lumen::Core::Math::Vec2f32{50.0f, 10.0f},
                         Lumen::ECS::Component::BoundingBox::BlockVision{false}}
                );

                tile_entity.AddComponent<Lumen::ECS::Component::Lifespan>(
                        3.0f
                );
        }

        constexpr void SpawneBlock(const Lumen::Core::Math::Vec2f32 &position, const Lumen::ECS::Component::BoundingBox &bounding_box) noexcept
        {
                auto &entity_manager = *Lumen::Scene::BaseScene::m_entity_manager_ptr;
                auto &tile_entity = entity_manager.CreateEntity(Lumen::ECS::Entity::Entity::TagType::TILE);
                tile_entity.AddComponent<Lumen::ECS::Component::Transform>(
                        position,
                        Lumen::Core::Math::Vec2f32{0.0f, 0.0f},
                        Lumen::Core::Math::Vec2f32{0.0f, 0.0f}
                );

                tile_entity.AddComponent<Lumen::ECS::Component::BoundingBox>(
                        bounding_box
                );
        }

        constexpr void CreateActions(void) noexcept
        {
                assert(this->m_is_initialized);
                for (std::optional<sf::Event> optional_event = Lumen::Scene::BaseScene::m_window_ptr->pollEvent();
                     optional_event.has_value();
                     optional_event = Lumen::Scene::BaseScene::m_window_ptr->pollEvent()) {
                        sf::Event &event = optional_event.value();
                        Lumen::Scene::BaseScene::m_action_manager.CreateActionFromEvent(event);
                }
        }

        constexpr void DoActions(void) noexcept
        {
                assert(this->m_is_initialized);

                this->DoBasicAction();
                this->DoMovementAction();
                this->DoWindowResizeAction();
                this->DoWindowCloseAction();
        }

        constexpr void DoBasicAction(void) noexcept
        {
                for (auto action : Lumen::Scene::BaseScene::m_action_manager) {
                        for (auto layer_stack_it = Lumen::Scene::BaseScene::m_layer_stack.rbegin();
                             layer_stack_it != Lumen::Scene::BaseScene::m_layer_stack.rend();
                             ++layer_stack_it) {
                                Lumen::LayerStack::LayerPtr &layer_ptr = (*layer_stack_it);
                                if (Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked == layer_ptr->DoAction(action)) {
                                        break;
                                }
                        }
                }
        }

        constexpr void DoMovementAction(void) noexcept
        {
                const auto movement_action = Lumen::Scene::BaseScene::m_action_manager.GetMovementAction();
                for (auto layer_stack_it = Lumen::Scene::BaseScene::m_layer_stack.rbegin();
                        layer_stack_it != Lumen::Scene::BaseScene::m_layer_stack.rend();
                        ++layer_stack_it) {
                        Lumen::LayerStack::LayerPtr &layer_ptr = (*layer_stack_it);
                        if (Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked == layer_ptr->DoMovementAction(movement_action)) {
                                break;
                        }
                }
        }

        constexpr void DoWindowResizeAction(void) noexcept
        {
                if (Lumen::Scene::BaseScene::m_action_manager.HasWindowResizeAction()) {
                        //std::cout << "[GamePlayScene] HasWindowResizeAction\n";
                        this->SetView();
                        Lumen::Scene::BaseScene::m_action_manager.ResetWindowResizeAction();
                }
        }

        constexpr void DoWindowCloseAction(void) noexcept
        {
                if (Lumen::Scene::BaseScene::m_action_manager.IsWindowCloseActionHappened()) {
                        //std::cout << "[GamePlayScene] IsWindowCloseActionHappened\n";
                        Lumen::Scene::BaseScene::m_inter_scene_communication_data->running = false;
                        Lumen::Scene::BaseScene::m_action_manager.ResetActionBuffer();
                        std::cout << "[GamePlayScene] IsWindowCloseActionHappened\n";
                }
        }


/*private:

public:
        virtual ~GamePlayScene(void) noexcept = default;

        constexpr void Init(void) noexcept override {}
        constexpr void Update(void) noexcept override {}
        constexpr void Animation(void) noexcept {}
        constexpr void Movement(void) noexcept {}
        constexpr void EnemySpawner(void) noexcept {}
        constexpr void Collision(void) noexcept {}
        constexpr void Render(void) noexcept override {}
        constexpr void DoAction(void) noexcept {}
        constexpr void Debug(void) noexcept {}*/
};

} // namespace Scene
} // namespace Lumen

/*
#include <Vec2.hpp>

#include <cassert>
#include <chrono>
//#include <thread>

class GamePlayScene : public BaseScene {
private:
        LevelPath m_level_path;
        Entity *m_player_ptr;
        Config m_config;

private:
        void InitHelperRegisteActions(void) noexcept
        {

        }
public:
        void Init(void) noexcept
        {
                // TODO: Registe Actions using data from config file
                this->InitHelperRegisteActions();


        }
        
        void Update(void);
        void Init(void);
        void Animation(void);
        void Movement(void);
        void EnemySpawner(void);
        void Collision(void);
        void Render(void);
        void DoAction(void);
        void Debug(void);
};

template<typename AssetManager, typename AnimationId, typename KeyMap,
         typename BoundingBox, typename Transform, typename Animation,
         typename Lifespan, typename State,
         typename SetView,
         typename TextureRender, typename CollisionRender, typename GraidRender,
         typename MovementSystem, typename PhysicsCollision, typename AnimationUpdate,
         typename LifespanUpdate,
         typename Event, typename Entity>
class BasicGamePlayScene {
private:
        SetView fn_setView;
        TextureRender fn_textureRender;
        CollisionRender fn_collisionRender;
        GraidRender fn_graidRender;
        MovementSystem fn_movementSystem;
        PhysicsCollision fn_physicsCollisionSystem;
        AnimationUpdate fn_animationUpdate;
        LifespanUpdate fn_lifespanUpdate;
        KeyMap m_keyMap;
        bool m_isInitialized : 1;
        bool m_isPaused : 1;
        bool m_drawTexture : 1;
        bool m_drawCollision : 1;
        bool m_drawGrid : 1;

        using this_t = BasicGamePlayScene;
public:
        constexpr BasicGamePlayScene(void) noexcept
        : m_isInitialized{false}, m_isPaused{false},
          m_drawTexture{false}, m_drawCollision{true},
          m_drawGrid{false} {}

        template<typename SceneData>
        constexpr void Init(SceneData &sceneData) noexcept
        {
                if (this->m_isInitialized) {
                        return;
                }
                auto &gamePlaySceneConfigData = sceneData.configData.gamePlayScene;
                this->InitKeyMap(gamePlaySceneConfigData);
                this->CreateLevel(sceneData);*/

                /*this->m_isInitialized = true;
                this->m_drawTexture = true;
                this->m_drawCollision = false;
                this->m_drawGrid = false;*//*
                this->m_isInitialized = true;
                this->m_drawTexture = true;
                this->m_drawCollision = true;
                this->m_drawGrid = false;

        }

        template<typename SceneData>
        constexpr void Clear(SceneData &sceneData) noexcept
        {
                sceneData.entityManager.Clear();
                this->m_keyMap.Clear();
                this->m_isInitialized = false;
        }

        template<typename SceneData>
        constexpr void Update(SceneData &sceneData) noexcept
        {
                this->DoAction(sceneData);

                if (this->m_isPaused) {
                        //using namespace std::chrono_literals;
                        //std::this_thread::sleep_for(2000ms);
                        return;
                }

                auto &entityManager = sceneData.entityManager;
                entityManager.Update();

                this->fn_movementSystem(sceneData);
                this->fn_physicsCollisionSystem(sceneData);
                this->fn_animationUpdate(sceneData);
                this->fn_lifespanUpdate(sceneData);
        }

        template<typename SceneData>
        constexpr void Render(SceneData &sceneData) noexcept
        {
                if (this->m_isPaused) {
                        return;
                }

                sceneData.ClearWindow(sceneData.configData.gamePlayScene.backgroundColor);
                this->fn_setView(sceneData);
                if (this->m_drawTexture) {
                        this->fn_textureRender(sceneData);
                }
                if (this->m_drawCollision) {
                        this->fn_collisionRender(sceneData);
                }
                if (this->m_drawGrid) {
                        this->fn_graidRender(sceneData);
                }
                sceneData.Display();
        }

private:

        template<typename GamePlaySceneConfigData>
        constexpr void InitKeyMap(GamePlaySceneConfigData &gamePlaySceneConfigData) noexcept
        {
                this->m_keyMap.Init();
                for (auto &actionKeyCodePair : gamePlaySceneConfigData.actionKeyCodePairVector) {
                        this->m_keyMap.RegisteAction(actionKeyCodePair.keyCode, actionKeyCodePair.actionName);
                }
        }

        template<typename SceneData>
        constexpr void CreateLevel(SceneData &sceneData) noexcept
        {
                sceneData.entityManager.Clear();

                auto &changeSceneArgs = sceneData.changeSceneArgs;
                std::size_t selectedLevelNumber = changeSceneArgs.GetLevelNamberMenuToGamePlay();
                const auto &gamePlaySceneData = sceneData.configData.gamePlayScene;
                assert(selectedLevelNumber < gamePlaySceneData.levelDataVector.size());
                const auto &levelData = gamePlaySceneData.levelDataVector[selectedLevelNumber];

                this->CreateGameWorld(sceneData, levelData);
                this->SpawnPlayer(sceneData, levelData);

                changeSceneArgs.Clear();
        }

        template<typename SceneData, typename LevelData>
        constexpr void CreateGameWorld(SceneData &sceneData, const LevelData &levelData) noexcept
        {
                const auto &assetManager = sceneData.assetManager;
                auto &entityManager = sceneData.entityManager;
                
                {
                        const auto &tileDataVector = levelData.tileDataVector;
                        this->SpawnTiles(entityManager, tileDataVector, assetManager, sceneData);
                }
                
        }

        template<typename EntityManager, typename TileDataVector, typename SceneData>
        constexpr void SpawnTiles(EntityManager &entityManager, const TileDataVector &tileDataVector,
                                  const AssetManager &assetManager, const SceneData &sceneData) noexcept
        {
                for (const auto &tileData : tileDataVector) {
                        auto &tile = entityManager.CreateEntity(Entity::TagType::Tile, tileData.animationType);
                        assert(nullptr != &tile);

                        const auto &tileBoundingBoxSize = tileData.boundingBoxSize;
                        assert(!tile.template HasComponent<BoundingBox>());
                        tile.template AddComponent<BoundingBox>(tileData.boundingBoxSize);

                        assert(!tile.template HasComponent<Transform>());
                        const Vec2f32 tilePosition =
                                this->GridToPixelPosition(sceneData, tileData.position, tileBoundingBoxSize);

                        //const Vec2f32 &tileAnimationSize = tileData.animationSize;
                        tile.template AddComponent<Transform>(
                                tilePosition,
                                Vec2f32{1.0f, 1.0f}, Vec2f32{0.0f, 0.0f}//,
                                //tileAnimationSize
                                );

                        assert(!tile.template HasComponent<Animation>());
                
                        AnimationId animationId = this_t::GetAnimationIdFromAnimationTypeCreateGameWorld(tileData.animationType);

                        const auto &tileAnimation = assetManager.GetAnimation(animationId);
                        assert(nullptr != &tileAnimation);
                        tile.template AddComponent<Animation>(tileAnimation);

                        assert(!tile.template HasComponent<State>());
                        tile.template AddComponent<State>({});

                }
        }

        template<typename AnimationType>
        static constexpr AnimationId
        GetAnimationIdFromAnimationTypeCreateGameWorld(AnimationType animationType) noexcept
        {
                switch (animationType) {
                case AnimationType::QuestionBrick:
                        return AnimationId::QuestionBrick;
                case AnimationType::Brick:
                        return AnimationId::Brick;
                case AnimationType::SmallPipe:
                        return AnimationId::SmallPipe;
                case AnimationType::TallPipe:
                        return AnimationId::TallPipe;
                case AnimationType::Ground:
                        return AnimationId::Ground;
                case AnimationType::Block:
                        return AnimationId::Block;
                case AnimationType::Pole:
                        return AnimationId::Pole;
                case AnimationType::PoleTop:
                        return AnimationId::PoleTop;
                default:
                        break;
                }
                assert(false);
                std::abort();
        }

        template<typename SceneData, typename LevelData>
        constexpr void SpawnPlayer(SceneData &sceneData, const LevelData &levelData) noexcept
        {
                const auto &gamePlaySceneData = sceneData.configData.gamePlayScene;
                const auto &playerData = gamePlaySceneData.playerData;
                const auto &assetManager = sceneData.assetManager;
                auto &entityManager = sceneData.entityManager;

                auto &player = entityManager.CreateEntity(Entity::TagType::Player, Entity::AnimationType::Player);
                assert(nullptr != &player);

                const auto &playerBoundingBoxSize = playerData.boundingBoxSize;
                assert(!player.template HasComponent<BoundingBox>());
                player.template AddComponent<BoundingBox>(playerBoundingBoxSize);


                assert(!player.template HasComponent<Transform>());
                const Vec2f32 playerInitialPosition =
                        this->GridToPixelPosition(sceneData, levelData.playerInitialPosition, playerBoundingBoxSize);
        
                //const Vec2f32 &playerAnimationSize = playerData.animationSize;
                player.template AddComponent<Transform>(
                        playerInitialPosition,
                        Vec2f32{1.0f, 1.0f}, Vec2f32{0.0f, 0.0f}//,
                        //playerAnimationSize
                        );

                assert(!player.template HasComponent<Gravity>());
                player.template AddComponent<Gravity>(playerData.gravitationalAcceleration);

                assert(!player.template HasComponent<Animation>());
                
                auto &playerStandAnimation = assetManager.GetAnimation(AnimationId::PlayerStand);
                assert(nullptr != &playerStandAnimation);
                player.template AddComponent<Animation>(playerStandAnimation);

        }

        template<typename SceneData>
        static constexpr Vec2f32 GridToPixelPosition(
                const SceneData &sceneData, const Vec2f32 &entityBottomLeftCornerGridPosition,
                const Vec2f32 &entityAnimationSize) noexcept
        {
                const Vec2f32 &gridCellSize = sceneData.configData.gamePlayScene.gridCellSize;

                const Vec2f32 entityBottomLeftCornerPixelPosition{
                        entityBottomLeftCornerGridPosition.x * gridCellSize.x,
                        entityBottomLeftCornerGridPosition.y * gridCellSize.y,
                };

                const Vec2f32 entityCenterPixelPositionWithFlippedYValue
                        = entityBottomLeftCornerPixelPosition + (entityAnimationSize / 2.0f);
                
                const auto &window = sceneData.window;
                const float windowHeight = static_cast<float>(window.getSize().y);
                const Vec2f32 entityCenterPixelPosition{
                        entityCenterPixelPositionWithFlippedYValue.x,
                        windowHeight - entityCenterPixelPositionWithFlippedYValue.y,
                };

                //transform.position = entityCenterPixelPosition;
                //transform.prevPosition = entityCenterPixelPosition;

                return entityCenterPixelPosition;
        }

        template<typename SceneData>
        constexpr void DoAction(SceneData &sceneData) noexcept
        {
                Event event;
                while (sceneData.PollEvent(event)) {
                        switch (event.type) {
                        case Event::KeyPressed: {
                                auto actionName = this->m_keyMap.GetActionNameFromKeyCode(event.key.code);
                                this->DoActionImpl(sceneData, Action{actionName, ActionType::Begin});
                                break;
                        }

                        case Event::KeyReleased: {
                                auto actionName = this->m_keyMap.GetActionNameFromKeyCode(event.key.code);
                                this->DoActionImpl(sceneData, Action{actionName, ActionType::End});
                                break;
                        }*/

                        /*case Event::MouseButtonPressed:
                                break;*/
                        /*default:
                                continue;
                        }
                }
        }

        template<typename SceneData>
        constexpr void DoActionImpl(SceneData &sceneData, Action action) noexcept
        {
                switch (action.actionType) {

                case ActionType::Void:
                        this->DoActionVoidImpl(sceneData, action.actionName);
                        break;

                case ActionType::Begin:
                        this->DoActionBeginImpl(sceneData, action.actionName);
                        break;
                case ActionType::End:
                        this->DoActionEndImpl(sceneData, action.actionName);
                        break;

                default:
                        break;
                }
        }

        template<typename SceneData>
        constexpr void DoActionVoidImpl(SceneData &sceneData, ActionName actionName) noexcept
        {
                switch (actionName) {

                case ActionName::Quit:
                        this->DoActionQuit(sceneData);
                        break;

                default:
                        assert(false);
                        break;
                }
        }

        template<typename SceneData>
        constexpr void DoActionBeginImpl(SceneData &sceneData, ActionName actionName) noexcept
        {
                switch (actionName) {

                case ActionName::Quit:
                        this->DoActionQuit(sceneData);
                        break;

                case ActionName::MoveUp:
                        this->fn_movementSystem.PlayerMoveUpBegin(sceneData);
                        break;

                case ActionName::MoveDown:
                        this->fn_movementSystem.PlayerMoveDownBegin(sceneData);
                        break;

                case ActionName::MoveLeft:
                        this->fn_movementSystem.PlayerMoveLeftBegin(sceneData);
                        break;

                case ActionName::MoveRight:
                        this->fn_movementSystem.PlayerMoveRightBegin(sceneData);
                        break;

                case ActionName::Shoot:
                        this->DoActionShoot(sceneData);
                        break;

                default:
                        break;
                }
        }

        template<typename SceneData>
        constexpr void DoActionEndImpl(SceneData &sceneData, ActionName actionName) noexcept
        {
                switch (actionName) {

                case ActionName::Quit:
                        this->DoActionQuit(sceneData);
                        break;
                case ActionName::Pause:
                        this->m_isPaused = !this->m_isPaused;
                        break;

                case ActionName::MoveUp:
                        this->fn_movementSystem.PlayerMoveUpEnd(sceneData);
                        break;

                case ActionName::MoveDown:
                        this->fn_movementSystem.PlayerMoveDownEnd(sceneData);
                        break;

                case ActionName::MoveLeft:
                        this->fn_movementSystem.PlayerMoveLeftEnd(sceneData);
                        break;

                case ActionName::MoveRight:
                        this->fn_movementSystem.PlayerMoveRightEnd(sceneData);
                        break;

                case ActionName::ToggleDrawingBoundingBox:
                        this->m_drawCollision = !this->m_drawCollision;
                        break;
                case ActionName::ToggleDrawingTexture:
                        this->m_drawTexture = !this->m_drawTexture;
                        break;
                case ActionName::ToggleDrawingGrid:
                        this->m_drawGrid = !this->m_drawGrid;
                        break;
        
                default:
                        break;
                }
        }

        template<typename SceneData>
        constexpr void DoActionQuit(SceneData &sceneData) noexcept
        {
                sceneData.ChangeScene(SceneType::GamePlay, SceneType::Menu);
                this->m_isPaused = false;
                this->Clear(sceneData);
        }

        template<typename SceneData>
        constexpr void DoActionShoot(SceneData &sceneData) noexcept
        {
                auto &entityManager = sceneData.entityManager;
                assert(1 == entityManager.GetEntityVector(Entity::TagType::Player).size());
                const auto &player = entityManager.GetEntityVector(Entity::TagType::Player)[0];

                const auto fnGetPlayerCurrentFaceDirection = [](const auto &player) -> float {
                        assert(player.template HasComponent<Animation>());
                        const auto &playerAnimation = player.template GetComponent<Animation>();
                        return (playerAnimation.GetScale().x < 0.0f) ? -1.0f : 1.0f;
                };
                const float playerCurrentFaceDirection = fnGetPlayerCurrentFaceDirection(player);

                const auto fnGetPlyerCurrentPosition = [](const auto &player) -> auto {
                        assert(player.template HasComponent<Transform>());
                        const auto &playerTransform = player.template GetComponent<Transform>();
                        return playerTransform.position;
                };
                const auto playerCurrentPosition = fnGetPlyerCurrentPosition(player);

                this->SpawnBullet(sceneData, playerCurrentPosition, playerCurrentFaceDirection);
        }

        template<typename SceneData, typename Vec2f32>
        constexpr void SpawnBullet(SceneData &sceneData, const Vec2f32 &position, const float direction) noexcept
        {
                const auto &gamePlaySceneData = sceneData.configData.gamePlayScene;
                const auto &bulletData = gamePlaySceneData.bulletData;
                auto &entityManager = sceneData.entityManager;
                const auto assetManager = sceneData.assetManager;

                auto &bullet = entityManager.CreateEntity(Entity::TagType::Bullet, Entity::AnimationType::Bullet);
                assert(nullptr != &bullet);

                const auto &bulletBoundingBoxSize = bulletData.boundingBoxSize;
                assert(!bullet.template HasComponent<BoundingBox>());
                bullet.template AddComponent<BoundingBox>(bulletBoundingBoxSize);


                assert(!bullet.template HasComponent<Transform>());
                const float velocityValueX = direction * bulletData.speed;
                bullet.template AddComponent<Transform>(
                        position,
                        Vec2f32{1.0f, 1.0f}, Vec2f32{velocityValueX, 0.0f}
                );

                assert(!bullet.template HasComponent<Animation>());
                auto bulletAnimation = assetManager.GetAnimation(AnimationId::Bullet);
                assert(nullptr != &bulletAnimation);
                auto scaleFactor = bulletAnimation.GetScale();
                scaleFactor.x *= direction;
                bulletAnimation.SetScale(scaleFactor);
                bullet.template AddComponent<Animation>(bulletAnimation);

                assert(!bullet.template HasComponent<Lifespan>());
                bullet.template AddComponent<Lifespan>(
                        bulletData.lifespan
                );
        }
};*/
