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
}

void Player::Update() noexcept
{
  game_.GetRenderer().AddToRenderQueue(sprite_);

  transform_.translation() += Vector3{
    (static_cast<float>(game_.GetInput().GetKey(SDL_SCANCODE_D)) - static_cast<float>(game_.GetInput().GetKey(SDL_SCANCODE_A))) * 0.01f,
    (static_cast<float>(game_.GetInput().GetKey(SDL_SCANCODE_W)) - static_cast<float>(game_.GetInput().GetKey(SDL_SCANCODE_S))) * 0.01f,
    0.0f
  };

  GAME_LOG(LogType::kInfo) << transform_.translation().transpose();
}
} // game