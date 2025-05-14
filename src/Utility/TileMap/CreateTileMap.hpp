
#pragma once

#include <Core/Math/Vector.hpp>

#include <SFML/Graphics/Image.hpp>

#include <cassert>
#include <cstdint>
#include <cctype>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <optional>
#include <iostream>

namespace Lumen {
namespace Utility {
namespace TileMap {

namespace Detail {

struct TileData {
        std::string id;
        std::filesystem::path file_path;
};

struct TileMapeData {
        std::vector<TileData> tiles;
        std::filesystem::path texture_file_path;
        std::filesystem::path config_file_path;
};

struct FileLocation {
        std::int64_t line{1};
        std::int64_t column{0};
        std::int64_t prev_column{0};
        char last_ch;
        bool can_unget{false};

        constexpr void GetChar(const int ch) noexcept
        {
                if (EOF == ch) {
                        return;
                }

                this->can_unget = true;
                this->last_ch = static_cast<char>(ch);
                if ('\n' == ch) {
                        ++this->line;
                        this->prev_column = this->column;
                        this->column = 0;
                } else {
                        ++this->column;
                }
        }

        constexpr void UngetChar(void) noexcept
        {
                if (!this->can_unget) {
                        std::cerr << "Cannot unget character\n";
                        std::abort();
                }

                this->can_unget = false;
                if ('\n' == this->last_ch) {
                        --this->line;
                        this->column = this->prev_column;
                } else {
                        --this->column;
                }
        }

};

void ReadJsonLeftBrace(std::ifstream &config_file, FileLocation &file_location) noexcept
{
        int ch;
        do {
                ch = config_file.get();
                file_location.GetChar(ch);
                if (EOF == ch) {
                        std::stringstream ss;
                        ss << "At line " << file_location.line << " column " << file_location.column
                           << ": Failed to read json left brace, get EOF\n";
                        std::cerr << ss.str();
                        std::abort();
                }
        } while (std::isspace(ch));

        if ('{' != ch) {
                std::stringstream ss;
                ss << "At line " << file_location.line << " column " << file_location.column
                   << ": Failed to read json left brace, excepting \'{\', but find \'" << static_cast<char>(ch) << "\'\n";
                std::cerr << ss.str();
                std::abort();
        }
}

void ReadJsonHeader(std::ifstream &config_file, FileLocation &file_location) noexcept
{
        ReadJsonLeftBrace(config_file, file_location);
}

void ReadJsonRightBrace(std::ifstream &config_file, FileLocation &file_location) noexcept
{
        int ch;
        do {
                ch = config_file.get();
                file_location.GetChar(ch);
                if (EOF == ch) {
                        std::stringstream ss;
                        ss << "At line " << file_location.line << " column " << file_location.column
                           << ": Failed to read json right brace, get EOF\n";
                        std::cerr << ss.str();
                        std::abort();
                }
        } while (std::isspace(ch));

        if ('}' != ch) {
                std::stringstream ss;
                ss << "At line " << file_location.line << " column " << file_location.column
                        << ": Failed to read json right brace, excepting \'}\', but find \'" << static_cast<char>(ch) << "\'\n";
                std::cerr << ss.str();
                std::abort();
        }
}

void ReadJsonFooter(std::ifstream &config_file, FileLocation &file_location) noexcept
{
        ReadJsonRightBrace(config_file, file_location);
}

std::string ReadJsonName(std::ifstream &config_file, FileLocation &file_location) noexcept
{
        int ch;
        do {
                ch = config_file.get();
                file_location.GetChar(ch);
                if (EOF == ch) {
                        std::stringstream ss;
                        ss << "At line " << file_location.line << " column " << file_location.column
                           << ": Failed to read json name, get EOF\n";
                        std::cerr << ss.str();
                        std::abort();
                }
        } while (std::isspace(ch));

        if ('\"' != ch) {
                std::stringstream ss;
                ss << "At line " << file_location.line << " column " << file_location.column
                   << ": Failed to read json name, excepting '\"', but find '" << static_cast<char>(ch) << "'\n";
                std::cerr << ss.str();
                std::abort();
        }

        std::string name;
        for (;;) {
                
                ch = config_file.get();
                file_location.GetChar(ch);

                if (EOF == ch) {
                        std::stringstream ss;
                        ss << "At line " << file_location.line << " column " << file_location.column
                           << ": Failed to read json name, excepting '\"', but find '" << static_cast<char>(ch) << "'\n";
                        std::cerr << ss.str();
                        std::abort();
                }

                if ('\"' == ch) {
                        break;
                }

                name.push_back(static_cast<char>(ch));
        } while (std::isspace(ch));

        return name;
}

bool HasNextJsonName(std::ifstream &config_file, FileLocation &file_location) noexcept
{
        int ch;
        do {
                ch = config_file.get();
                file_location.GetChar(ch);
                if (EOF == ch) {
                        return false;
                }

        } while (std::isspace(ch));


        if (',' == ch) {
                return true;
        }
        if ('\"' == ch) {
                config_file.unget();
                file_location.UngetChar();
                return true;
        }

        return false;
}

std::string ReadJsonValueString(std::ifstream &config_file, FileLocation &file_location) noexcept
{
        int ch;
        do {
                ch = config_file.get();
                file_location.GetChar(ch);
                if (EOF == ch) {
                        std::stringstream ss;
                        ss << "At line " << file_location.line << " column " << file_location.column
                           << ": Failed to read json value string, get EOF\n";
                        std::cerr << ss.str();
                        std::abort();
                }
        } while (std::isspace(ch));

        if ('\"' != ch) {
                std::stringstream ss;
                ss << "At line " << file_location.line << " column " << file_location.column
                   << ": Failed to read json value string, excepting \'\"\', but find \'" << static_cast<char>(ch) << "\'\n";
                std::cerr << ss.str();
                std::abort();
        }

        std::string value_str;
        for (;;) {
                ch = config_file.get();
                file_location.GetChar(ch);

                if (EOF == ch) {
                        std::stringstream ss;
                        ss << "At line " << file_location.line << " column " << file_location.column
                           << ": Failed to read json value string, excepting \'\"\', but find \'"
                           << static_cast<char>(ch) << "\'\n";
                        std::cerr << ss.str();
                        std::abort();
                }
                
                if ('\"' == ch) {
                        break;
                }
                value_str.push_back(static_cast<char>(ch));
        }

        return value_str;
}

void ReadJsonColon(std::ifstream &config_file, FileLocation &file_location) noexcept
{
        int ch;
        do {
                ch = config_file.get();
                file_location.GetChar(ch);
                if (EOF == ch) {
                        std::stringstream ss;
                        ss << "At line " << file_location.line << " column " << file_location.column
                           << ": Failed to read json colon, get EOF\n";
                        std::cerr << ss.str();
                        std::abort();
                }
        } while (std::isspace(ch));

        if (':' != ch) {
                std::stringstream ss;
                ss << "At line " << file_location.line << " column " << file_location.column
                   << ": Failed to read json colon, excepting \':\', but find \'" << static_cast<char>(ch) << "\'\n";
                std::cerr << ss.str();
                std::abort();
        }
}

TileMapeData ReadTileMapData(std::ifstream &config_file, FileLocation &file_location) noexcept
{
        int ch;
        do {
                ch = config_file.get();
                file_location.GetChar(ch);

                if (EOF == ch) {
                        std::stringstream ss;
                        ss << "At line " << file_location.line << " column " << file_location.column
                           << ": Failed to read tile map data, get EOF\n";
                        std::cerr << ss.str();
                        std::abort();
                }

        } while (std::isspace(ch));

        if (':' != ch) {
                std::stringstream ss;
                ss << "At line " << file_location.line << " column " << file_location.column
                   << ": Excepting \':\', but find \'" << static_cast<char>(ch) << "\'\n";
                std::cerr << ss.str();
                std::abort();
        }

        ReadJsonLeftBrace(config_file, file_location);

        TileMapeData tile_map_data;
        while (HasNextJsonName(config_file, file_location)) {
                const auto name = ReadJsonName(config_file, file_location);

                if ("texture_file_path" == name) {
                        ReadJsonColon(config_file, file_location);
                        auto value = ReadJsonValueString(config_file, file_location);
                        tile_map_data.texture_file_path = value;
                } else if ("config_file_path" == name) {
                        ReadJsonColon(config_file, file_location);
                        auto value = ReadJsonValueString(config_file, file_location);
                        tile_map_data.config_file_path = value;
                } else if ("tiles" == name) {
                        ReadJsonColon(config_file, file_location);
                        
                        break;
                        
                } else {
                        std::stringstream ss;
                        ss << "At line " << file_location.line << " column " << file_location.column
                           << ": Excepting the json name to be \"tiles\", \"texture_file_path\", or \"config_file_path\", but find \""
                           << name << "\"\n";
                        std::cerr << ss.str();
                        std::abort();
                }
        }

        ReadJsonRightBrace(config_file, file_location);

        return tile_map_data;
}

TileMapeData LoadConfigFile(const std::filesystem::path &config_file_path) noexcept
{
        assert(std::filesystem::exists(config_file_path));
        assert(std::filesystem::is_regular_file(config_file_path));

        std::ifstream config_file{config_file_path};
        Lumen::Utility::TileMap::Detail::FileLocation file_location;

        if (!config_file.is_open()) {
                std::cerr << "Failed to open file: " << config_file_path << "\n";
                std::abort();
        }

        ReadJsonHeader(config_file, file_location);
std::cout << __FILE__ " :" << __LINE__ << " file_location.line: " << file_location.line << " column " << file_location.column << "\n";
        TileMapeData tile_map_data;
        while (HasNextJsonName(config_file, file_location)) {
std::cout << __FILE__ " :" << __LINE__ << " file_location.line: " << file_location.line << " column " << file_location.column << "\n";
                const auto name = ReadJsonName(config_file, file_location);
std::cout << __FILE__ " :" << __LINE__ << " file_location.line: " << file_location.line << " column " << file_location.column << "\n";
                if ("tile_map" == name) {
                        tile_map_data = Detail::ReadTileMapData(config_file, file_location);
                } else {
                        std::stringstream ss;
                        ss << "At line " << file_location.line << " column " << file_location.column
                           << ": Excepting the json name to be \"tile_map\", but find \"" << name << "\"\n";
                        std::cerr << ss.str();
                        std::abort();
                }
        }

        ReadJsonFooter(config_file, file_location);

        return tile_map_data;
}

// struct TileMapSizeData {
//         unsigned int tile_map_width;
//         unsigned int tile_map_height;
//         unsigned int image_width_max;
//         unsigned int image_height_max;
// };

// TileMapSizeData GetTileMapSize(const std::vector<sf::Image> &images) noexcept
// {
//         assert(!images.empty());

//         TileMapSizeData data{0, 0, 0, 0};

//         for (const auto &image : images) {
//                 const auto size = image.getSize();
//                 if (size.x > data.image_width_max) {
//                         data.image_width_max = size.x;
//                 }
//                 if (size.y > data.image_height_max) {
//                         data.image_height_max = size.y;
//                 }
//                 data.tile_map_width += size.x;

//         }

//         data.tile_map_height = data.image_height_max;

//         return data;
// }

} // namespace Detail

[[nodiscard]] int CreateTileMap(const std::filesystem::path &config) noexcept
{
        (void)config;
        return 0;
}

} // namespace TileMap
} // namespace Utility
} // namespace Lumen
