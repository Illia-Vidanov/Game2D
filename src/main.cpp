/*
Todo:

*/
#include "Setup.hpp"

#include "Core/Game.hpp"


int WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
  ZoneScopedC(0xd10834);

  game::Game game(0, nullptr);
  game.Run();

  return 0;
}