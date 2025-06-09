#include "Game.hpp"

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Utils/FlagParser.hpp"
#include "Core/Window.hpp"
#include "Core/EventSystem.hpp"
#include "Core/Entity.hpp"
#include "Rendering/RenderSystem.hpp"
#include "Rendering/ResourceManager.hpp"
#include "Player/InputSystem.hpp"
#include "Player/Camera.hpp"
#include "Player/PlayerComponent.hpp"
#include "Utils/Type.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "Rendering/SpriteComponents.hpp"
#include "Physics/TransformComponent.hpp"
#include "Physics/ColliderComponents.hpp"
#include "Physics/RigidbodyComponent.hpp"


namespace game
{
Game::Game(const int argc, const char * const *argv) noexcept
  : running_(([]{
      ZoneScopedC(0xb3041b);
      Logger::Get().SetVerboseLevel(10);
    }(), false)) // pre initialization functions
  , flags_(argc, argv) // args are UTF8 encoded because we use SDL2main
  , event_cleaner_{event_system_}
  , window_{*this}
  , render_system_{*this}
  , resource_manager_{*this}
  , input_system_{*this}
  , physics_system_{*this}
  , camera_{*this}
{
  ZoneScopedC(0xb3041b);

  GAME_VLOG(1, LogType::kInfo) << "Running!!!";

  event_system_.AddListener(event_cleaner_, EventType::kQuit, this, []([[maybe_unused]] const Event &event, void *game){ return reinterpret_cast<Game*>(game)->QuitEvent(); });
  event_system_.AddListener(event_cleaner_, EventType::kKeyDown, this, [](const Event &event, void *game){ if(event.GetKeycode() == static_cast<int>(SDLK_ESCAPE)) { reinterpret_cast<Game*>(game)->QuitEvent(); } return false; });

  window_.InitEvents();
  input_system_.InitEvents();
}

Game::~Game() noexcept
{
  ZoneScopedC(0xb3041b);

  registry_.clear();
}

void Game::Run() noexcept
{
  ZoneScopedC(0xb3041b);

  Entity player = Entity(*this);
  TransformComponent &player_transform = player.AddComponent<TransformComponent>();
  player_transform.SetScale(Vector2{10, 10});
  AnimatedSpriteComponent &player_sprite = player.AddComponent<AnimatedSpriteComponent>();
  player_sprite.SetShader(&resource_manager_.GetShader(ShaderType::kAnimatedSprite));
  player_sprite.SetTexture(&resource_manager_.GetTexture(TextureType::kPlayer));
  player_sprite.SetAtlasStep(0.5f);
  player.AddComponent<RectangleColliderComponent>();
  player.AddComponent<RigidbodyComponent>();
  PlayerComponent &player_component = player.AddComponent<PlayerComponent>();

  Entity box = Entity(*this);
  TransformComponent &box_transform = box.AddComponent<TransformComponent>();
  box_transform.SetPosition(Vector2{20, 20});
  box_transform.SetScale(Vector2{10, 10});
  TexturedSpriteComponent &box_sprite = box.AddComponent<TexturedSpriteComponent>();
  box_sprite.SetShader(&resource_manager_.GetShader(ShaderType::kTexturedSprite));
  box_sprite.SetTexture(&resource_manager_.GetTexture(TextureType::kNoTexture64));
  box.AddComponent<RectangleColliderComponent>();

  running_ = true;
  while(running_)
  {
    window_.DispatchSDLEvents();
    event_system_.DispatchEnquedEvents();
    
    render_system_.StartFrame();

    input_system_.Update();
    physics_system_.Update();
    player_component.Update();

    render_system_.EndFrame();

    FrameMark;
  }
}

auto Game::QuitEvent() noexcept -> bool
{
  running_ = false;

  return false;
}
} // game