#include "Game.hpp"

#include "Setup.hpp"

#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL.h>
#include <tracy/Tracy.hpp>

#include "Utils/Logger.hpp"
#include "Utils/FlagParser.hpp"
#include "Core/Window.hpp"
#include "Utils/Logger.hpp"


namespace game
{
Game::Game(const int argc, const char * const *argv) noexcept
  : running_(([]{
      
    }(), false)) // pre initialization functions
  , flags_(argc, argv) // args are UTF8 encoded because we use SDL2main
  , window_(*this)
{
  ZoneScopedC(0xb3041b);

  GAME_DLOG(LogType::Info) << "Running!!!";
}

void Game::Run() noexcept
{
  ZoneScopedC(0xb3041b);

  running_ = true;
  while(running_)
  {
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
      if(event.type == SDL_QUIT)
        running_ = false;
    }

    FrameMark;
  }

  Exit();
}

void Game::Exit() noexcept
{
  ZoneScopedC(0xb3041b);

  window_.Exit();
}
} // game