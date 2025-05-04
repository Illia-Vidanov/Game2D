/*
Todo:
- Hide warnings with unused parametrs
- Finish logger
- Input
- Flags
- Background
- Moving Scene
- Sprite
- Animation
- Simplest character controller
*/
#include "Setup.hpp"

#include "Core/Game.hpp"


int WinMain([[maybe_unused]] HINSTANCE hInst, [[maybe_unused]] HINSTANCE hInstPrev, [[maybe_unused]] PSTR cmdline, [[maybe_unused]] int cmdshow)
{
  ZoneScopedC(0xd10834);

  game::Game game(0, nullptr);
  game.Run();

  return 0;
}