/*
Todo:
Transfer main gl classes
Coursor class
Hello triangle
span
multithreded file reading
Every class has all necessary constructors
*/

#include "Setup.hpp"

#include <tracy/Tracy.hpp>

#include "Core/Game.hpp"

int main(int argc, char **argv)
{
  ZoneScopedC(0xd10834);

  game::Game game(argc, argv);
  game.Run();

  return 0;
}