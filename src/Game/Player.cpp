#include "Player.hpp"

#include "Setup.hpp"

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <SDL2/SDL.h>

#include "Utils/Math.hpp"
#include "Utils/MathConstants.hpp"
#include "Rendering/Sprite.hpp"
#include "Core/Game.hpp"
#include "Rendering/ResourceManager.hpp"
#include "Core/EventHandler.hpp"


namespace game
{
Player::Player(Game &game) noexcept
  : game_{game}
  , sprite_{game_.GetResourceManager().GetShader(ShaderType::kDefault), transform_}
  , event_cleaner_{game_.GetEventHandler()}
{
  sprite_.GetShader().SetUniform("spriteColor", 1.0f, 1.0f, 0.0f);

  game_.GetEventHandler().AddListener(event_cleaner_, EventType::kKeyDown, this, [](const Event &event, void *player) -> bool { return reinterpret_cast<Player*>(player)->KeyDownEvent(event); });
}

void Player::Update() noexcept
{
  game_.GetRenderer().AddToRenderQueue(sprite_);
}

auto Player::KeyDownEvent(const Event &event) noexcept -> bool
{
  GAME_LOG(LogType::kInfo) << transform_.translation().transpose();
  switch (event.GetKeycode())
  {
  case SDLK_w:
    transform_.translation() += Vector3{0, 1, 0};  
    break;
  case SDLK_a:
    transform_.translation() += Vector3{-1, 0, 0};  
    break;
  case SDLK_s:
    transform_.translation() += Vector3{0, -1, 0};  
    break;
  case SDLK_d:
    transform_.translation() += Vector3{1, 0, 0};  
    break;
  default:
    break;
  }

  return false;
}
} // game