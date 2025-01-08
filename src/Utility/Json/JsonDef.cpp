
#include "JsonDef.hpp"

namespace Lumen {
namespace Utility {
namespace Json {

namespace Detail {

void DeleteJsonValue(JsonValue *value) noexcept
{
        delete value;
}

} // namespace Detail


} // namespace Json
} // namespace Utility
} // namespace Lumen