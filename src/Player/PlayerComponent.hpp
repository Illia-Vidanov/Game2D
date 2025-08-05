#ifndef GAME_PLAYER_COMPONENT_HPP
#define GAME_PLAYER_COMPONENT_HPP

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Core/ComponentBase.hpp"
#include "Utils/Math.hpp"


namespace game
{
class Entity;

// All states like grounded etc. are here to get everything state machine need in one place
// Main state  Sub-state
// Grounded,   State
// 0           0 x 31
enum class PlayerState
{
  kNone,
  kIdle,
  kWalking,

  kAir = kNone, // kNone because Air state is when ground state is 0
  kGroundedBit = Bitmask(31),
  kMainBits = kGroundedBit,
};


class PlayerComponent : public ComponentBase
{
public:
  PlayerComponent(Entity *entity) noexcept;
  void Update() noexcept;

  // Constants (not const and private to be able to change by debugging features)
  // All speeds are in units per second
  DefaultFloatType kGroundAcceleration = 10.0f;
  DefaultFloatType kGroundDeceleration = 10.0f;
  DefaultFloatType kAirAcceleration = 6.0f;
  DefaultFloatType kAirDeceleration = 10.0f;

  DefaultFloatType kDefaultGravityStrength = 10.0f;
  DefaultFloatType kGroundCheckDistance = 5.5f;
  DefaultFloatType kMaxJumpHeight = 10.0f;
  DefaultFloatType kMinJumpHeight = 5.0f;
  DefaultFloatType kAscendTime = 1.0f;
  DefaultFloatType kDescendTime = 3.0f;

  Vector2 jump_direction_ = Vector2{0.0f, 1.0f};
  DefaultFloatType jump_velocity_ = 0.0f;

  Vector2 directed_speed_ = Vector2::Zero();
  DefaultFloatType desired_speed_ = 0.0f;
  Vector2 gravity_direction_ = Vector2{0.0f, -1.0f};
  DefaultFloatType gravity_strength_ = 10.0f;
  Vector2 move_direction_ = Vector2::Zero();

  PlayerState state_;
  bool is_changing_main_state_ = true; // true because at the creation we enter the state
};
} // game

#endif // GAME_PLAYER_COMPONENT_HPP