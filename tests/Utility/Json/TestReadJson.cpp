
#include <Utility/Json/ReadJson.hpp>

#include <string>
#include <string_view>

void TestReadJsonFile() noexcept
{

        Lumen::Utility::Json::JsonInFileStream json_istream{"./config.json"};

        // auto json_value = json_stream.Get("tile_map");
        // auto json_string = Lumen::Utility::Json::JsonCast<Lumen::Utility::Json::JsonString>(json_value);
        // std::cout << json_string << "\n";

        auto json_string = json_istream.GetString("sdvubhsdku");
        std::cout << "\"" << json_string << "\"\n";
        std::cout << "\"" << json_istream.GetString("1") << "\"\n";
        std::cout << "\"" << json_istream.GetString("2") << "\"\n";
        std::cout << "\"" << json_istream.GetString("3") << "\"\n";
        std::cout << "\"" << json_istream.GetString("4") << "\"\n";
        std::cout << "\"" << json_istream.GetString("5") << "\"\n";

        std::cout << json_istream.GetBoolean("11") << "\n";
        std::cout << json_istream.GetBoolean("3275r") << "\n";

        auto null = json_istream.GetNull("nullptr");
        (void)null;

        auto ch_str = json_istream.GetString("而无法给出v但是v根本就哭吧v尽快吧v从k");
        std::ofstream ofs{"./test.txt"};
        ofs << ch_str;

        std::cout << json_istream.GetIntegerNumber("i3278") << "\n";

        std::cout << json_istream.GetFloatingPointNumber("f3232") << "\n";

        auto &array = json_istream.GetArray("arr");\
        std::cout << array[0].GetString() << "\n";
        std::cout << array[1].GetString() << "\n";
        std::cout << array[2].GetString() << "\n";
        std::cout << array[3].GetString() << "\n";
        std::cout << array[4].GetString() << "\n";
        // std::cout << array[5].GetString() << "\n";
        // std::cout << array[6].GetString() << "\n";
        // std::cout << array[7].GetString() << "\n";
        // std::cout << array[8].GetString() << "\n";
        // std::cout << array[9].GetString() << "\n";
        // std::cout << array[10].GetString() << "\n";
        // std::cout << array[11].GetString() << "\n";
        //std::cout << "\"" << json_istream.GetString("6") << "\"\n";
}

int main(void)
{
        TestReadJsonFile();
        std::cout << "Done\n";
        return 0;
}

// ,
//         "tile_map" : {
//                 "tiles" : [

//                         { "id" : "ui_button", "file_path" : "ui_button.png" }
//                 ],
//                 "texture_file_path" : "UI_texture.png",
//                 "config_file_path" : "UI_config.bin"
//         }
//         ,
//         "1" : {},
//         "2" : [],
//         "3" : "",
//         "4" : 0,
//         "5" : -1,
//         "6" : true,
//         "7" : false,
//         "8" : null