
#include <Log/DebugLog.hpp>

void TestDebugLog(void) noexcept
{
        LUMEN_DEBUG_CERR("This is a log: ", 14, " ", 34, 34, "\n");
        LUMEN_DEBUG_CERR("This is a log: ", 14, "34, 34", "\n");
        LUMEN_DEBUG_COUT("This is a log: ", 14, 34, " ", 34, "\n");
        LUMEN_DEBUG_CERR("This is a log: ", 14 + 2354236, " ", 9.02357 + 326.3345, "suvhascv", "\n");
}

int main(void)
{
        TestDebugLog();
        std::cout << "DONE\n";
}
