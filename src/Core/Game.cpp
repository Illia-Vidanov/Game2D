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
  , ui_{*this}
  , resource_manager_{*this}
  , input_system_{*this}
  , physics_system_{*this}
  , camera_{*this}
{
  ZoneScopedC(0xb3041b);

  GAME_VLOG(1, LogType::kInfo) << "Running!!!";

  frame_start_ = std::chrono::high_resolution_clock::now();

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

  entities_.emplace("player", Entity(*this));
  TransformComponent &player_transform = player_->AddComponent<TransformComponent>();
  player_transform.SetScale(Vector2{10, 10});
  AnimatedSpriteComponent &player_sprite = player_->AddComponent<AnimatedSpriteComponent>();
  player_sprite.SetShader(&resource_manager_.GetShader(ShaderType::kAnimatedSprite));
  player_sprite.SetTexture(&resource_manager_.GetTexture(TextureType::kPlayer));
  player_sprite.SetAtlasStep(0.5f);
  RectangleColliderComponent &player_collider = player_->AddComponent<RectangleColliderComponent>();
  player_collider.SetFriction(1.0f);
  player_collider.SetRestitution(0.3f);
  RigidbodyComponent &player_rigidbody = player_->AddComponent<RigidbodyComponent>();
  player_rigidbody.SetGravityScale(0.0f);
  PlayerComponent &player_component = player_->AddComponent<PlayerComponent>();

  box_ = new Entity(*this);
  TransformComponent &box_transform = box_->AddComponent<TransformComponent>();
  box_transform.SetPosition(Vector2{5, 5});
  box_transform.SetScale(Vector2{100, 10});
  TexturedSpriteComponent &box_sprite = box_->AddComponent<TexturedSpriteComponent>();
  box_sprite.SetShader(&resource_manager_.GetShader(ShaderType::kTexturedSprite));
  box_sprite.SetTexture(&resource_manager_.GetTexture(TextureType::kNoTexture64));
  RectangleColliderComponent &box_collider = box_->AddComponent<RectangleColliderComponent>();

  delta_time_ = std::chrono::high_resolution_clock::now() - frame_start_;

  running_ = true;
  while(running_)
  {
    frame_start_ = std::chrono::high_resolution_clock::now();

    window_.DispatchSDLEvents();
    event_system_.DispatchEnquedEvents();
    
    render_system_.StartFrame();

    input_system_.Update();
    player_component.Update();
    ui_.Update();
    
    render_system_.EndFrame();
    
    if(fixed_delta_time_counter_ >= kFixedDeltaTime)
    {
      fixed_delta_time_ = fixed_delta_time_counter_;
      fixed_delta_time_counter_ = std::clamp(fixed_delta_time_counter_ - kFixedDeltaTime, std::chrono::duration<double>(0.0f), kFixedDeltaTime);

      physics_system_.FixedUpdate();
    }

    delta_time_ = std::chrono::high_resolution_clock::now() - frame_start_;
    fixed_delta_time_counter_ += delta_time_;

    FrameMark;
  }
}

auto Game::QuitEvent() noexcept -> bool
{
  running_ = false;

  return false;
}
} // game