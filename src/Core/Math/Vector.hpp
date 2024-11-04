
#include <type_traits>

#include <glm/vec2.hpp>

namespace Lumen {

namespace Core {

namespace Math {

struct Vec2f : private glm::vec2 {
private:
        template<typename GlmVecQualifier, GlmVecQualifier qualifier>
        constexpr Vec2f(const glm::vec<2, float, qualifier> &glm_vec) noexcept
        : glm::vec2{glm_vec} {}

        template<typename T>
        struct BaseCastIf {
        private:
                template<typename U>
                struct BaseCastIfImpl {
                        using type = U;
                };

                template<>
                struct BaseCastIfImpl<Vec2f> {
                        using type = glm::vec2;
                };
        
        public:
                using type = typename BaseCastIfImpl<
                                typename std::remove_cv<
                                        typename std::remove_reference<T>::type
                                >::type>::type;
        };


public:
        using glm::vec2::x;
        using glm::vec2::y;

        constexpr Vec2f(void) noexcept : glm::vec2{} {}

        constexpr Vec2f(float x, float y) noexcept
        : glm::vec2{x, y} {}

        constexpr Vec2f(const Vec2f &vec2f) noexcept
        : glm::vec2{vec2f} {}

        constexpr Vec2f &operator=(const Vec2f &vec2f) noexcept = default;

#define LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(unary_assign_operator, Argument, argument)                \
        constexpr Vec2f &operator unary_assign_operator(Argument argument) noexcept                                     \
        {                                                                                                               \
                glm::vec2::operator unary_assign_operator(static_cast<typename BaseCastIf<Argument>::type>(argument));  \
                return *this;                                                                                           \
        }

        // Unary arithmetic assign operators
        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(+=, float, scalar)
        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(+=, const Vec2f &, vec2f)

        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(-=, float, scalar)
        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(-=, const Vec2f &, vec2f)

        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(*=, float, scalar)
        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(*=, const Vec2f &, vec2f)

        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(/=, float, scalar)
        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(/=, const Vec2f &, vec2f)

        // Unary bit assign operators
        /*LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(%=, float, scalar)
        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(%=, const Vec2f &, vec2f)

        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(&=, float, scalar)
        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(&=, const Vec2f &, vec2f)

        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(|=, float, scalar)
        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(|=, const Vec2f &, vec2f)

        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(^=, float, scalar)
        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(^=, const Vec2f &, vec2f)

        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(<<=, float, scalar)
        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(<<=, const Vec2f &, vec2f)

        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(>>=, float, scalar)
        LUMEN_CORE_MATH_VEC2F_UNARY_ASSIGN_OPERATOR_GENERATOR(>>=, const Vec2f &, vec2f)*/

#undef LUMEN_CORE_MATH_VEC2F_UNARYC_ASSIGN_OPERATOR_GENERATOR

        /*constexpr Vec2f &operator+=(float scalar) noexcept
        {
                glm::vec2::operator+=(scalar);
                return *this;
        }

        constexpr Vec2f &operator+=(const Vec2f &vec2f) noexcept
        {
                glm::vec2::operator+=(vec2f);
                return *this;
        }

        constexpr Vec2f &operator-=(float scalar) noexcept
        {
                glm::vec2::operator-=(scalar);
                return *this;
        }*/

        constexpr Vec2f &operator++(void) noexcept
        {
                glm::vec2::operator++();
                return *this;
        }

        constexpr Vec2f &operator--(void) noexcept
        {
                glm::vec2::operator--();
                return *this;
        }

        //friend constexpr Vec2f operator+(const Vec2f &vec2f) noexcept;
        //friend constexpr Vec2f operator-(const Vec2f &vec2f) noexcept;

        //friend constexpr Vec2f operator+(const Vec2f &vec2f, float scalar) noexcept;
        //friend constexpr Vec2f operator+(const Vec2f &vec2f_1, const Vec2f &vec2f_2) noexcept;

        //friend constexpr Vec2f operator-(const Vec2f &vec2f, float scalar) noexcept;
        //friend constexpr Vec2f operator-(const Vec2f &vec2f_1, const Vec2f &vec2f_2) noexcept;

#define LUMEN_CORE_MATH_VEC2F_UNARY_OPERATOR_GENERATOR(unary_operator, Argument, argument)                              \
        friend constexpr Vec2f operator unary_operator(Argument argument) noexcept                                      \
        {                                                                                                               \
                return glm::operator unary_operator(static_cast<typename BaseCastIf<Argument>::type>(argument));        \
        }

        // Unary operators
        LUMEN_CORE_MATH_VEC2F_UNARY_OPERATOR_GENERATOR(+, const Vec2f &, vec2f)
        LUMEN_CORE_MATH_VEC2F_UNARY_OPERATOR_GENERATOR(-, const Vec2f &, vec2f)

#undef LUMEN_CORE_MATH_VEC2F_UNARY_OPERATOR_GENERATOR

#define LUMEN_CORE_MATH_VEC2F_BINARY_OPERATOR_GENERATOR(binary_operator, Argument1, argument1, Argument2, argument2)    \
        friend constexpr Vec2f operator binary_operator(Argument1 argument1, Argument2 argument2) noexcept              \
        {                                                                                                               \
                return glm::operator binary_operator(static_cast<typename BaseCastIf<Argument1>::type>(argument1),      \
                                                     static_cast<typename BaseCastIf<Argument2>::type>(argument2));     \
        }

        // Binary operators
        LUMEN_CORE_MATH_VEC2F_BINARY_OPERATOR_GENERATOR(+, const Vec2f &, vec2f, float, scalar)
        LUMEN_CORE_MATH_VEC2F_BINARY_OPERATOR_GENERATOR(+, float, scalar, const Vec2f &, vec2f)
        LUMEN_CORE_MATH_VEC2F_BINARY_OPERATOR_GENERATOR(+, const Vec2f &, vec2f_1, const Vec2f &, vec2f_2)

        LUMEN_CORE_MATH_VEC2F_BINARY_OPERATOR_GENERATOR(-, const Vec2f &, vec2f, float, scalar)
        LUMEN_CORE_MATH_VEC2F_BINARY_OPERATOR_GENERATOR(-, float, scalar, const Vec2f &, vec2f)
        LUMEN_CORE_MATH_VEC2F_BINARY_OPERATOR_GENERATOR(-, const Vec2f &, vec2f_1, const Vec2f &, vec2f_2)

        LUMEN_CORE_MATH_VEC2F_BINARY_OPERATOR_GENERATOR(*, const Vec2f &, vec2f, float, scalar)
        LUMEN_CORE_MATH_VEC2F_BINARY_OPERATOR_GENERATOR(*, float, scalar, const Vec2f &, vec2f)
        LUMEN_CORE_MATH_VEC2F_BINARY_OPERATOR_GENERATOR(*, const Vec2f &, vec2f_1, const Vec2f &, vec2f_2)

        LUMEN_CORE_MATH_VEC2F_BINARY_OPERATOR_GENERATOR(/, const Vec2f &, vec2f, float, scalar)
        LUMEN_CORE_MATH_VEC2F_BINARY_OPERATOR_GENERATOR(/, float, scalar, const Vec2f &, vec2f)
        LUMEN_CORE_MATH_VEC2F_BINARY_OPERATOR_GENERATOR(/, const Vec2f &, vec2f_1, const Vec2f &, vec2f_2)

#undef LUMEN_CORE_MATH_VEC2F_BINARY_OPERATOR_GENERATOR
};

} // namespace Math
} // namespace Core
} // namespace Lumen
