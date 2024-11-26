
#include <Utility/Random/Lehmer.hpp>

#include <iostream>

void TestLehmer(void) noexcept
{
        std::uint32_t seed = 1;
        for (std::size_t i = 0; i < 100; ++i) {
                seed = Lumen::Utility::Random::LehmerRandomNumberGenerator(seed);
                std::cout << seed << ' ';
        }
        std::cout << '\n';
}

int main(void)
{
        TestLehmer();
        std::cout << "DONE\n";
}