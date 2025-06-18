/*
Todo:
- Background
- TODO in Enum
- Profile and cache uniform values
- Saving
- Camera code
- Postprocessing
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