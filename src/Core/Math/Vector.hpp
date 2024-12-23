
#pragma once

#include <LumenDef/ConstexprIf.hpp>

#include <cmath>
#include <type_traits>

#include <glm/glm.hpp>

namespace Lumen {

namespace Core {

namespace Math {

template<typename Number>
constexpr Number Abs(Number number) noexcept
{
        return glm::abs(number);
}

struct Vec2f32;

namespace Detail {

namespace Vec2f32 {
template<typename U>
struct BaseCastIfImpl {
        using type = U;
};


template<>
struct BaseCastIfImpl<Lumen::Core::Math::Vec2f32> {
        using type = glm::vec2;
};

} // namespace Vec2f32
} // namespace Detail

struct Vec2f32 : private glm::vec2 {
private:
        template<typename GlmVecQualifier, GlmVecQualifier qualifier>
        constexpr Vec2f32(const glm::vec<2, float, qualifier> &glm_vec) noexcept
        : glm::vec2{glm_vec} {}

        template<typename T>
        struct BaseCastIf {
                using type = typename Lumen::Core::Math::Detail::Vec2f32::BaseCastIfImpl<
                                typename std::remove_cv<
                                        typename std::remove_reference<T>::type
                                >::type>::type;
        };


public:
        using glm::vec2::x;
        using glm::vec2::y;

        constexpr Vec2f32(void) noexcept : glm::vec2{} {}

        constexpr Vec2f32(float x, float y) noexcept
        : glm::vec2{x, y} {}

        constexpr Vec2f32(const Vec2f32 &vec2f) noexcept
        : glm::vec2{vec2f} {}

        constexpr Vec2f32 &operator=(const Vec2f32 &vec2f) noexcept = default;

        CONSTEXPR_IF_GLM_NORMALIZE Vec2f32 &Normalize(void) noexcept
        {
                *this = glm::normalize(*this);
                return *this;
        }

#define LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(unary_assign_operator, Argument, argument)                \
        constexpr Vec2f32 &operator unary_assign_operator(Argument argument) noexcept                                   \
        {                                                                                                               \
                glm::vec2::operator unary_assign_operator(static_cast<typename BaseCastIf<Argument>::type>(argument));  \
                return *this;                                                                                           \
        }

        // Unary arithmetic assign operators
        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(+=, float, scalar)
        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(+=, const Vec2f32 &, vec2f)

        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(-=, float, scalar)
        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(-=, const Vec2f32 &, vec2f)

        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(*=, float, scalar)
        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(*=, const Vec2f32 &, vec2f)

        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(/=, float, scalar)
        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(/=, const Vec2f32 &, vec2f)

        // Unary bit assign operators
        /*LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(%=, float, scalar)
        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(%=, const Vec2f32 &, vec2f)

        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(&=, float, scalar)
        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(&=, const Vec2f32 &, vec2f)

        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(|=, float, scalar)
        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(|=, const Vec2f32 &, vec2f)

        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(^=, float, scalar)
        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(^=, const Vec2f32 &, vec2f)

        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(<<=, float, scalar)
        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(<<=, const Vec2f32 &, vec2f)

        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(>>=, float, scalar)
        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(>>=, const Vec2f32 &, vec2f)*/

#undef LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR

        /*constexpr Vec2f32 &operator+=(float scalar) noexcept
        {
                glm::vec2::operator+=(scalar);
                return *this;
        }

        constexpr Vec2f32 &operator+=(const Vec2f32 &vec2f) noexcept
        {
                glm::vec2::operator+=(vec2f);
                return *this;
        }

        constexpr Vec2f32 &operator-=(float scalar) noexcept
        {
                glm::vec2::operator-=(scalar);
                return *this;
        }*/

        constexpr Vec2f32 &operator++(void) noexcept
        {
                glm::vec2::operator++();
                return *this;
        }

        constexpr Vec2f32 &operator--(void) noexcept
        {
                glm::vec2::operator--();
                return *this;
        }

        //friend constexpr Vec2f operator+(const Vec2f &vec2f) noexcept;
        //friend constexpr Vec2f32 operator-(const Vec2f32 &vec2f) noexcept;

        //friend constexpr Vec2f32 operator+(const Vec2f32 &vec2f, float scalar) noexcept;
        //friend constexpr Vec2f32 operator+(const Vec2f32 &vec2f_1, const Vec2f32 &vec2f_2) noexcept;

        //friend constexpr Vec2f32 operator-(const Vec2f32 &vec2f, float scalar) noexcept;
        //friend constexpr Vec2f32 operator-(const Vec2f32 &vec2f_1, const Vec2f32 &vec2f_2) noexcept;

#define LUMEN_CORE_MATH_VEC2F_UNARY_OPERATOR_GENERATOR(unary_operator, Argument, argument)                              \
        friend constexpr Vec2f32 operator unary_operator(Argument argument) noexcept                                      \
        {                                                                                                               \
                return glm::operator unary_operator(static_cast<typename BaseCastIf<Argument>::type>(argument));        \
        }

        // Unary operators
        LUMEN_CORE_MATH_VEC2F_UNARY_OPERATOR_GENERATOR(+, const Vec2f32 &, vec2f)
        LUMEN_CORE_MATH_VEC2F_UNARY_OPERATOR_GENERATOR(-, const Vec2f32 &, vec2f)

#undef LUMEN_CORE_MATH_VEC2F_UNARY_OPERATOR_GENERATOR

#define LUMEN_CORE_MATH_VEC2F_BINARY_OPERATOR_GENERATOR(binary_operator, Argument1, argument1, Argument2, argument2)    \
        friend constexpr Vec2f32 operator binary_operator(Argument1 argument1, Argument2 argument2) noexcept              \
        {                                                                                                               \
                return glm::operator binary_operator(static_cast<typename BaseCastIf<Argument1>::type>(argument1),      \
                                                     static_cast<typename BaseCastIf<Argument2>::type>(argument2));     \
        }

        // Binary operators
        LUMEN_CORE_MATH_VEC2F_BINARY_OPERATOR_GENERATOR(+, const Vec2f32 &, vec2f, float, scalar)
        LUMEN_CORE_MATH_VEC2F_BINARY_OPERATOR_GENERATOR(+, float, scalar, const Vec2f32 &, vec2f)
        LUMEN_CORE_MATH_VEC2F_BINARY_OPERATOR_GENERATOR(+, const Vec2f32 &, vec2f_1, const Vec2f32 &, vec2f_2)

        LUMEN_CORE_MATH_VEC2F_BINARY_OPERATOR_GENERATOR(-, const Vec2f32 &, vec2f, float, scalar)
        LUMEN_CORE_MATH_VEC2F_BINARY_OPERATOR_GENERATOR(-, float, scalar, const Vec2f32 &, vec2f)
        LUMEN_CORE_MATH_VEC2F_BINARY_OPERATOR_GENERATOR(-, const Vec2f32 &, vec2f_1, const Vec2f32 &, vec2f_2)

        LUMEN_CORE_MATH_VEC2F_BINARY_OPERATOR_GENERATOR(*, const Vec2f32 &, vec2f, float, scalar)
        LUMEN_CORE_MATH_VEC2F_BINARY_OPERATOR_GENERATOR(*, float, scalar, const Vec2f32 &, vec2f)
        LUMEN_CORE_MATH_VEC2F_BINARY_OPERATOR_GENERATOR(*, const Vec2f32 &, vec2f_1, const Vec2f32 &, vec2f_2)

        LUMEN_CORE_MATH_VEC2F_BINARY_OPERATOR_GENERATOR(/, const Vec2f32 &, vec2f, float, scalar)
        LUMEN_CORE_MATH_VEC2F_BINARY_OPERATOR_GENERATOR(/, float, scalar, const Vec2f32 &, vec2f)
        LUMEN_CORE_MATH_VEC2F_BINARY_OPERATOR_GENERATOR(/, const Vec2f32 &, vec2f_1, const Vec2f32 &, vec2f_2)

#undef LUMEN_CORE_MATH_VEC2F_BINARY_OPERATOR_GENERATOR
};

struct Vec2i;

namespace Detail {

namespace Vec2i {

template<typename U>
struct BaseCastIfImpl {
        using type = U;
};

template<>
struct BaseCastIfImpl<Lumen::Core::Math::Vec2i> {
        using type = glm::ivec2;
};

} // namespace Vec2i
} // namespace Detail

struct Vec2i : private glm::ivec2 {
private:
        template<typename GlmVecQualifier, GlmVecQualifier qualifier>
        constexpr Vec2i(const glm::vec<2, int, qualifier> &glm_vec) noexcept
        : glm::ivec2{glm_vec} {}

        template<typename T>
        struct BaseCastIf {
                using type = typename Lumen::Core::Math::Detail::Vec2i::BaseCastIfImpl<
                                typename std::remove_cv<
                                        typename std::remove_reference<T>::type
                                >::type>::type;
        };


public:
        using glm::ivec2::x;
        using glm::ivec2::y;

        constexpr Vec2i(void) noexcept : glm::ivec2{} {}

        constexpr Vec2i(int x, int y) noexcept : glm::ivec2{x, y} {}

        constexpr Vec2i(const Vec2i &vec2i) noexcept : glm::ivec2{vec2i} {}

        constexpr Vec2i &operator=(const Vec2i &vec2i) noexcept = default;

#define LUMEN_CORE_MATH_VEC2I_UNARY_ASSIGN_OPERATOR_GENERATOR(unary_assign_operator, Argument, argument)                \
        constexpr Vec2i &operator unary_assign_operator(Argument argument) noexcept                                     \
        {                                                                                                               \
                glm::ivec2::operator unary_assign_operator(static_cast<typename BaseCastIf<Argument>::type>(argument)); \
                return *this;                                                                                           \
        }

        // Unary arithmetic assign operators
        LUMEN_CORE_MATH_VEC2I_UNARY_ASSIGN_OPERATOR_GENERATOR(+=, int, scalar)
        LUMEN_CORE_MATH_VEC2I_UNARY_ASSIGN_OPERATOR_GENERATOR(+=, const Vec2i &, vec2i)

        LUMEN_CORE_MATH_VEC2I_UNARY_ASSIGN_OPERATOR_GENERATOR(-=, int, scalar)
        LUMEN_CORE_MATH_VEC2I_UNARY_ASSIGN_OPERATOR_GENERATOR(-=, const Vec2i &, vec2i)

        LUMEN_CORE_MATH_VEC2I_UNARY_ASSIGN_OPERATOR_GENERATOR(*=, int, scalar)
        LUMEN_CORE_MATH_VEC2I_UNARY_ASSIGN_OPERATOR_GENERATOR(*=, const Vec2i &, vec2i)

        LUMEN_CORE_MATH_VEC2I_UNARY_ASSIGN_OPERATOR_GENERATOR(/=, int, scalar)
        LUMEN_CORE_MATH_VEC2I_UNARY_ASSIGN_OPERATOR_GENERATOR(/=, const Vec2i &, vec2i)

        // Unary bit assign operators
        LUMEN_CORE_MATH_VEC2I_UNARY_ASSIGN_OPERATOR_GENERATOR(%=, int, scalar)
        LUMEN_CORE_MATH_VEC2I_UNARY_ASSIGN_OPERATOR_GENERATOR(%=, const Vec2i &, vec2i)

        LUMEN_CORE_MATH_VEC2I_UNARY_ASSIGN_OPERATOR_GENERATOR(&=, int, scalar)
        LUMEN_CORE_MATH_VEC2I_UNARY_ASSIGN_OPERATOR_GENERATOR(&=, const Vec2i &, vec2i)

        LUMEN_CORE_MATH_VEC2I_UNARY_ASSIGN_OPERATOR_GENERATOR(|=, int, scalar)
        LUMEN_CORE_MATH_VEC2I_UNARY_ASSIGN_OPERATOR_GENERATOR(|=, const Vec2i &, vec2i)

        LUMEN_CORE_MATH_VEC2I_UNARY_ASSIGN_OPERATOR_GENERATOR(^=, int, scalar)
        LUMEN_CORE_MATH_VEC2I_UNARY_ASSIGN_OPERATOR_GENERATOR(^=, const Vec2i &, vec2i)

        LUMEN_CORE_MATH_VEC2I_UNARY_ASSIGN_OPERATOR_GENERATOR(<<=, int, scalar)
        LUMEN_CORE_MATH_VEC2I_UNARY_ASSIGN_OPERATOR_GENERATOR(<<=, const Vec2i &, vec2i)

        LUMEN_CORE_MATH_VEC2I_UNARY_ASSIGN_OPERATOR_GENERATOR(>>=, int, scalar)
        LUMEN_CORE_MATH_VEC2I_UNARY_ASSIGN_OPERATOR_GENERATOR(>>=, const Vec2i &, vec2i)

#undef LUMEN_CORE_MATH_VEC2I_UNARY_ASSIGN_OPERATOR_GENERATOR

        constexpr Vec2i &operator++(void) noexcept
        {
                glm::ivec2::operator++();
                return *this;
        }

        constexpr Vec2i &operator--(void) noexcept
        {
                glm::ivec2::operator--();
                return *this;
        }

#define LUMEN_CORE_MATH_VEC2I_UNARY_OPERATOR_GENERATOR(unary_operator, Argument, argument)                              \
        friend constexpr Vec2i operator unary_operator(Argument argument) noexcept                                      \
        {                                                                                                               \
                return glm::operator unary_operator(static_cast<typename BaseCastIf<Argument>::type>(argument));        \
        }

        // Unary operators
        LUMEN_CORE_MATH_VEC2I_UNARY_OPERATOR_GENERATOR(+, const Vec2i &, vec2i)
        LUMEN_CORE_MATH_VEC2I_UNARY_OPERATOR_GENERATOR(-, const Vec2i &, vec2i)

#undef LUMEN_CORE_MATH_VEC2I_UNARY_OPERATOR_GENERATOR

#define LUMEN_CORE_MATH_VEC2I_BINARY_OPERATOR_GENERATOR(binary_operator, Argument1, argument1, Argument2, argument2)    \
        friend constexpr Vec2i operator binary_operator(Argument1 argument1, Argument2 argument2) noexcept              \
        {                                                                                                               \
                return glm::operator binary_operator(static_cast<typename BaseCastIf<Argument1>::type>(argument1),      \
                                                     static_cast<typename BaseCastIf<Argument2>::type>(argument2));     \
        }

        // Binary operators
        LUMEN_CORE_MATH_VEC2I_BINARY_OPERATOR_GENERATOR(+, const Vec2i &, vec2i, int, scalar)
        LUMEN_CORE_MATH_VEC2I_BINARY_OPERATOR_GENERATOR(+, int, scalar, const Vec2i &, vec2i)
        LUMEN_CORE_MATH_VEC2I_BINARY_OPERATOR_GENERATOR(+, const Vec2i &, vec2i_1, const Vec2i &, vec2i_2)

        LUMEN_CORE_MATH_VEC2I_BINARY_OPERATOR_GENERATOR(-, const Vec2i &, vec2i, int, scalar)
        LUMEN_CORE_MATH_VEC2I_BINARY_OPERATOR_GENERATOR(-, int, scalar, const Vec2i &, vec2i)
        LUMEN_CORE_MATH_VEC2I_BINARY_OPERATOR_GENERATOR(-, const Vec2i &, vec2i_1, const Vec2i &, vec2i_2)

        LUMEN_CORE_MATH_VEC2I_BINARY_OPERATOR_GENERATOR(*, const Vec2i &, vec2i, int, scalar)
        LUMEN_CORE_MATH_VEC2I_BINARY_OPERATOR_GENERATOR(*, int, scalar, const Vec2i &, vec2i)
        LUMEN_CORE_MATH_VEC2I_BINARY_OPERATOR_GENERATOR(*, const Vec2i &, vec2i_1, const Vec2i &, vec2i_2)

        LUMEN_CORE_MATH_VEC2I_BINARY_OPERATOR_GENERATOR(/, const Vec2i &, vec2i, int, scalar)
        LUMEN_CORE_MATH_VEC2I_BINARY_OPERATOR_GENERATOR(/, int, scalar, const Vec2i &, vec2i)
        LUMEN_CORE_MATH_VEC2I_BINARY_OPERATOR_GENERATOR(/, const Vec2i &, vec2i_1, const Vec2i &, vec2i_2)

#undef LUMEN_CORE_MATH_VEC2I_BINARY_OPERATOR_GENERATOR

};

CONSTEXPR_IF_CXX_23 Vec2i Abs(const Vec2i &vec2i) noexcept
{
        return {std::abs(vec2i.x), std::abs(vec2i.y)};
}

} // namespace Math
} // namespace Core
} // namespace Lumen
