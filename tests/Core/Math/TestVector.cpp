
#include <cstdlib>
#include <sstream>
#include <iostream>

#include <Core/Math/Vector.hpp>

constexpr const float FLOAT32_COMPARE_ACCURACY = 0.0001f;

constexpr bool Float32Equal(float fval1, float fval2) noexcept
{
        float sub = fval1 - fval2;
        float greater_value = (fval1 > fval2) ? fval1 : fval2;
        greater_value = (greater_value > 0.0f) ? greater_value : - greater_value;
        float accuracy = FLOAT32_COMPARE_ACCURACY * greater_value;
        return (sub > -accuracy) && (sub < accuracy);
}

#define TestFloat32(fval, freference)                                                                           \
        do {                                                                                                    \
                if (Float32Equal((fval), (freference))) {                                                       \
                        break;                                                                                  \
                }                                                                                               \
                {                                                                                               \
                        std::stringstream string_stream{};                                                      \
                        string_stream << "[Float32 Test Fail]: at \"" __FILE__ "\" Line: "                      \
                                                        << __LINE__ << " Excepting: " << (freference)           \
                                                        << ", But got: " << (fval) << "\n";                     \
                        std::cout << string_stream.str();                                                       \
                }                                                                                               \
                std::abort();                                                                                   \
        } while (0)


void TestLumenCoreMathVectorCase_1(void) noexcept
{
        constexpr float VEC2f_1_X_INIT_VALUE = 1876.0f;
        constexpr float VEC2f_1_Y_INIT_VALUE = -5678.543f;
        constexpr float VEC2f_2_X_INIT_VALUE = 12.0f;
        constexpr float VEC2f_2_Y_INIT_VALUE = 66.54f;
        Lumen::Core::Math::Vec2f vec2f_1{}; // Test default constractor
        vec2f_1.x = VEC2f_1_X_INIT_VALUE; // Test public member x
        vec2f_1.y = VEC2f_1_Y_INIT_VALUE; // Test public member y

        TestFloat32(vec2f_1.x, VEC2f_1_X_INIT_VALUE);
        TestFloat32(vec2f_1.y, VEC2f_1_Y_INIT_VALUE);

        Lumen::Core::Math::Vec2f vec2f_2{VEC2f_2_X_INIT_VALUE, VEC2f_2_Y_INIT_VALUE}; // Test constexpr Vec2f(float x, float y) noexcept
        TestFloat32(vec2f_2.x, VEC2f_2_X_INIT_VALUE);
        TestFloat32(vec2f_2.y, VEC2f_2_Y_INIT_VALUE);

        Lumen::Core::Math::Vec2f result{vec2f_1}; // Test constexpr Vec2f(const Vec2f &vec2f) noexcept
        TestFloat32(result.x, vec2f_1.x);
        TestFloat32(result.y, vec2f_1.y);

        result = vec2f_2;
        TestFloat32(result.x, vec2f_2.x);
        TestFloat32(result.y, vec2f_2.y);

        result += 1.0f;
        TestFloat32(result.x, vec2f_2.x + 1.0f);
        TestFloat32(result.y, vec2f_2.y + 1.0f);

        result += vec2f_1;
        TestFloat32(result.x, vec2f_2.x + 1.0f + vec2f_1.x);
        TestFloat32(result.y, vec2f_2.y + 1.0f + vec2f_1.y);

        result -= 1.0f;
        TestFloat32(result.x, vec2f_2.x + vec2f_1.x);
        TestFloat32(result.y, vec2f_2.y + vec2f_1.y);

        result -= vec2f_1;
        TestFloat32(result.x, vec2f_2.x);
        TestFloat32(result.y, vec2f_2.y);

        result *= 986.98f;
        TestFloat32(result.x, vec2f_2.x * 986.98f);
        TestFloat32(result.y, vec2f_2.y * 986.98f);

        result *= vec2f_1;
        TestFloat32(result.x, vec2f_2.x * 986.98f * vec2f_1.x);
        TestFloat32(result.y, vec2f_2.y * 986.98f * vec2f_1.y);

        result /= 986.98f;
        TestFloat32(result.x, vec2f_2.x * vec2f_1.x);
        TestFloat32(result.y, vec2f_2.y * vec2f_1.y);

        result /= vec2f_1;
        TestFloat32(result.x, vec2f_2.x);
        TestFloat32(result.y, vec2f_2.y);

        ++result;
        TestFloat32(result.x, vec2f_2.x + 1.0f);
        TestFloat32(result.y, vec2f_2.y + 1.0f);

        --result;
        TestFloat32(result.x, vec2f_2.x);
        TestFloat32(result.y, vec2f_2.y);

        {
                Lumen::Core::Math::Vec2f puls_result = +result;
                TestFloat32(puls_result.x, result.x);
                TestFloat32(puls_result.y, result.y);
        }

        {
                Lumen::Core::Math::Vec2f minus_result = -result;
                TestFloat32(minus_result.x, -result.x);
                TestFloat32(minus_result.y, -result.y);
        }

        result = vec2f_1 + vec2f_2;
        TestFloat32(result.x, vec2f_1.x + vec2f_2.x);
        TestFloat32(result.y, vec2f_1.y + vec2f_2.y);

        result = vec2f_1 + 456.3f;
        TestFloat32(result.x, vec2f_1.x + 456.3f);
        TestFloat32(result.y, vec2f_1.y + 456.3f);

        result = 7753.8f + vec2f_1;
        TestFloat32(result.x, 7753.8f + vec2f_1.x);
        TestFloat32(result.y, 7753.8f + vec2f_1.y);

        result = vec2f_1 - vec2f_2;
        TestFloat32(result.x, vec2f_1.x - vec2f_2.x);
        TestFloat32(result.y, vec2f_1.y - vec2f_2.y);

        result = vec2f_1 - 236.2f;
        TestFloat32(result.x, vec2f_1.x - 236.2f);
        TestFloat32(result.y, vec2f_1.y - 236.2f);

        result = 0.0f - vec2f_1;
        TestFloat32(result.x, 0.0f - vec2f_1.x);
        TestFloat32(result.y, 0.0f - vec2f_1.y);

        result = vec2f_1 * vec2f_2;
        TestFloat32(result.x, vec2f_1.x * vec2f_2.x);
        TestFloat32(result.y, vec2f_1.y * vec2f_2.y);
        
        result = vec2f_1 * 244.764f;
        TestFloat32(result.x, vec2f_1.x * 244.764f);
        TestFloat32(result.y, vec2f_1.y * 244.764f);

        result = 1.0f * vec2f_1;
        TestFloat32(result.x, 1.0f * vec2f_1.x);
        TestFloat32(result.y, 1.0f * vec2f_1.y);

        result = vec2f_1 / vec2f_2;
        TestFloat32(result.x, vec2f_1.x / vec2f_2.x);
        TestFloat32(result.y, vec2f_1.y / vec2f_2.y);

        result = vec2f_1 / 123.97f;
        TestFloat32(result.x, vec2f_1.x / 123.97f);
        TestFloat32(result.y, vec2f_1.y / 123.97f);

        result = 3545.86f / vec2f_1;
        TestFloat32(result.x, 3545.86f / vec2f_1.x);
        TestFloat32(result.y, 3545.86f / vec2f_1.y);
}

int main(void)
{
        std::cout << "TestVector begin:\n";
        TestLumenCoreMathVectorCase_1();
        std::cout << "Done\n";
}
