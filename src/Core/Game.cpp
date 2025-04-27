#include "Game.hpp"

#include "Setup.hpp"

#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL.h>
#include <tracy/Tracy.hpp>

#include "Utils/Logger.hpp"
#include "Utils/FlagParser.hpp"
#include "Core/Window.hpp"
#include "Utils/Logger.hpp"
#include "Utils/Type.hpp"

#include "Rendering/Sprite.hpp"


namespace game
{
Game::Game(const int argc, const char * const *argv) noexcept
  : running_(([]{
      ZoneScopedC(0xb3041b);
      Logger::Get().SetVerboseLevel(10);
    }(), false)) // pre initialization functions
  , flags_(argc, argv) // args are UTF8 encoded because we use SDL2main
  , event_cleaner_(event_handler_)
  , window_(*this)
  , renderer_(*this)
  , resource_manager_(*this)
  , input_(*this)
  , player_(*this)
{
  ZoneScopedC(0xb3041b);

  GAME_VLOG(1, LogType::kInfo) << "Running!!!";

  event_handler_.AddListener(event_cleaner_, EventType::kQuit, this, [](const Event &event, void *game){ return reinterpret_cast<Game*>(game)->QuitEvent(); });
  event_handler_.AddListener(event_cleaner_, EventType::kKeyDown, this, [](const Event &event, void *game){ if(event.GetKeycode() == static_cast<int>(SDLK_ESCAPE)) { reinterpret_cast<Game*>(game)->QuitEvent(); } return false; });

  window_.InitEvents();
  resource_manager_.InitEvents();
  input_.InitEvents();
}

void Game::Run() noexcept
{
  ZoneScopedC(0xb3041b);

  running_ = true;
  while(running_)
  {
    window_.DispatchSDLEvents();
    event_handler_.DispatchEnquedEvents();
    
    renderer_.StartFrame();

    input_.Update();
    physics_.Update();
    player_.Update();

    renderer_.EndFrame();

    FrameMark;
  }

  Exit();
}

void Game::Exit() noexcept
{
  ZoneScopedC(0xb3041b);

  resource_manager_.Exit();
  renderer_.Exit();
  window_.Exit();
}

auto Game::QuitEvent() noexcept -> bool
{
  running_ = false;

  return false;
}
} // game