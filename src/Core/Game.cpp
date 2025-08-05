#include "Game.hpp"

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Utils/FlagParser.hpp"
#include "Core/Window.hpp"
#include "Core/Entity.hpp"
#include "Rendering/RenderSystem.hpp"
#include "Player/InputSystem.hpp"
#include "Utils/Type.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "Game/Scene.hpp"
#include "UI/UI.hpp"
#include "Core/AllComponents.hpp"


namespace game
{
Game::Game(const int argc, const char * const *argv) noexcept
  : running_(([]{
      ZoneScopedC(0xb3041b);
      Logger::Get().SetVerboseLevel(10);
    }(), false)) // pre initialization functions
  , flags_(argc, argv) // args are UTF8 encoded because we use SDL2main
  , window_{*this}
  , render_system_{*this}
  , ui_{*this}
  , resource_manager_{*this}
  , input_system_{*this}
  , physics_system_{*this}
  , debug_{*this}
  , scene_{new Scene1{*this}}
{
  ZoneScopedC(0xb3041b);

  GAME_VLOG(1, LogType::kInfo) << "Running!!!";

  frame_start_ = std::chrono::high_resolution_clock::now();

  window_.InitEvents();
}

Game::~Game() noexcept
{
  ZoneScopedC(0xb3041b);

  registry_.clear();
}

void Game::Run() noexcept
{
  ZoneScopedC(0xb3041b);

  delta_time_ = std::chrono::high_resolution_clock::now() - frame_start_;

  running_ = true;
  while(running_)
  {
    frame_start_ = std::chrono::high_resolution_clock::now();

    input_system_.Update();
    window_.DispatchSDLEvents();
    if(input_system_.GetKeyDown(SDLK_ESCAPE))
      QuitEvent();
    
    render_system_.StartFrame();

    scene_->Update();
    UpdateComponents<PlayerComponent>();
    debug_.Update();

    ui_.Update();
    
    render_system_.EndFrame();
    
    if(fixed_delta_time_counter_ >= kFixedDeltaTime)
    {
      ZoneScopedNC("Fixed Update", 0xb3041b);

      fixed_delta_time_ = fixed_delta_time_counter_;
      fixed_delta_time_counter_ = std::clamp(fixed_delta_time_counter_ - kFixedDeltaTime, std::chrono::duration<double>(0.0f), kFixedDeltaTime);

      physics_system_.FixedUpdate();
    }

    delta_time_ = std::chrono::high_resolution_clock::now() - frame_start_;
    fixed_delta_time_counter_ += delta_time_;

    FrameMark;
  }
}

auto Game::FindCamera() noexcept -> CameraComponent &
{
  entt::view<entt::get_t<CameraComponent>> cameras = registry_.view<CameraComponent>();
  for(entt::entity entity : cameras)
  {
    return cameras.get<CameraComponent>(entity);
  }

  // Creating default camera
  Entity &camera = GetOrCreateEntity("Camera");
  camera.AddComponent<TransformComponent>();
  return camera.AddComponent<CameraComponent>();
}

auto Game::GetOrCreateEntity(const std::string &name) noexcept -> Entity &
{
  std::unordered_map<std::string, Entity*>::iterator it = entities_.find(name);
  if(it == entities_.end())
    return CreateEntity(name);
  
  return *it->second;
}

void Game::QuitEvent() noexcept
{
  running_ = false;
}
} // game