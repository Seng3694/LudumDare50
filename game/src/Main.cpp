#include "LDGame.hpp"
#include "PlayState.hpp"

#ifdef _NDEBUG
#include <windows.h>

int WinMain(
    HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
    int nShowCmd)
{
    LDGame game("Mow Bot", 768, 704);
    game.run();
    return 0;
}

#else

int main()
{
    LDGame game("Mow Bot", 768, 704);
    game.run();
    return 0;
}

#endif
