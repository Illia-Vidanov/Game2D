#include "PlayerComponent.hpp"

#include "Setup.hpp"

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <SDL2/SDL.h>
#include <tracy/Tracy.hpp>

#include "Utils/Math.hpp"
#include "Utils/MathConstants.hpp"
#include "Utils/Logger.hpp"
#include "Rendering/SpriteComponent.hpp"
#include "Rendering/ResourceManager.hpp"
#include "Core/Game.hpp"
#include "Core/EventSystem.hpp"
#include "Physics/ColliderComponents.hpp"
#include "Player/InputSystem.hpp"
#include "Physics/TransformComponent.hpp"


namespace game
{
void PlayerComponent::Initialize(entt::entity self, Game &game)
{
  ZoneScopedC(0x31ff98);

  self_ = self;
  game_ = &game;

  //sprite_.GetShader().SetUniform("spriteColor", 1.0f, 1.0f, 1.0f);
  game_->GetRegistry().get<TransformComponent>(self_).linear().diagonal() *= 10;
}

void PlayerComponent::Update() noexcept
{
  ZoneScopedC(0x31ff98);

  game_->GetRegistry().get<TransformComponent>(self_).translation() += Vector2{
    (static_cast<float>(game_->GetInputSystem().GetKey(SDL_SCANCODE_D)) - static_cast<float>(game_->GetInputSystem().GetKey(SDL_SCANCODE_A))) * 0.01f,
    (static_cast<float>(game_->GetInputSystem().GetKey(SDL_SCANCODE_W)) - static_cast<float>(game_->GetInputSystem().GetKey(SDL_SCANCODE_S))) * 0.01f
  };

  //GAME_DLOG(LogType::kInfo) << transform_.translation().transpose();
}
} // game