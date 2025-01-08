
#define LUMEN_DEBUG_ACTION_FORMAT_ENABLED

#include <Action/ActionManager.hpp>
#include <LayerStack/LayerStack.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <iostream>

#include <SFML/Window/Window.hpp>
#include <SFML/Window/Keyboard.hpp>

class ExampleLayer1 : public Lumen::LayerStack::BaseLayer {
public:

        constexpr void Update(float delta_time) noexcept override
        {
                (void)delta_time;
                //std::cout << "[ExampleLayer1]: Update\n";
        }

        [[nodiscard]] constexpr Lumen::LayerStack::BaseLayer::DoActionResult DoAction(Lumen::Action::Action action) noexcept override
        {
                switch (action.action_name) {
                case Lumen::Action::ActionName::MOVE_UP:
                        std::cout << "[ExampleLayer1]: { " << Lumen::Action::fmt(action.action_name) << ", "
                                  << Lumen::Action::fmt(action.action_status) << " }\n";
                        return Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked;
                
                default:
                        break;
                }
                return Lumen::LayerStack::BaseLayer::DoActionResult::NotHandledOrNotBlocked;
        }
};

class ExampleLayer2 : public Lumen::LayerStack::BaseLayer {
public:

        constexpr void Update(float delta_time) noexcept override
        {
                (void)delta_time;
                //std::cout << "[ExampleLayer2]: Update\n";
        }

        [[nodiscard]] constexpr Lumen::LayerStack::BaseLayer::DoActionResult DoAction(Lumen::Action::Action action) noexcept override
        {
                switch (action.action_name) {
                case Lumen::Action::ActionName::MOVE_RIGHT:
                        std::cout << "[ExampleLayer2]: { " << Lumen::Action::fmt(action.action_name) << ", "
                                  << Lumen::Action::fmt(action.action_status) << " }\n";
                        return Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked;
                
                default:
                        break;
                }
                return Lumen::LayerStack::BaseLayer::DoActionResult::NotHandledOrNotBlocked;
        }
};

void TestLayerStackCase1(void) noexcept
{
        sf::RenderWindow window{sf::VideoMode{{800, 600}}, {"My window"}};
        Lumen::Action::ActionManager action_manager{};

        Lumen::LayerStack::LayerStack layer_stack{};
        layer_stack.PushBackLayer(Lumen::LayerStack::MakeLayer<ExampleLayer1>());
        layer_stack.PushBackLayer(Lumen::LayerStack::MakeLayer<ExampleLayer2>());

        action_manager.Init();

        action_manager.RegisteAction(sf::Keyboard::Key::W, Lumen::Action::ActionName::MOVE_UP);
        action_manager.RegisteAction(sf::Keyboard::Key::A, Lumen::Action::ActionName::MOVE_LEFT);
        action_manager.RegisteAction(sf::Keyboard::Key::S, Lumen::Action::ActionName::MOVE_DOWN);
        action_manager.RegisteAction(sf::Keyboard::Key::D, Lumen::Action::ActionName::MOVE_RIGHT);

        action_manager.RegisteAction(sf::Keyboard::Key::G, Lumen::Action::ActionName::TOGGLE_DRAWING_GRID);
        action_manager.SetActionKind(Lumen::Action::ActionName::TOGGLE_DRAWING_GRID, Lumen::Action::ActionKind::TRIGGER);

        auto last_update_time_point = std::chrono::high_resolution_clock::now();

        while (window.isOpen()) {
                for (std::optional<sf::Event> optional_event = window.pollEvent(); optional_event.has_value(); optional_event = window.pollEvent()) {
                        sf::Event &event = optional_event.value();
                        if (event.is<sf::Event::KeyPressed>()) {
                                const sf::Event::KeyPressed &key_pressed_data = *event.getIf<sf::Event::KeyPressed>();
                                if (sf::Keyboard::Key::A == key_pressed_data.code) {
                                        std::cout << "[SFML KeyPressed]: A\n";

                                }
                                if (sf::Keyboard::Key::Escape == key_pressed_data.code) {
                                        window.close();
                                }
                        } else if (event.is<sf::Event::KeyReleased>()) {
                                const sf::Event::KeyReleased key_pressed_data = *event.getIf<sf::Event::KeyReleased>();
                                if (sf::Keyboard::Key::A == key_pressed_data.code) {
                                        std::cout << "[SFML KeyReleased]: A\n";
                                }
                        }

                        action_manager.CreateActionFromEvent(event);
                        std::cout << "[Update Action]: \n";
                        for (auto action : action_manager) {
                                auto end = layer_stack.rend();
                                for (auto it = layer_stack.rbegin(); it != end; ++it) {
                                        if (Lumen::LayerStack::BaseLayer::DoActionResult::HandledOrBlocked == (*it)->DoAction(action)) {
                                                break;
                                        }
                                }
                        }
                        std::cout << "\n";

                        std::cout << "\n";
                }
                auto current_time_point = std::chrono::high_resolution_clock::now();
                auto delta_time = std::chrono::duration<float>{current_time_point - last_update_time_point}.count();
                last_update_time_point = current_time_point;

                for (auto &LayerPtr : layer_stack) {
                        LayerPtr->Update(delta_time);
                }
                //std::cout << "\n";
        }
}

int main(void)
{
        TestLayerStackCase1();
        std::cout << "Done\n";
}
