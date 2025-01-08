
#include <cassert>
#include <variant>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <iostream>

namespace Lumen {
namespace Utility {
namespace Json {

using JsonString = std::string;
using JsonIntegerNumber = int;
using JsonFloatingPointNumber = float;
using JsonBoolean = bool;
struct JsonNull {};

enum JsonValueTag {
        NULL_VALUE,
        STRING,
        INTEGER_NUMBER,
        FLOATING_POINT_NUMBER,
        OBJECT,
        ARRAY,
        BOOLEAN,
};

class JsonValue;
namespace Detail {

extern void DeleteJsonValue(JsonValue *value) noexcept;

class JsonValuePtr {
private:
        using this_t = JsonValuePtr;
        JsonValue *m_value;
public:
        constexpr JsonValuePtr(JsonValue *value) noexcept : m_value{value} {}

        constexpr JsonValuePtr(const JsonValuePtr &other) noexcept = delete;
        constexpr JsonValuePtr(this_t &&other) noexcept : m_value{other.m_value}
        {
                other.m_value = nullptr;
        }

        constexpr JsonValuePtr &operator=(const JsonValuePtr &other) noexcept = delete;
        constexpr JsonValuePtr &operator=(JsonValuePtr &&other) noexcept
        {
                if (this == &other) {
                        return *this;
                }

                if (nullptr != this->m_value) {
                        DeleteJsonValue(this->m_value);
                }

                this->m_value = other.m_value;
                other.m_value = nullptr;

                return *this;
        }

        constexpr ~JsonValuePtr(void) noexcept
        {
                if (nullptr != this->m_value) {
                        DeleteJsonValue(this->m_value);
                }
        }

        constexpr JsonValue &operator*(void) noexcept
        {
                return *this->m_value;
        }

        constexpr JsonValue &operator*(void) const noexcept
        {
                return *this->m_value;
        }

        constexpr JsonValue *operator->(void) const noexcept
        {
                return this->m_value;
        }
};
} // namespace Detail

class JsonObject {
private:
        std::unordered_map<std::string, Detail::JsonValuePtr> m_values;

public:

        constexpr void Emplace(std::string &&name, Detail::JsonValuePtr &&value) noexcept
        {
                this->m_values.emplace(std::move(name), std::move(value));
        }
};

class JsonArray {
private:
        std::vector<Detail::JsonValuePtr> m_values;

public:

        constexpr void PushBack(Detail::JsonValuePtr value) noexcept
        {
                this->m_values.push_back(std::move(value));
        }

        constexpr JsonValue &operator[](std::size_t index) noexcept
        {
                assert(index < this->m_values.size());
                return *this->m_values[index];
        }

        constexpr const JsonValue &operator[](std::size_t index) const noexcept
        {
                assert(index < this->m_values.size());
                return *this->m_values[index];
        }

        constexpr std::size_t Size(void) const noexcept
        {
                return this->m_values.size();
        }

        constexpr auto begin(void) noexcept
        {
                return this->m_values.begin();
        }

        constexpr auto end(void) noexcept
        {
                return this->m_values.end();
        }

        constexpr auto begin(void) const noexcept
        {
                return this->m_values.begin();
        }

        constexpr auto end(void) const noexcept
        {
                return this->m_values.end();
        }
};

class JsonValue {
private:
        std::variant<std::unique_ptr<JsonObject>, std::unique_ptr<JsonArray>,
                     JsonString, JsonIntegerNumber,
                     JsonFloatingPointNumber, JsonBoolean, JsonNull> m_value;
        JsonValueTag m_tag;

public:

        constexpr JsonValue(Lumen::Utility::Json::JsonString &&value) noexcept
        : m_value{std::move(value)}, m_tag{JsonValueTag::STRING} {}

        constexpr JsonValue(Lumen::Utility::Json::JsonBoolean value) noexcept
        : m_value{std::move(value)}, m_tag{JsonValueTag::BOOLEAN} {}

        constexpr JsonValue(Lumen::Utility::Json::JsonNull) noexcept
        : m_value{Lumen::Utility::Json::JsonNull{}}, m_tag{JsonValueTag::NULL_VALUE} {}

        constexpr JsonValue(Lumen::Utility::Json::JsonIntegerNumber value) noexcept
        : m_value{std::move(value)}, m_tag{JsonValueTag::INTEGER_NUMBER} {}

        constexpr JsonValue(Lumen::Utility::Json::JsonFloatingPointNumber value) noexcept
        : m_value{std::move(value)}, m_tag{JsonValueTag::FLOATING_POINT_NUMBER} {}

        constexpr JsonValue(std::unique_ptr<Lumen::Utility::Json::JsonObject> &&value) noexcept
        : m_value{std::move(value)}, m_tag{JsonValueTag::OBJECT} {}

        constexpr JsonValue(std::unique_ptr<Lumen::Utility::Json::JsonArray> &&value) noexcept
        : m_value{std::move(value)}, m_tag{JsonValueTag::ARRAY} {}

        constexpr JsonValueTag GetTag(void) const noexcept
        {
                return this->m_tag;
        }

        constexpr bool IsNull(void) const noexcept
        {
                return JsonValueTag::NULL_VALUE == this->m_tag;
        }

        constexpr bool IsString(void) const noexcept
        {
                return JsonValueTag::STRING == this->m_tag;
        }

        constexpr bool IsIntegerNumber(void) const noexcept
        {
                return JsonValueTag::INTEGER_NUMBER == this->m_tag;
        }

        constexpr bool IsFloatingPointNumber(void) const noexcept
        {
                return JsonValueTag::FLOATING_POINT_NUMBER == this->m_tag;
        }

        constexpr bool IsObject(void) const noexcept
        {
                return JsonValueTag::OBJECT == this->m_tag;
        }

        constexpr bool IsArray(void) const noexcept
        {
                return JsonValueTag::ARRAY == this->m_tag;
        }

        constexpr bool IsBoolean(void) const noexcept
        {
                return JsonValueTag::BOOLEAN == this->m_tag;
        }

        constexpr const JsonString &GetString(void) const noexcept
        {
                return std::get<JsonString>(this->m_value);
        }

        constexpr JsonIntegerNumber GetIntegerNumber(void) const noexcept
        {
                return std::get<JsonIntegerNumber>(this->m_value);
        }

        constexpr JsonFloatingPointNumber GetFloatingPointNumber(void) const noexcept
        {
                return std::get<JsonFloatingPointNumber>(this->m_value);
        }

        constexpr const JsonObject &GetObject(void) const noexcept
        {
                return *std::get<std::unique_ptr<JsonObject> >(this->m_value);
        }

        constexpr const JsonArray &GetArray(void) const noexcept
        {
                return *std::get<std::unique_ptr<JsonArray> >(this->m_value);
        }

        constexpr JsonBoolean GetBoolean(void) const noexcept
        {
                return std::get<JsonBoolean>(this->m_value);
        }

        constexpr JsonNull GetNull(void) const noexcept
        {
                return std::get<JsonNull>(this->m_value);
        }

        constexpr JsonString &GetString(void) noexcept
        {
                return std::get<JsonString>(this->m_value);
        }

        constexpr JsonIntegerNumber &GetIntegerNumber(void) noexcept
        {
                return std::get<JsonIntegerNumber>(this->m_value);
        }

        constexpr JsonFloatingPointNumber &GetFloatingPointNumber(void) noexcept
        {
                return std::get<JsonFloatingPointNumber>(this->m_value);
        }

        constexpr JsonObject &GetObject(void) noexcept
        {
                return *std::get<std::unique_ptr<JsonObject> >(this->m_value);
        }

        constexpr JsonArray &GetArray(void) noexcept
        {
                return *std::get<std::unique_ptr<JsonArray> >(this->m_value);
        }

        constexpr JsonBoolean &GetBoolean(void) noexcept
        {
                return std::get<JsonBoolean>(this->m_value);
        }

        constexpr JsonNull &GetNull(void) noexcept
        {
                return std::get<JsonNull>(this->m_value);
        }

};

namespace Detail {

class FileLocation {
private:
        std::int64_t line{1};
        std::int64_t column{0};
        std::int64_t prev_column{0};
        char last_ch;
        bool can_unget{false};

public:

        constexpr void Reset(void) noexcept
        {
                this->line = 1;
                this->column = 0;
                this->prev_column = 0;
                this->last_ch = '\0';
                this->can_unget = false;
        }

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

        constexpr std::int64_t GetLine(void) const noexcept
        {
                return this->line;
        }

        constexpr std::int64_t GetColumn(void) const noexcept
        {
                return this->column;
        }
};
        
} // namespace Detail

} // namespace Json
} // namespace Utility
} // namespace Lumen