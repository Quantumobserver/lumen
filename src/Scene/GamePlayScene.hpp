
#pragma once

#include "Scene.hpp"
#include <ECS/System/Debug/DrawBoundingBox.hpp>
#include <ECS/System/Movement.hpp>

namespace Lumen {
namespace Scene {

class GamePlayScene : public Lumen::Scene::BaseScene {
private:
        bool m_is_initialized{false};
public:
        constexpr GamePlayScene(sf::RenderWindow *window_ptr,
                                Lumen::ECS::Entity::EntityManager *entity_manager_ptr,
                                Lumen::Scene::InterSceneCommunicationData *inter_scene_communication_data) noexcept
         : Lumen::Scene::BaseScene{Lumen::Scene::SceneID::GAME_PLAY,
                                   window_ptr,
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

                this->m_is_initialized = true;
        }

        constexpr void Movement(void) noexcept
        {
                assert(this->m_is_initialized);
                //std::cout << "[GamePlayScene] Movement\n";
                float delta_time = Lumen::Scene::BaseScene::m_inter_scene_communication_data->delta_time;
                Lumen::ECS::System::MovementForEach(*Lumen::Scene::BaseScene::m_entity_manager_ptr, delta_time);
        }

        constexpr void Update(void) noexcept override
        {
                assert(this->m_is_initialized);
                this->CreateActions();
                this->DoActions();
                this->Movement();
                auto &e = this->m_entity_manager_ptr->GetEntityCurrentlyControlledByThePlayer();
                auto &t = e.GetComponent<Lumen::ECS::Component::Transform>();
                std::cout << "[GamePlayScene] t.position{ " << t.position.x << ", " << t.position.y << "}\n";
        }

        constexpr void Render(void) noexcept override
        {
                assert(this->m_is_initialized);
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

        class TestUILayer : public Lumen::LayerStack::BaseLayer {
        private:
                GamePlayScene *m_game_play_scene;
        public:
                constexpr TestUILayer(GamePlayScene *game_play_scene) noexcept
                : m_game_play_scene{game_play_scene}
                {
                        assert(nullptr != this->m_game_play_scene);
                }

                constexpr void Update(void) noexcept override
                {
                        //std::cout << "[ExampleLayer1]: Update\n";
                }

                [[nodiscard]] constexpr Lumen::LayerStack::BaseLayer::DoActionResult DoAction(Lumen::Action::Action action) noexcept override
                {
                        switch (action.action_name) {
                        case Lumen::Action::ActionName::QUIT: {
                                std::cout << "[GamePlayScene:TestUILayer]: QUIT\n";
                                assert(nullptr != this->m_game_play_scene);
                                assert(nullptr != this->m_game_play_scene->m_inter_scene_communication_data);
                                this->m_game_play_scene->m_inter_scene_communication_data->change_scene = true;
                                auto &change_scene_args = this->m_game_play_scene->m_inter_scene_communication_data->change_scene_args;
                                change_scene_args.from_scene = Lumen::Scene::SceneID::GAME_PLAY;
                                change_scene_args.change_to_this_scene = Lumen::Scene::SceneID::MENU;
                                return Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked;
                        }
                        case Lumen::Action::ActionName::MOVE_UP:
                                std::cout << "[GamePlayScene:TestUILayer]: MOVE_UP\n";
                                return Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked;
                        
                        default:
                                break;
                        }
                        return Lumen::LayerStack::BaseLayer::DoActionResult::NotHandedOrNotBlocked;
                }
        };

        class TestBackgroundLayer : public Lumen::LayerStack::BaseLayer {
        public:

                constexpr void Update(void) noexcept override
                {
                        //std::cout << "[ExampleLayer2]: Update\n";
                }

                [[nodiscard]] constexpr Lumen::LayerStack::BaseLayer::DoActionResult DoAction(Lumen::Action::Action action) noexcept override
                {
                        switch (action.action_name) {
                        case Lumen::Action::ActionName::MOVE_RIGHT:
                                std::cout << "[GamePlayScene:TestBackgroundLayer]: MOVE_RIGHT\n";
                                return Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked;
                        
                        default:
                                break;
                        }
                        return Lumen::LayerStack::BaseLayer::DoActionResult::NotHandedOrNotBlocked;
                }
        };

        friend TestUILayer;

        class GameWorldLayer : public Lumen::LayerStack::BaseLayer {
        private:
                GamePlayScene *m_game_play_scene;
        public:
                constexpr GameWorldLayer(GamePlayScene *game_play_scene) noexcept
                : m_game_play_scene{game_play_scene}
                {
                        assert(nullptr != this->m_game_play_scene);
                }

                constexpr void Update(void) noexcept override
                {

                }

                constexpr void Render(void) noexcept override
                {

                }

                [[nodiscard]] constexpr Lumen::LayerStack::BaseLayer::DoActionResult DoAction([[maybe_unused]] Lumen::Action::Action action) noexcept override
                {
                        switch (action.action_name) {

                        //case Lumen::Action::ActionName::JUMP:
                        //        return Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked;

                        case Lumen::Action::ActionName::SQUAT:
                                // TODO:
                                return Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked;

                        case Lumen::Action::ActionName::SHOOT:
                                // TODO:
                                return Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked;

                        case Lumen::Action::ActionName::TOGGLE_DRAWING_BOUNDING_BOX:
                                // TODO:
                                return Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked;

                        case Lumen::Action::ActionName::TOGGLE_DRAWING_TEXTURE:
                                // TODO:
                                return Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked;

                        default:
                                break;
                        }
                        return Lumen::LayerStack::BaseLayer::DoActionResult::NotHandedOrNotBlocked;
                }

                [[nodiscard]] constexpr Lumen::LayerStack::BaseLayer::DoActionResult
                DoMovementAction([[maybe_unused]] Lumen::Action::MovementAction movement_action) noexcept override
                {
                        auto &entity_manager = *this->m_game_play_scene->m_entity_manager_ptr;//std::cout << "[GameWorldLayer] " << __LINE__ << "\n";
                        float speed = entity_manager.GetSpeedOfEntityCurrentlyControlledByThePlayer();//std::cout << "[GameWorldLayer] " << __LINE__ << "\n";
                        auto &player = entity_manager.GetEntityCurrentlyControlledByThePlayer();//std::cout << "[GameWorldLayer] " << __LINE__ << "\n";
                        auto &transform = player.GetComponent<Lumen::ECS::Component::Transform>();//std::cout << "[GameWorldLayer] " << __LINE__ << "\n";
                        if (!movement_action.has_movement) {
                                transform.velocity = {0.0f, 0.0f};//std::cout << "[GameWorldLayer] " << __LINE__ << "\n";
                                return Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked;
                        }
                        Lumen::Core::Math::Vec2f32 velocity{0.0f, 0.0f};
                        if (movement_action.is_move_up) {
                                velocity += {0.0f, -1.0f};
                        }
                        if (movement_action.is_move_down) {
                                velocity += {0.0f, 1.0f};
                        }
                        if (movement_action.is_move_left) {
                                velocity += {-1.0f, 0.0f};
                        }
                        if (movement_action.is_move_right) {
                                velocity += {1.0f, 0.0f};
                        }
                        //std::cout << "[GameWorldLayer] velocity{ " << velocity.x << ", " << velocity.y << "}\n";
                        constexpr float FLOAT_ZERO = 0.001f;
                        if (((velocity.x < FLOAT_ZERO) && (velocity.x > -FLOAT_ZERO)) &&
                            (velocity.y < FLOAT_ZERO) && (velocity.y > -FLOAT_ZERO)) {
                                return Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked;
                        }
                        
                        velocity.Normalize();
                        velocity *= speed;//std::cout << "[GameWorldLayer] " << __LINE__ << "\n";
                        std::cout << "[GameWorldLayer] velocity{ " << velocity.x << ", " << velocity.y << "}\n";
                        transform.velocity = velocity;//std::cout << "[GameWorldLayer] " << __LINE__ << "\n";
                        //std::cout << "[GameWorldLayer] " << __LINE__ << "\n";
                        return Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked;
                }
        };
        friend GameWorldLayer;
        
        constexpr void InitLayerStack(void) noexcept
        {
                assert(!this->m_is_initialized);
                Lumen::Scene::BaseScene::m_layer_stack.PushBackLayer(Lumen::LayerStack::MakeLayer<TestUILayer>(this));
                Lumen::Scene::BaseScene::m_layer_stack.PushBackLayer(Lumen::LayerStack::MakeLayer<GameWorldLayer>(this));
                Lumen::Scene::BaseScene::m_layer_stack.PushBackLayer(Lumen::LayerStack::MakeLayer<TestBackgroundLayer>());
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
                entity_manager.Update();
                std::cout << "[InitSpawnePlayer]\n";
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
                do {
                        const auto movement_action = Lumen::Scene::BaseScene::m_action_manager.GetMovementAction();
                        for (auto layer_stack_it = Lumen::Scene::BaseScene::m_layer_stack.rbegin();
                             layer_stack_it != Lumen::Scene::BaseScene::m_layer_stack.rend();
                             ++layer_stack_it) {
                                Lumen::LayerStack::LayerPtr &layer_ptr = (*layer_stack_it);
                                if (Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked == layer_ptr->DoMovementAction(movement_action)) {
                                        break;
                                }
                        }
                } while (false);
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
