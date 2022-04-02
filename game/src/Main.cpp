#include "Game.hpp"
#include "PlayState.hpp"

int main()
{
    gjt::Game game("LudumDare50", 864, 768);
    game.run<PlayState>();
    return 0;
}
