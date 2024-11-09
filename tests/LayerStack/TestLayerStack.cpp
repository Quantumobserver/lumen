
#define LUMEN_DEBUG_ACTION_FORMAT_ENABLED

#include <Action/ActionManager.hpp>
#include <LayerStack/LayerStack.hpp>
#include "Render/Window/RenderWindow.hpp"

#include <iostream>

#include <SFML/Window/Window.hpp>
#include <SFML/Window/Keyboard.hpp>

class ExampleLayer1 : public Lumen::LayerStack::BaseLayer {
public:

        constexpr void Update(void) noexcept override
        {
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
                return Lumen::LayerStack::BaseLayer::DoActionResult::NotHandedOrNotBlocked;
        }
};

class ExampleLayer2 : public Lumen::LayerStack::BaseLayer {
public:

        constexpr void Update(void) noexcept override
        {
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
                return Lumen::LayerStack::BaseLayer::DoActionResult::NotHandedOrNotBlocked;
        }
};

void TestLayerStackCase1(void) noexcept
{
        Lumen::RenderWindow window{Lumen::VideoMode{{800, 600}}, Lumen::WindowTitle{"My window"}};
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

        //sf::Image image{};
        // run the program as long as the window is open
        while (window.IsOpen()) {
                // check all the window's events that were triggered since the last iteration of the loop
                for (Lumen::Event event = window.PollEvent(); event.HasEvent(); event = window.PollEvent()) {

                        if (event.IsThisEventType<sf::Event::KeyPressed>()) {
                                const sf::Event::KeyPressed key_pressed_data = event.GetEventData<sf::Event::KeyPressed>();
                                if (sf::Keyboard::Key::A == key_pressed_data.code) {
                                        std::cout << "[SFML KeyPressed]: A\n";

                                }
                                if (sf::Keyboard::Key::Escape == key_pressed_data.code) {
                                        window.CloseWindow();
                                }
                        } else if (event.IsThisEventType<sf::Event::KeyReleased>()) {
                                const sf::Event::KeyReleased key_pressed_data = event.GetEventData<sf::Event::KeyReleased>();
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

                for (auto &LayerPtr : layer_stack) {
                        LayerPtr->Update();
                }
                //std::cout << "\n";
        }
}

int main(void)
{
        TestLayerStackCase1();
        std::cout << "Done\n";
}
