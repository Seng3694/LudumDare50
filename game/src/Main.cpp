#include "Game.hpp"
#include "PlayState.hpp"

int main()
{
    gjt::Game game("LudumDare50", 1280, 720);
    game.run<PlayState>();
    return 0;
}
