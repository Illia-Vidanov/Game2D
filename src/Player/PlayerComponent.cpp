#include "PlayerComponent.hpp"

#include "Setup.hpp"


#include "Utils/Math.hpp"
#include "Physics/VectorMath.hpp"
#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Rendering/ResourceManager.hpp"
#include "Core/Game.hpp"
#include "Core/Entity.hpp"
#include "Core/AllComponents.hpp"
#include "Player/InputSystem.hpp"
#include "Utils/Enum.hpp"


namespace game
{
PlayerComponent::PlayerComponent(Entity *entity) noexcept
  : ComponentBase{entity}
{
  ZoneScopedC(0x31ff98);
}

void PlayerComponent::Update() noexcept
{
  ZoneScopedC(0x31ff98);

  Game &game = entity_->GetGame();
  TransformComponent &transform = entity_->GetComponent<TransformComponent>();
  TransformComponent &camera_transform = game.FindCamera().GetEntity().GetComponent<TransformComponent>();
  RigidbodyComponent &rigidbody = entity_->GetComponent<RigidbodyComponent>();

  // State machine
  DefaultFloatType acceleration;
  DefaultFloatType deceleration;
  switch(state_ & PlayerState::kMainBits)
  {
  // Grounded
  case PlayerState::kGroundedBit:
    // Enter Grounded state
    if(is_changing_main_state_)  
    {
      acceleration = kGroundAcceleration;
      deceleration = kGroundDeceleration;
      desired_speed_ = 10.0f;
      gravity_strength_ = kDefaultGravityStrength;

      is_changing_main_state_ = false;
    }


    // Update Grounded state
    move_direction_ = Vector2{(static_cast<float>(game.GetInputSystem().GetKey(SDL_SCANCODE_D)) - static_cast<float>(game.GetInputSystem().GetKey(SDL_SCANCODE_A))), 0.0f};

    if(game.GetInputSystem().GetKeyDown(SDLK_SPACE))
    {
      jump_velocity_ = (2.0f * kMaxJumpHeight) / kAscendTime;
      gravity_strength_ = (2.0f * kMaxJumpHeight) / kAscendTime * kAscendTime;
    }


    // Change Grounded to Air
    if(game.GetPhysicsSystem().Raycast(transform.GetPosition(), Vector2{0, -kGroundCheckDistance}).entity == nullptr)
    {
      state_ ^= PlayerState::kGroundedBit;

      is_changing_main_state_ = true;
    }

    break;
  
  // Air
  case PlayerState::kAir:
    // Enter Air
    if(is_changing_main_state_)
    {
      is_changing_main_state_ = false;

      acceleration = kAirAcceleration;
      deceleration = kAirDeceleration;
      desired_speed_ = 15.0f;
    }
    

    // Update Air
    move_direction_ = Vector2{(static_cast<float>(game.GetInputSystem().GetKey(SDL_SCANCODE_D)) - static_cast<float>(game.GetInputSystem().GetKey(SDL_SCANCODE_A))), 0.0f};

    // Here 1.0f / (sin/cos) is used to get direction vector
    // sin and cos are swapped to make the forward direction to up direction
    // dot is used to check if directions are facing the same way
    if(rigidbody.GetVelocity().dot(Vector2{1.0f / transform.GetCos(), 1.0f / transform.GetSin()}) < 0.0f)
      gravity_strength_ = (2.0f * kMaxJumpHeight) / kDescendTime * kDescendTime;
    GAME_LOG(LogType::kInfo) << rigidbody.GetVelocity().dot(Vector2{1.0f / transform.GetCos(), 1.0f / transform.GetSin()}.transpose());



    // Change Air to Grounded
    if(game.GetPhysicsSystem().Raycast(transform.GetPosition(), Vector2{0, -kGroundCheckDistance}).entity != nullptr)
    {
      state_ |= PlayerState::kGroundedBit;

      is_changing_main_state_ = true;
    }

    break;

  default:
    GAME_ASSERT(false);
    break;
  }


  // Apply movement
  jump_velocity_ -= gravity_strength_ * game.GetDeltaTime().count();
  if(jump_velocity_ < 0)
    jump_velocity_ = 0;

  const Vector2 directed_desired_speed_ = move_direction_ * desired_speed_;
  // Check for equals 0 to decelerate when we want to stop
  if(directed_speed_.dot(directed_desired_speed_) <= 0.0f)
  {
    DefaultFloatType timed_deceleration = deceleration * game.GetDeltaTime().count();
    directed_speed_.x() += std::clamp(directed_desired_speed_.x() - directed_speed_.x(),
    -timed_deceleration,
    timed_deceleration);
    directed_speed_.y() += std::clamp(directed_desired_speed_.y() - directed_speed_.y(),
    -timed_deceleration,
    timed_deceleration);
  }
  else
  {
    DefaultFloatType timed_acceleration = acceleration * game.GetDeltaTime().count();
    directed_speed_.x() += std::clamp(directed_desired_speed_.x() - directed_speed_.x(),
                                      -timed_acceleration,
                                      timed_acceleration);
    directed_speed_.y() += std::clamp(directed_desired_speed_.y() - directed_speed_.y(),
                                      -timed_acceleration,
                                      timed_acceleration);
  }
  rigidbody.SetVelocity(directed_speed_ + gravity_direction_ * gravity_strength_ + jump_direction_ * (jump_velocity_ * 2));

  //GAME_DLOG(LogType::kInfo) << directed_speed_ + gravity_direction_ * gravity_strength_ + jump_direction_ * (jump_velocity_ * 2);
  // TODO: separate file / class
  // Move camera
  //float diff = transform.GetPosition().x() - camera_transform.GetPosition().x(); 
  //if(std::abs(diff) > 10)
  //  camera_transform.SetPosition(VectorLerp(camera_transform.GetPosition(), Vector2{camera_transform.GetPosition().x() + diff - 30 * Sign(diff), camera_transform.GetPosition().y()}));
  
  // TODO: separate file / class
  // Animation
  static std::chrono::duration<double> animation_speed = 0.5s;
  static std::chrono::duration<double> animation_counter = 0s;
  animation_counter += game.GetDeltaTime();
  if(animation_counter > animation_speed)
  {
    animation_counter = 0s;
    static int sprite_index = 0;
    sprite_index = (sprite_index + 1) % 4;
    dynamic_cast<AnimatedSpriteData*>(entity_->GetComponent<SpriteComponent>().GetData())->SetSpriteIndex(Vector2i{sprite_index % 2, sprite_index / 2});
  } 
}
} // game