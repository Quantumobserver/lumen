
#pragma once

#include <string>
#include <utility>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Image.hpp>

#include "Event/Event.hpp"

namespace Lumen {

class RenderWindow;
class VideoMode;

class WindowSize : private sf::Vector2u {
        friend RenderWindow;
        friend VideoMode;
        constexpr WindowSize(const sf::Vector2u &vector2u) noexcept : sf::Vector2u{vector2u} {}
public:
        constexpr WindowSize(unsigned int width, unsigned int height) noexcept : sf::Vector2u{width, height} {}
};

class VideoMode : private sf::VideoMode {
        friend RenderWindow;

        template<typename T>
        struct BaseCastIf {
        private:
                template<typename U>
                struct BaseCastIfImpl {
                        using type = U;
                };

                template<>
                struct BaseCastIfImpl<WindowSize> {
                        using type = sf::Vector2u;
                };
        
        public:
                using type = typename BaseCastIfImpl<
                                typename std::remove_cv<
                                        typename std::remove_reference<T>::type
                                >::type>::type;
        };

public:
        constexpr VideoMode(void) noexcept = default;
        VideoMode(const WindowSize &window_size, unsigned int pixel_depths_in_bits_per_pixel = 32) noexcept
        : sf::VideoMode{static_cast<typename BaseCastIf<decltype(window_size)>::type>(window_size),
                        pixel_depths_in_bits_per_pixel} {}
};

class WindowTitle : private sf::String {
        friend RenderWindow;

public:
        WindowTitle(const char* string) noexcept : sf::String{string} {}
};

class WindowStyle {
public:
        friend RenderWindow;

        using flag_type = std::uint32_t;

        enum WindowStyleFlag : flag_type {
                NONE     = 0,      //!< No border / title bar (this flag and all others are mutually exclusive)
                HAS_TITLE_BAR = 1 << 0, //!< Title bar + fixed border
                HAS_RESIZEABLE_BAR   = 1 << 1, //!< Title bar + resizable border + maximize button
                HAS_CLOSE_BUTTON    = 1 << 2, //!< Title bar + close button

                DEFAULT = HAS_TITLE_BAR | HAS_RESIZEABLE_BAR | HAS_CLOSE_BUTTON, //!< Default window style
        };

private:
        flag_type m_window_style;

        [[nodiscard]] constexpr flag_type operator()(void) const noexcept
        {
                return this->m_window_style;
        }

public:
        constexpr WindowStyle(flag_type flag) noexcept : m_window_style{flag} {}

        static const WindowStyle DEFAULT_WINDOW_STYLE;
};
const WindowStyle WindowStyle::DEFAULT_WINDOW_STYLE{WindowStyleFlag::DEFAULT};

enum class WindowState {
        FloatingWindow = static_cast<int>(sf::State::Windowed),
        Fullscreen = static_cast<int>(sf::State::Fullscreen),
};

class WindowIcon : sf::Image {
        friend RenderWindow;
};

class WindowIconSize : private sf::Vector2u {
        friend RenderWindow;
        constexpr WindowIconSize(const sf::Vector2u &vector2u) noexcept : sf::Vector2u{vector2u} {}
public:
};

class WindowPosition : private sf::Vector2i {
        friend RenderWindow;
        constexpr WindowPosition(const sf::Vector2i &vector2i) noexcept : sf::Vector2i{vector2i} {}
};

class Color : private sf::Color {
private:
        friend RenderWindow;

public:
        constexpr Color(void) = default;
        constexpr Color(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t alpha = 255) noexcept
        : sf::Color{red, green, blue, alpha} {}

};

class WindowView : private sf::View {
        friend RenderWindow;

        constexpr WindowView(const sf::View &view) noexcept : sf::View{view} {}
};

class WindowViewport : private sf::IntRect {
        friend RenderWindow;
        constexpr WindowViewport(const sf::IntRect &int_rect) noexcept : sf::IntRect{int_rect} {}
};

using Pixel = std::uint8_t;

//#define constexpr

class RenderWindow : private sf::RenderWindow {
private:
        template<typename T>
        struct BaseCastIf {
        private:
                template<typename U>
                struct BaseCastIfImpl {
                        using type = U;
                };

                template<>
                struct BaseCastIfImpl<VideoMode> {
                        using type = sf::VideoMode;
                };

                template<>
                struct BaseCastIfImpl<WindowTitle> {
                        using type = sf::String;
                };

                template<>
                struct BaseCastIfImpl<WindowSize> {
                        using type = sf::Vector2u;
                };

                template<>
                struct BaseCastIfImpl<WindowIcon> {
                        using type = sf::Image;
                };

                template<>
                struct BaseCastIfImpl<WindowIconSize> {
                        using type = sf::Vector2u;
                };

                template<>
                struct BaseCastIfImpl<WindowPosition> {
                        using type = sf::Vector2i;
                };

                template<>
                struct BaseCastIfImpl<WindowView> {
                        using type = sf::View;
                };
        
        public:
                using type = typename BaseCastIfImpl<
                                typename std::remove_cv<
                                        typename std::remove_reference<T>::type
                                >::type>::type;
        };
public:
        using titlt_type = Lumen::WindowTitle;

        RenderWindow(void) = default;

        RenderWindow(Lumen::VideoMode video_mode,
                     Lumen::WindowTitle &&title,
                     Lumen::WindowStyle style = Lumen::WindowStyle::DEFAULT_WINDOW_STYLE,
                     Lumen::WindowState state = Lumen::WindowState::FloatingWindow) noexcept
        : sf::RenderWindow{static_cast<typename BaseCastIf<decltype(video_mode)>::type>(video_mode),
                           std::move(static_cast<typename BaseCastIf<decltype(title)>::type>(title)),
                           style(), static_cast<sf::State>(state)} {}

        RenderWindow(Lumen::VideoMode video_mode, Lumen::WindowTitle &&title, Lumen::WindowState state) noexcept
        : sf::RenderWindow{static_cast<typename BaseCastIf<decltype(video_mode)>::type>(video_mode),
                           std::move(static_cast<typename BaseCastIf<decltype(title)>::type>(title)),
                           static_cast<sf::State>(state)} {}

        [[nodiscard]] constexpr Lumen::WindowSize GetWindowSize(void) const noexcept
        {
                return {sf::RenderWindow::getSize()};
        }

        constexpr void SetWindowIcon(Lumen::WindowIcon &&icon) noexcept
        {
                sf::RenderWindow::setIcon(static_cast<typename BaseCastIf<decltype(icon)>::type>(icon));
        }

        /*constexpr void SetWindowIcon(WindowIconSize icon_size, const Pixel *pixels) noexcept
        {
                 sf::WindowBase::setIcon(static_cast<typename BaseCastIf<decltype(icon_size)>::type>(icon_size), pixels);
        }*/

        [[nodiscard]] constexpr bool IsSrgb(void) const noexcept
        {
                return sf::RenderWindow::isSrgb();
        }

        constexpr void CloseWindow(void) noexcept
        {
                sf::Window::close();
        }
        
        constexpr void CreateWindow(Lumen::VideoMode mode,
                                    Lumen::WindowTitle &&title,
                                    Lumen::WindowStyle style = Lumen::WindowStyle::DEFAULT_WINDOW_STYLE,
                                    Lumen::WindowState state = Lumen::WindowState::FloatingWindow) noexcept
        {
                sf::Window::create(static_cast<typename BaseCastIf<decltype(mode)>::type>(mode),
                                   static_cast<typename BaseCastIf<decltype(title)>::type>(title),
                                   style(), static_cast<sf::State>(state),
                                   sf::ContextSettings{});
        }

        constexpr void SetVerticalSyncEnabled(bool enabled) noexcept
        {
                sf::Window::setVerticalSyncEnabled(enabled);
        }

        constexpr void SetFramerateLimit(unsigned int frame_rate_limit) noexcept
        {
                sf::Window::setFramerateLimit(frame_rate_limit);
        }

        constexpr void Display(void) noexcept
        {
                sf::Window::display();
        }

        constexpr bool IsOpen(void) const noexcept
        {
                return sf::WindowBase::isOpen();
        }

        constexpr Lumen::Event PollEvent(void) noexcept
        {
                return {sf::WindowBase::pollEvent()};
        }

        // TODO:
        //std::optional<Event> WindowBase::waitEvent(Time timeout)

        constexpr Lumen::WindowPosition GetWindowPosition(void) const noexcept
        {
                return {sf::WindowBase::getPosition()};
        }

        constexpr void SetWindowPosition(const Lumen::WindowPosition &position) noexcept
        {
                sf::WindowBase::setPosition(static_cast<typename BaseCastIf<decltype(position)>::type>(position));
        }

        constexpr void SetWindowSize(const Lumen::WindowIconSize &size) noexcept
        {
                sf::WindowBase::setSize(static_cast<typename BaseCastIf<decltype(size)>::type>(size));
        }

        constexpr void SetWindowTitle(Lumen::WindowTitle &&title) noexcept
        {
                sf::WindowBase::setTitle(std::move(static_cast<typename BaseCastIf<decltype(title)>::type>(title)));
        }

        constexpr void SetWindowIcon(const Lumen::WindowIconSize icon_size, const Pixel *pixels) noexcept
        {
                sf::WindowBase::setIcon(static_cast<typename BaseCastIf<decltype(icon_size)>::type>(icon_size), pixels);
        }

        /*constexpr void SetWindowIcon(Lumen::WindowIcon &&icon) noexcept
        {
                // TODO: 
                //sf::WindowBase::setIcon(static_cast<typename BaseCastIf<decltype(icon_size)>::type>(icon_size), pixels);
        }*/

        constexpr void SetWindowVisible(bool visible) noexcept
        {
                sf::WindowBase::setVisible(visible);
        }

        constexpr void SetMouseCursorVisible(bool visible) noexcept
        {
                sf::WindowBase::setMouseCursorVisible(visible);
        }

        constexpr void SetMouseCursorGrabbed(bool grabbed) noexcept
        {
                sf::WindowBase::setMouseCursorGrabbed(grabbed);
        }

        constexpr void SetKeyRepeatEnabled(bool enabled) noexcept
        {
                sf::WindowBase::setKeyRepeatEnabled(enabled);
        }

        // TODO:
        // void sf::WindowBase::setJoystickThreshold(float threshold);

        constexpr void RequestInputFocus(void) noexcept
        {
                sf::WindowBase::requestFocus();
        }

        [[nodiscard]] constexpr bool HasInputFocus(void) const noexcept
        {
                return sf::WindowBase::hasFocus();
        }

        constexpr void ClearScreen(const Lumen::Color &color = Lumen::Color{0, 0, 0, 255}) noexcept
        {
                sf::RenderTarget::clear(static_cast<typename BaseCastIf<decltype(color)>::type>(color));
        }

        constexpr void SetWindowView(const Lumen::WindowView &view) noexcept
        {
                sf::RenderTarget::setView(static_cast<typename BaseCastIf<decltype(view)>::type>(view));
        }

        constexpr Lumen::WindowView GetWindowView(void) const noexcept
        {
                return {sf::RenderTarget::getView()};
        }

        constexpr Lumen::WindowViewport GetWindowViewport(const Lumen::WindowView &view) const noexcept
        {
                return {sf::RenderTarget::getViewport(static_cast<typename BaseCastIf<decltype(view)>::type>(view))};
        }

        constexpr void Draw(const sf::Drawable &drawable) noexcept
        {
                sf::RenderTarget::draw(drawable);
        }

        /*constexpr void Draw(const Vertex *vertices, std::size_t vertexCount, PrimitiveType type) noexcept
        {
                sf::RenderTarget::draw(const Vertex *vertices, std::size_t vertexCount, PrimitiveType type, const RenderStates &states=RenderStates::Default)
        }*/
};
//#undef constexpr

} // namespace Lumen
