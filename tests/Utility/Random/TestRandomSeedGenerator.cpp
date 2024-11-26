
#include <Utility/Random/RandomSeedGenerator.hpp>

#include <iostream>

void TestRandomSeedGenerator(void) noexcept
{
        for (std::size_t i = 0; i < 100; ++i) {
                Lumen::Utility::Random::RandomSeed seed = Lumen::Utility::Random::GenerateRandomSeed();
                std::cout << seed << ' ';
        }
        std::cout << '\n';
}

int main(void)
{
        TestRandomSeedGenerator();
        std::cout << "DONE\n";
}