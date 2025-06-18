/*
Todo:
- Background
- TODO in Enum
- Save uniform locations
- Cache uniform values
- Saving
- Outlining
- Camera code
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