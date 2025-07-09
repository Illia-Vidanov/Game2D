#include "PlayerComponent.hpp"

#include "Setup.hpp"


#include "Utils/Math.hpp"
#include "Physics/VectorMath.hpp"
#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Rendering/SpriteComponents.hpp"
#include "Rendering/ResourceManager.hpp"
#include "Core/Game.hpp"
#include "Core/Entity.hpp"
#include "Physics/ColliderComponents.hpp"
#include "Player/InputSystem.hpp"
#include "Physics/TransformComponent.hpp"
#include "Physics/RigidbodyComponent.hpp"


namespace game
{
PlayerComponent::PlayerComponent(Entity *entity) noexcept
  : entity_{entity}
{
  ZoneScopedC(0x31ff98);
}

void PlayerComponent::Update() noexcept
{
  ZoneScopedC(0x31ff98);

  TransformComponent &transform = entity_->GetComponent<TransformComponent>();
  TransformComponent &camera_transform = entity_->GetGame().GetCamera().GetEntity().GetComponent<TransformComponent>();

  entity_->GetComponent<RigidbodyComponent>().AddForce(Vector2{
    (static_cast<float>(entity_->GetGame().GetInputSystem().GetKey(SDL_SCANCODE_D)) - static_cast<float>(entity_->GetGame().GetInputSystem().GetKey(SDL_SCANCODE_A))),
    (static_cast<float>(entity_->GetGame().GetInputSystem().GetKey(SDL_SCANCODE_W)) - static_cast<float>(entity_->GetGame().GetInputSystem().GetKey(SDL_SCANCODE_S)))
  } * 100);

  float diff = transform.GetPosition().x() - camera_transform.GetPosition().x(); 
  if(std::abs(diff) > 30)
    camera_transform.SetPosition(Vector2{camera_transform.GetPosition().x() + diff - 30 * Sign(diff), camera_transform.GetPosition().y()});

  static std::chrono::duration<double> animation_speed = 0.5s;
  static std::chrono::duration<double> animation_counter = 0s;
  animation_counter += entity_->GetGame().GetDeltaTime();
  if(animation_counter > animation_speed)
  {
    animation_counter = 0s;
    static int sprite_index = 0;
    sprite_index = (sprite_index + 1) % 4;
    dynamic_cast<AnimatedSpriteData*>(entity_->GetComponent<SpriteComponent>().GetData())->SetSpriteIndex(Vector2i{sprite_index % 2, sprite_index / 2});
  } 
}
} // game