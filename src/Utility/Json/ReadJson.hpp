
#include "JsonDef.hpp"

#include <stack>
#include <fstream>
#include <sstream>
#include <utility>
#include <string_view>
#include <filesystem>

namespace Lumen {
namespace Utility {
namespace Json {

class JsonInFileStream {
private:
        using this_t = JsonInFileStream;

        std::ifstream m_file_stream;
        std::stack<char> m_json_stack;
        Lumen::Utility::Json::Detail::FileLocation m_file_location;
        std::unordered_map<std::string, Lumen::Utility::Json::Detail::JsonValuePtr> m_values;

public: 

        JsonInFileStream(void) noexcept = default;
        JsonInFileStream(const JsonInFileStream &) = delete;
        JsonInFileStream(JsonInFileStream &&other) noexcept = default;

        JsonInFileStream &operator=(const JsonInFileStream &) = delete;
        JsonInFileStream &operator=(JsonInFileStream &&other) noexcept = default;

        JsonInFileStream(const std::filesystem::path &file_path) noexcept
        {
                this->m_file_stream.open(file_path);
                this->ParseJson();
        }

        Lumen::Utility::Json::JsonString &GetString(const std::string &name) noexcept
        {
                auto iterator = this->m_values.find(name);
                if (this->m_values.end() == iterator) {
                        std::stringstream ss;
                        ss << "Failed to find json value with name: " << name << "\n";
                        std::cerr << ss.str();
                        std::abort();
                }
                if (!iterator->second->IsString()) {
                        std::stringstream ss;
                        ss << "Json value with name: " << name << " is not a string\n";
                        std::cerr << ss.str();
                        std::abort();
                }
                return iterator->second->GetString();
        }

        Lumen::Utility::Json::JsonBoolean &GetBoolean(const std::string &name) noexcept
        {
                auto iterator = this->m_values.find(name);
                if (this->m_values.end() == iterator) {
                        std::stringstream ss;
                        ss << "Failed to find json value with name: " << name << "\n";
                        std::cerr << ss.str();
                        std::abort();
                }
                if (!iterator->second->IsBoolean()) {
                        std::stringstream ss;
                        ss << "Json value with name: " << name << " is not a boolean\n";
                        std::cerr << ss.str();
                        std::abort();
                }
                return iterator->second->GetBoolean();
        }

        Lumen::Utility::Json::JsonNull GetNull(const std::string &name) const noexcept
        {
                auto iterator = this->m_values.find(name);
                if (this->m_values.end() == iterator) {
                        std::stringstream ss;
                        ss << "Failed to find json value with name: " << name << "\n";
                        std::cerr << ss.str();
                        std::abort();
                }
                if (!iterator->second->IsNull()) {
                        std::stringstream ss;
                        ss << "Json value with name: " << name << " is not a null\n";
                        std::cerr << ss.str();
                        std::abort();
                }
                return iterator->second->GetNull();
        }

        Lumen::Utility::Json::JsonIntegerNumber &GetIntegerNumber(const std::string &name) noexcept
        {
                auto iterator = this->m_values.find(name);
                if (this->m_values.end() == iterator) {
                        std::stringstream ss;
                        ss << "Failed to find json value with name: " << name << "\n";
                        std::cerr << ss.str();
                        std::abort();
                }
                if (!iterator->second->IsIntegerNumber()) {
                        std::stringstream ss;
                        ss << "Json value with name: " << name << " is not an integer number\n";
                        std::cerr << ss.str();
                        std::abort();
                }
                return iterator->second->GetIntegerNumber();
        }

        Lumen::Utility::Json::JsonFloatingPointNumber &GetFloatingPointNumber(const std::string &name) noexcept
        {
                auto iterator = this->m_values.find(name);
                if (this->m_values.end() == iterator) {
                        std::stringstream ss;
                        ss << "Failed to find json value with name: " << name << "\n";
                        std::cerr << ss.str();
                        std::abort();
                }
                if (!iterator->second->IsFloatingPointNumber()) {
                        std::stringstream ss;
                        ss << "Json value with name: " << name << " is not a floating number\n";
                        std::cerr << ss.str();
                        std::abort();
                }
                return iterator->second->GetFloatingPointNumber();
        }

        Lumen::Utility::Json::JsonObject &GetObject(const std::string &name) noexcept
        {
                auto iterator = this->m_values.find(name);
                if (this->m_values.end() == iterator) {
                        std::stringstream ss;
                        ss << "Failed to find json value with name: " << name << "\n";
                        std::cerr << ss.str();
                        std::abort();
                }
                if (!iterator->second->IsObject()) {
                        std::stringstream ss;
                        ss << "Json value with name: " << name << " is not an object\n";
                        std::cerr << ss.str();
                        std::abort();
                }
                return iterator->second->GetObject();
        }

        Lumen::Utility::Json::JsonArray &GetArray(const std::string &name) noexcept
        {
                auto iterator = this->m_values.find(name);
                if (this->m_values.end() == iterator) {
                        std::stringstream ss;
                        ss << "Failed to find json value with name: " << name << "\n";
                        std::cerr << ss.str();
                        std::abort();
                }
                if (!iterator->second->IsArray()) {
                        std::stringstream ss;
                        ss << "Json value with name: " << name << " is not an array\n";
                        std::cerr << ss.str();
                        std::abort();
                }
                return iterator->second->GetArray();
        }

private:
        void ParseJson(void) noexcept
        {
                this->InitParsing();

                this->ParseJsonHeader();

                while (this->HasNextJsonNameValuePair()) {
                        this->ParseJsonNameValuePair();
                        this->ParseJsonCommaIf();
                }

                this->ParseJsonFooter();
        }

        void InitParsing(void) noexcept
        {
                if (!this->m_file_stream.is_open()) {
                        std::cerr << "Failed to open file.\n";
                        std::abort();
                }

                if (0 != this->m_json_stack.size()) {
                        this->m_json_stack = std::stack<char>{};
                }

                this->m_file_location.Reset();
                this->m_values.clear();
        }

        char GetNextNonWhitespaceChar(void) noexcept
        {
                int ch;
                do {
                        ch = this->m_file_stream.get();
                        this->m_file_location.GetChar(ch);

                        if (EOF == ch) {
                                std::stringstream ss;
                                ss << "At file location line " << this->m_file_location.GetLine()
                                   << " column " << this->m_file_location.GetColumn() << ": Unexpected end of file.\n";
                                std::cerr << ss.str();
                                std::abort();
                        }
                } while (std::isspace(ch));
                return static_cast<char>(ch);
        }

        char GetNextChar(void) noexcept
        {
                int ch = this->m_file_stream.get();
                this->m_file_location.GetChar(ch);

                if (EOF == ch) {
                        std::stringstream ss;
                        ss << "At file location line " << this->m_file_location.GetLine()
                           << " column " << this->m_file_location.GetColumn() << ": Unexpected end of file.\n";
                        std::cerr << ss.str();
                        std::abort();
                }

                return static_cast<char>(ch);
        }
        void Unget(void) noexcept
        {
                this->m_file_stream.unget();
                this->m_file_location.UngetChar();
        }

        void ParseJsonCommaIf(void) noexcept
        {
                char ch = this->GetNextNonWhitespaceChar();

                if (',' == ch) {
                        return;
                }

                switch (ch) {
                case '}': [[fallthrough]];
                case ']':
                        this->Unget();
                        return;
                default:
                        break;
                }

                std::stringstream ss;
                ss << "At file location line " << this->m_file_location.GetLine()
                   << " column " << this->m_file_location.GetColumn() << ": Expected ',' but got '"
                   << ch << "'.\n";
                std::cerr << ss.str();
                std::abort();
        }

        void ParseJsonLeftBrace(void) noexcept
        {
                char ch = this->GetNextNonWhitespaceChar();

                if ('{' != ch) {
                        std::stringstream ss;
                        ss << "At file location line " << this->m_file_location.GetLine()
                           << " column " << this->m_file_location.GetColumn() << ": Expected '{' but got '"
                          << ch << "'.\n";
                        std::cerr << ss.str();
                        std::abort();
                }

                this->m_json_stack.push(ch);

                ch = this->GetNextNonWhitespaceChar();
                this->Unget();
                if (('\"' != ch) && ('}' != ch)) {
                        std::stringstream ss;
                        ss << "At file location line " << this->m_file_location.GetLine()
                           << " column " << this->m_file_location.GetColumn() << ": Expected '\"' or '}' but got '"
                           << ch << "'.\n";
                        std::cerr << ss.str();
                        std::abort();
                }
        }

        void ParseJsonRightBrace(void) noexcept
        {
                char ch = this->GetNextNonWhitespaceChar();

                if ('}' != ch) {
                        std::stringstream ss;
                        ss << "At file location line " << this->m_file_location.GetLine()
                           << " column " << this->m_file_location.GetColumn() << ": Expected '}' but got '"
                           << ch << "'.\n";
                        std::cerr << ss.str();
                        std::abort();
                }

                if ('{' != this->m_json_stack.top()) {
                        std::stringstream ss;
                        ss << "At file location line " << this->m_file_location.GetLine()
                           << " column " << this->m_file_location.GetColumn() << ": Brace mismatch '"
                           << this->m_json_stack.top() << "' cannot be closed by '}'.\n";
                }

                this->m_json_stack.pop();
        }

        void ParseJsonHeader(void) noexcept
        {
                if (!this->m_json_stack.empty()) {
                        std::stringstream ss;
                        ss << "At file location line " << this->m_file_location.GetLine()
                           << " column " << this->m_file_location.GetColumn()
                           << ": Json stack is not empty befor parsing json header.\n";
                        std::cerr << ss.str();
                        std::abort();
                }
                this->ParseJsonLeftBrace();
        }

        void ParseJsonFooter(void) noexcept
        {
                this->ParseJsonRightBrace();

                if (!this->m_json_stack.empty()) {
                        std::stringstream ss;
                        ss << "At file location line " << this->m_file_location.GetLine()
                           << " column " << this->m_file_location.GetColumn()
                           << ": Json stack is not empty after parsing json footer.\n";
                        std::cerr << ss.str();
                        std::abort();
                }
        }

        void ParseJsonLeftBracket(void) noexcept
        {
                char ch = this->GetNextNonWhitespaceChar();

                if ('[' != ch) {
                        std::stringstream ss;
                        ss << "At file location line " << this->m_file_location.GetLine()
                           << " column " << this->m_file_location.GetColumn()
                           << ": Expected '[' but got '" << ch << "'.\n";
                        std::cerr << ss.str();
                        std::abort();
                }

                this->m_json_stack.push(ch);

                ch = this->GetNextNonWhitespaceChar();
                this->Unget();
                if (',' == ch) {
                        std::stringstream ss;
                        ss << "At file location line " << this->m_file_location.GetLine()
                           << " column " << this->m_file_location.GetColumn()
                           << ": Unexpected ',' after '['.\n";
                        std::cerr << ss.str();
                        std::abort();
                }
        }

        void ParseJsonRightBracket(void) noexcept
        {
                char ch = this->GetNextNonWhitespaceChar();

                if (']' != ch) {
                        std::stringstream ss;
                        ss << "At file location line " << this->m_file_location.GetLine()
                           << " column " << this->m_file_location.GetColumn()
                           << ": Expected ']' but got '" << ch << "'.\n";
                        std::cerr << ss.str();
                        std::abort();
                }

                if ('[' != this->m_json_stack.top()) {
                        std::stringstream ss;
                        ss << "At file location line " << this->m_file_location.GetLine()
                           << " column " << this->m_file_location.GetColumn() << ": Bracket mismatch '"
                           << this->m_json_stack.top() << "' cannot be closed by ']'.\n";
                        std::cerr << ss.str();
                        std::abort();
                }

                this->m_json_stack.pop();
        }

        bool HasNextArrayElement(void) noexcept
        {
                char ch = this->GetNextNonWhitespaceChar();
                this->Unget();
                return ']' != ch;
        }

        bool HasNextJsonNameValuePair(void) noexcept
        {
                char ch = this->GetNextNonWhitespaceChar();
                this->Unget();
                return '}' != ch;
        }

        void ParseJsonNameValuePair(void) noexcept
        {
                auto name = this->ParseJsonNameValuePairName();

                char ch = this->GetNextNonWhitespaceChar();
                if (':' != ch) {
                        std::stringstream ss;
                        ss << "At file location line " << this->m_file_location.GetLine()
                           << " column " << this->m_file_location.GetColumn()
                           << ": Expected ':' but got '" << ch << "'.\n";
                        std::cerr << ss.str();
                        std::abort();
                }

                auto value = this->ParseJsonValue();

                this->m_values.emplace(std::move(name), std::move(value));
        }

        static char ParseJsonEscapeSequence(char ch,
                                            const Lumen::Utility::Json::Detail::FileLocation &file_location) noexcept
        {
                switch (ch) {
                case '\"':
                        return '\"';
                case '\\':
                        return '\\';
                case '/':
                        return '/';
                case 'b':
                        return '\b';
                case 'f':
                        return '\f';
                case 'n':
                        return '\n';
                case 'r':
                        return '\r';
                case 't':
                        return '\t';
                default:
                        break;
                }
                std::stringstream ss;
                ss << "At file location line " << file_location.GetLine()
                   << " column " << file_location.GetColumn()
                   << ": Invalid escape sequence '\\" << ch << "'.\n";
                std::cerr << ss.str();
                std::abort();
        }

        std::string ParseJsonNameValuePairName(void) noexcept
        {
                char ch = this->GetNextNonWhitespaceChar();
                if (',' == ch) {
                        ch = this->GetNextNonWhitespaceChar();
                }
                if ('\"' != ch) {
                        std::stringstream ss;
                        ss << "At file location line " << this->m_file_location.GetLine()
                           << " column " << this->m_file_location.GetColumn()
                           << ": Expected '\"' but got '" << ch << "'.\n";
                        std::cerr << ss.str();
                        std::abort();
                }

                std::string name{};
                for (;;) {
                        ch = this->GetNextChar();
                        if ('\"' == ch) {
                                break;
                        }
                        if (std::iscntrl(ch)) {
                                std::stringstream ss;
                                ss << "At file location line " << this->m_file_location.GetLine()
                                   << " column " << this->m_file_location.GetColumn()
                                   << ": Control character '" << ch << "' cannot be used in json name.\n";
                                std::cerr << ss.str();
                                std::abort();
                        }
                        if ('\\' == ch) {
                                ch = this_t::ParseJsonEscapeSequence(this->GetNextChar(), this->m_file_location);
                        }
                        name.push_back(ch);
                }

                return name;
        }

        Lumen::Utility::Json::Detail::JsonValuePtr ParseJsonValue(void) noexcept
        {
                // TODO: Implement this function.
                char ch = this->GetNextNonWhitespaceChar();
                if (',' == ch) {
                        ch = this->GetNextNonWhitespaceChar();
                }
                this->Unget();

                switch (ch) {
                case '\"':
                        return { std::make_unique<Lumen::Utility::Json::JsonValue>(this->ParseJsonString()).release() };
                
                case '{':
                        return { std::make_unique<Lumen::Utility::Json::JsonValue>(this->ParseJsonObject()).release() };
                
                case '[':
                        return { std::make_unique<Lumen::Utility::Json::JsonValue>(this->ParseJsonArray()).release() };
                
                case 't':
                        return { std::make_unique<Lumen::Utility::Json::JsonValue>(this->ParseJsonBooleanTrue()).release() };
                case 'f':                 
                        return { std::make_unique<Lumen::Utility::Json::JsonValue>(this->ParseJsonBooleanFalse()).release() };
                
                case 'n':
                        return { std::make_unique<Lumen::Utility::Json::JsonValue>(this->ParseJsonNull()).release() };

                case '-': [[fallthrough]];
                case '0': [[fallthrough]];
                case '1': [[fallthrough]];
                case '2': [[fallthrough]];
                case '3': [[fallthrough]];
                case '4': [[fallthrough]];
                case '5': [[fallthrough]];
                case '6': [[fallthrough]];
                case '7': [[fallthrough]];
                case '8': [[fallthrough]];
                case '9': 
                        return this->ParseJsonNumber();
                
                default:
                        break;
                }

                std::stringstream ss;
                ss << "At file location line " << this->m_file_location.GetLine()
                   << " column " << this->m_file_location.GetColumn()
                   << ": Unexpected character '" << ch << "'.\n";
                std::cerr << ss.str();
                std::abort();
        }

        Lumen::Utility::Json::JsonString ParseJsonString(void) noexcept
        {
                char ch = this->GetNextNonWhitespaceChar();
                if ('\"' != ch) {
                        std::stringstream ss;
                        ss << "At file location line " << this->m_file_location.GetLine()
                           << " column " << this->m_file_location.GetColumn()
                           << ": Expected '\"' but got '" << ch << "'.\n";
                        std::cerr << ss.str();
                        std::abort();
                }

                Lumen::Utility::Json::JsonString string{};
                for (;;) {
                        ch = this->GetNextChar();
                        if ('\"' == ch) {
                                break;
                        }
                        if (std::iscntrl(ch)) {
                                std::stringstream ss;
                                ss << "At file location line " << this->m_file_location.GetLine()
                                   << " column " << this->m_file_location.GetColumn()
                                   << ": Control character '" << ch << "' cannot be used in json string.\n";
                                std::cerr << ss.str();
                                std::abort();
                        }
                        if ('\\' == ch) {
                                ch = this_t::ParseJsonEscapeSequence(this->GetNextChar(), this->m_file_location);
                        }
                        string.push_back(ch);
                }

                return string;
        }

        Lumen::Utility::Json::JsonBoolean ParseJsonBooleanTrue(void) noexcept
        {
                auto print_error = [this](char expected_ch, char got_ch) {
                        std::stringstream ss;
                        ss << "At file location line " << this->m_file_location.GetLine()
                           << " column " << this->m_file_location.GetColumn()
                           << ": Expected '" << expected_ch << "' but got '" << static_cast<char>(got_ch) << "'.\n";
                        std::cerr << ss.str();
                        std::abort();
                };

                char ch = this->GetNextNonWhitespaceChar();

                if ('t' != ch) {
                        print_error('t', ch);
                }

                ch = this->GetNextChar();
                if ('r' != ch) {
                        print_error('r', ch);
                }

                ch = this->GetNextChar();
                if ('u' != ch) {
                        print_error('u', ch);
                }

                ch = this->GetNextChar();
                if ('e' != ch) {
                        print_error('e', ch);
                }

                return true;
        }

        Lumen::Utility::Json::JsonBoolean ParseJsonBooleanFalse(void) noexcept
        {
                auto print_error =  [this] [[noreturn]] (char expected_ch, char got_ch) {
                        std::stringstream ss;
                        ss << "At file location line " << this->m_file_location.GetLine()
                           << " column " << this->m_file_location.GetColumn()
                           << ": Expected '" << expected_ch << "' but got '" << static_cast<char>(got_ch) << "'.\n";
                        std::cerr << ss.str();
                        std::abort();
                };

                char ch = this->GetNextNonWhitespaceChar();

                if ('f' != ch) {
                        print_error('f', ch);
                }

                ch = this->GetNextChar();
                if ('a' != ch) {
                        print_error('a', ch);
                }

                ch = this->GetNextChar();
                if ('l' != ch) {
                        print_error('l', ch);
                }

                ch = this->GetNextChar();
                if ('s' != ch) {
                        print_error('s', ch);
                }

                ch = this->GetNextChar();
                if ('e' != ch) {
                        print_error('e', ch);
                }

                return false;
        }

        Lumen::Utility::Json::JsonNull ParseJsonNull(void) noexcept
        {
                auto print_error =  [this] [[noreturn]] (char expected_ch, char got_ch) {
                        std::stringstream ss;
                        ss << "At file location line " << this->m_file_location.GetLine()
                           << " column " << this->m_file_location.GetColumn()
                           << ": Expected '" << expected_ch << "' but got '" << static_cast<char>(got_ch) << "'.\n";
                        std::cerr << ss.str();
                        std::abort();
                };

                char ch = this->GetNextNonWhitespaceChar();

                if ('n' != ch) {
                        print_error('n', ch);
                }

                ch = this->GetNextChar();
                if ('u' != ch) {
                        print_error('u', ch);
                }

                ch = this->GetNextChar();
                if ('l' != ch) {
                        print_error('l', ch);
                }

                ch = this->GetNextChar();
                if ('l' != ch) {
                        print_error('l', ch);
                }

                return Lumen::Utility::Json::JsonNull{};
        }

        Lumen::Utility::Json::Detail::JsonValuePtr ParseJsonNumber(void) noexcept
        {
                char ch = this->GetNextNonWhitespaceChar();

                int sign = 1;
                if ('-' == ch) {
                        ch = this->GetNextChar();
                        sign = -1;
                }

                int number_integer_part = 0;
                if (!std::isdigit(ch)) {
                        std::stringstream ss;
                        ss << "At file location line " << this->m_file_location.GetLine()
                           << " column " << this->m_file_location.GetColumn()
                           << ": Expected digit but got '" << static_cast<char>(ch) << "'.\n";
                        std::cerr << ss.str();
                        std::abort();
                }
                if ('0' != ch) {
                        for (;;) {
                                number_integer_part = number_integer_part * 10 + (ch - '0');
                                ch = this->GetNextChar();
                                if ('.' == ch) {
                                        break;
                                }
                                if (!std::isdigit(ch)) {
                                        this->Unget();
                                        return {
                                                std::make_unique<Lumen::Utility::Json::JsonValue>(
                                                        Lumen::Utility::Json::JsonIntegerNumber{sign * number_integer_part}
                                                ).release()
                                        };
                                }
                        }
                } else {
                        ch = this->GetNextChar();
                        if (std::isdigit(ch)) {
                                std::stringstream ss;
                                ss << "At file location line " << this->m_file_location.GetLine()
                                   << " column " << this->m_file_location.GetColumn()
                                   << ": Unexpected digit '" << static_cast<char>(ch) << "'.\n";
                                std::cerr << ss.str();
                                std::abort();
                        }
                        if ('.' != ch) {
                                this->Unget();
                                return {
                                        std::make_unique<Lumen::Utility::Json::JsonValue>(
                                                Lumen::Utility::Json::JsonIntegerNumber{0}
                                        ).release()
                                };
                        }

                }

                float number_fractional_part = 0.0f;
                float fractional_mult = 0.1f;

                ch = this->GetNextChar();
                if (!std::isdigit(ch)) {
                        std::stringstream ss;
                        ss << "At file location line " << this->m_file_location.GetLine()
                           << " column " << this->m_file_location.GetColumn()
                           << ": Expected digit but got '" << static_cast<char>(ch) << "'.\n";
                        std::cerr << ss.str();
                        std::abort();
                }
                number_fractional_part += static_cast<float>(ch - '0') * fractional_mult;
                fractional_mult *= 0.1f;
                for (;;) {
                        ch = this->GetNextChar();
                        if (!std::isdigit(ch)) {
                                this->Unget();
                                return {
                                        std::make_unique<Lumen::Utility::Json::JsonValue>(
                                                Lumen::Utility::Json::JsonFloatingPointNumber{
                                                        static_cast<float>(sign) * (static_cast<float>(number_integer_part) + number_fractional_part)
                                                }
                                        ).release()
                                };
                        }
                        number_fractional_part += static_cast<float>(ch - '0') * fractional_mult;
                        fractional_mult *= 0.1f;
                }
        }

        std::unique_ptr<Lumen::Utility::Json::JsonArray> ParseJsonArray(void) noexcept
        {
                this->ParseJsonLeftBracket();

                auto json_array = std::make_unique<Lumen::Utility::Json::JsonArray>();
                while (this->HasNextArrayElement()) {
                        json_array->PushBack(this->ParseJsonValue());
                        this->ParseJsonCommaIf();
                }

                this->ParseJsonRightBracket();

                return json_array;
        }

        std::unique_ptr<Lumen::Utility::Json::JsonObject> ParseJsonObject(void) noexcept
        {
                this->ParseJsonLeftBrace();

                auto json_object = std::make_unique<Lumen::Utility::Json::JsonObject>();
                while (this->HasNextJsonNameValuePair()) {
                        auto name = this->ParseJsonNameValuePairName();

                        char ch = this->GetNextNonWhitespaceChar();
                        if (':' != ch) {
                                std::stringstream ss;
                                ss << "At file location line " << this->m_file_location.GetLine()
                                   << " column " << this->m_file_location.GetColumn()
                                   << ": Expected ':' but got '" << ch << "'.\n";
                                std::cerr << ss.str();
                                std::abort();
                        }

                        auto value = this->ParseJsonValue();

                        json_object->Emplace(std::move(name), std::move(value));

                        this->ParseJsonCommaIf();
                }

                this->ParseJsonRightBrace();

                return json_object;
        }
};

} // namespace Json
} // namespace Utility
} // namespace Lumen