
#include "Lumen.hpp"
#include "VisualProfiling/VisualProfiler.hpp"

int main(void)
{
        LUMEN_VISUAL_PROFILE_FUNCTION();
        Lumen::Game lumen_game{};
        lumen_game.Init();
        lumen_game.Run();
}
