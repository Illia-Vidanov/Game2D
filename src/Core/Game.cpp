#include "Game.hpp"

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/FlagParser.hpp"
#include "Core/Window.hpp"
#include "Utils/Logger.hpp"
#include "Utils/Type.hpp"
#include "Player/PlayerComponent.hpp"
#include "Rendering/SpriteComponent.hpp"
#include "Physics/TransformComponent.hpp"


namespace game
{
Game::Game(const int argc, const char * const *argv) noexcept
  : running_(([]{
      ZoneScopedC(0xb3041b);
      Logger::Get().SetVerboseLevel(10);
    }(), false)) // pre initialization functions
  , flags_(argc, argv) // args are UTF8 encoded because we use SDL2main
  , player_{registry_.create()}
  , event_cleaner_{event_system_}
  , window_{*this}
  , render_system_{*this}
  , resource_manager_{*this}
  , input_system_{*this}
  , physics_system_{*this}
{
  ZoneScopedC(0xb3041b);

  GAME_VLOG(1, LogType::kInfo) << "Running!!!";

  event_system_.AddListener(event_cleaner_, EventType::kQuit, this, [](const Event &event, void *game){ return reinterpret_cast<Game*>(game)->QuitEvent(); });
  event_system_.AddListener(event_cleaner_, EventType::kKeyDown, this, [](const Event &event, void *game){ if(event.GetKeycode() == static_cast<int>(SDLK_ESCAPE)) { reinterpret_cast<Game*>(game)->QuitEvent(); } return false; });

  window_.InitEvents();
  resource_manager_.InitEvents();
  input_system_.InitEvents();

  registry_.emplace<TransformComponent>(player_);
  SpriteComponent &player_sprite = registry_.emplace<SpriteComponent>(player_);
  player_sprite.SetShader(resource_manager_.GetShader(ShaderType::kDefault));
  player_sprite.SetTexture(resource_manager_.GetTexture(TextureType::kNoImage64));
  registry_.emplace<SquareColliderComponent>(player_);
  registry_.emplace<PlayerComponent>(player_).Initialize(player_, *this);
}

void Game::Run() noexcept
{
  ZoneScopedC(0xb3041b);

  running_ = true;
  while(running_)
  {
    window_.DispatchSDLEvents();
    event_system_.DispatchEnquedEvents();
    
    render_system_.StartFrame();

    input_system_.Update();
    physics_system_.Update();
    registry_.get<PlayerComponent>(player_).Update();

    render_system_.EndFrame();

    FrameMark;
  }

  Exit();
}

void Game::Exit() noexcept
{
  ZoneScopedC(0xb3041b);

  //registry_.clear();
  resource_manager_.Exit();
  render_system_.Exit();
  window_.Exit();
}

auto Game::QuitEvent() noexcept -> bool
{
  running_ = false;

  return false;
}
} // game