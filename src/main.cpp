/*
Todo:
- Background
- Saving
- Postprocessing
- FPS counter in Debug menu
- Input
- Debug Menu
*/
#include "Setup.hpp"

#include "Core/Game.hpp"


int main(int argc, char *argv[])
{
  ZoneScopedC(0xd10834);

  game::Game game(argc, argv);
  game.Run();

  return 0;
}