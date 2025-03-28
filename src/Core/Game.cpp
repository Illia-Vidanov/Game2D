#include "Game.hpp"

#include "Setup.hpp"

#include <SDL2/SDL_keycode.h>
#include <tracy/Tracy.hpp>

#include "Utils/Logger.hpp"
#include "Utils/FlagParser.hpp"
#include "Rendering/Renderer.hpp"
#include "Core/Window.hpp"
#include "Core/EventHandler.hpp"
#include "Utils/Logger.hpp"
#include "Platform/Platform.hpp"


namespace game
{
Game::Game(const int argc, const char * const *argv) noexcept
  : running_(([]{
      
    }(), false)) // pre initialization functions
  , flags_(argc, argv) // args are UTF8 encoded because we use SDL2main
  , event_cleaner_(events_)
  , renderer_(Renderer::GetRenderer(*this))
  , window_(*this)
  , world_(*this)
{
  ZoneScopedC(0xb3041b);

  GAME_LOG(LogType::Info) << "OS: " << GAME_TO_STRING(GAME_PLATFORM);

  renderer_.Init();

  events_.AddListener(event_cleaner_, EventType::Quit, this,
    [](__attribute__((unused)) const Event &event, void *data) -> bool
    {
      reinterpret_cast<Game*>(data)->QuitEvent();
      return true;
    });
  events_.AddListener(event_cleaner_, EventType::KeyDown, this,
    [](const Event &event, void *data) -> bool
    {
      if(event.GetKeycode() != SDLK_ESCAPE)
        return false;
      reinterpret_cast<Game*>(data)->QuitEvent();
      return true;
    });
}

void Game::Run() noexcept
{
  {
  ZoneScopedC(0xb3041b);

  running_ = true;
  while(running_)
  {
    events_.DispatchSDLEvents();
    events_.DispatchEnquedEvents();

    
    renderer_.StartFrame();
    world_.Update();
    renderer_.EndFrame();

    FrameMark;
  }
  }

  Exit();
}

void Game::QuitEvent()
{
  ZoneScopedC(0xb3041b);

  running_ = false;
}

void Game::Exit() noexcept
{
  ZoneScopedC(0xb3041b);

  renderer_.Exit();
  window_.Exit();
}
} // game