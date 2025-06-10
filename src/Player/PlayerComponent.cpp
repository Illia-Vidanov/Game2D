#include "PlayerComponent.hpp"

#include "Setup.hpp"


#include "Utils/Math.hpp"
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
PlayerComponent::PlayerComponent(Entity &entity) noexcept
  : entity_{entity}
{
  GAME_ASSERT(entity_.HasComponent<TransformComponent>());
  
  ZoneScopedC(0x31ff98);
}

void PlayerComponent::Update() noexcept
{
  ZoneScopedC(0x31ff98);

  TransformComponent &transform = entity_.GetComponent<TransformComponent>();
  Camera &camera = entity_.GetGame().GetCamera();

  entity_.GetComponent<RigidbodyComponent>().AddForce(Vector2{
    (static_cast<float>(entity_.GetGame().GetInputSystem().GetKey(SDL_SCANCODE_D)) - static_cast<float>(entity_.GetGame().GetInputSystem().GetKey(SDL_SCANCODE_A))),
    (static_cast<float>(entity_.GetGame().GetInputSystem().GetKey(SDL_SCANCODE_W)) - static_cast<float>(entity_.GetGame().GetInputSystem().GetKey(SDL_SCANCODE_S)))
  } * 100);

  float diff = transform.GetPosition().x() - camera.GetPosition().x(); 
  if(std::abs(diff) > 30)
    camera.SetPosition(Vector2{camera.GetPosition().x() + diff - 30 * Sign(diff), camera.GetPosition().y()});

  static std::chrono::duration<double> animation_speed = 0.5s;
  static std::chrono::duration<double> animation_counter = 0s;
  animation_counter += entity_.GetGame().GetDeltaTime();
  if(animation_counter > animation_speed)
  {
    animation_counter = 0s;
    static int sprite_index = 0;
    sprite_index = (sprite_index + 1) % 4;
    entity_.GetComponent<AnimatedSpriteComponent>().SetSpriteIndex(Vector2i{sprite_index % 2, sprite_index / 2});
  } 
}
} // game