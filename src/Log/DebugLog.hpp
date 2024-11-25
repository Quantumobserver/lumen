
#pragma once

#ifndef NDEBUG

#include <iostream>
#include <string>
#include <sstream>
#include <utility>

namespace Lumen {

namespace Debug {

template<typename T>
constexpr std::stringstream &CreateStringStream(std::stringstream &string_stream, T &&value) noexcept
{
        string_stream << value;

        return string_stream;
}

template<typename T, typename ...Args>
constexpr std::stringstream &CreateStringStream(std::stringstream &string_stream, T &&value, Args &&...args) noexcept
{
        string_stream << value;

        return CreateStringStream(string_stream, std::forward<Args>(args)...);
}

} // namespace Debug
} // namespace Lumen

#define LUMEN_DEBUG_LOG_OUT(stream, ...)                                        \
        do {                                                                    \
                std::stringstream string_stream{};                              \
                Lumen::Debug::CreateStringStream(string_stream, __VA_ARGS__);   \
                stream << string_stream.str();                                  \
        } while (0)

#define LUMEN_DEBUG_COUT(...) LUMEN_DEBUG_LOG_OUT(std::cout, __VA_ARGS__)
#define LUMEN_DEBUG_CERR(...) LUMEN_DEBUG_LOG_OUT(std::cerr, __VA_ARGS__)

#endif // !defined(NDEBUG)
