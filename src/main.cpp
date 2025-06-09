/*
Todo:
- Hide warnings with unused parametrs
- Finish logger
- Input
- Flags
- Background
- Sprite
- Animation
- Simplest character controller
- Gameobject class
- Clean up
- Delta time / limit fps
- TODO in Enum
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