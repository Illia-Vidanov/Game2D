#include "PlayerComponent.hpp"

#include "Setup.hpp"


#include "Utils/Math.hpp"
#include "Rendering/SpriteComponent.hpp"
#include "Rendering/ResourceManager.hpp"
#include "Core/Game.hpp"
#include "Physics/ColliderComponents.hpp"
#include "Player/InputSystem.hpp"
#include "Physics/TransformComponent.hpp"
#include "Physics/RigidbodyComponent.hpp"


namespace game
{
PlayerComponent::PlayerComponent(entt::entity self, Game &game) noexcept
  : self_{self}
  , game_{game}
{
  ZoneScopedC(0x31ff98);

}

void PlayerComponent::Update() noexcept
{
  ZoneScopedC(0x31ff98);

  TransformComponent &transform = game_.GetRegistry().get<TransformComponent>(self_);
  Camera &camera = game_.GetCamera();

  game_.GetRegistry().get<RigidbodyComponent>(self_).SetVelocity(Vector2{
    (static_cast<float>(game_.GetInputSystem().GetKey(SDL_SCANCODE_D)) - static_cast<float>(game_.GetInputSystem().GetKey(SDL_SCANCODE_A))),
    (static_cast<float>(game_.GetInputSystem().GetKey(SDL_SCANCODE_W)) - static_cast<float>(game_.GetInputSystem().GetKey(SDL_SCANCODE_S)))
  });

  float diff = transform.GetPosition().x() - camera.GetPosition().x(); 
  if(std::abs(diff) > 30)
    camera.SetPosition(Vector2{camera.GetPosition().x() + diff - 30 * Sign(diff), camera.GetPosition().y()});
  //GAME_DLOG(LogType::kInfo) << game_.GetRegistry().get<RigidbodyComponent>(self_).GetVelocity().transpose();
}
} // game