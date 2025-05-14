
#include <Utility/TileMap/CreateTileMap.hpp>

#include <iostream>

void TestReadJsonFile() noexcept
{
        const auto config = Lumen::Utility::TileMap::Detail::LoadConfigFile("./config.json");
        (void)config;
}

int main(void)
{
        TestReadJsonFile();
        std::cout << "Done\n";
        return 0;
}