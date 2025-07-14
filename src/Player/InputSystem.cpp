#include "InputSystem.hpp"

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Core/Game.hpp"
#include "Utils/Enum.hpp"
#include "Core/Events.hpp"

namespace game
{
  InputSystem::InputSystem(Game &game) noexcept
  : game_(game)
{
  ZoneScopedC(0x4907da);
}

void InputSystem::Update() noexcept
{
  ZoneScopedC(0x4907da);

  if(down_keys_[SDL_SCANCODE_GRAVE])
    game_.GetDebug().SetActive(!game_.GetDebug().GetActive());
  
  std::fill(down_keys_.begin(), down_keys_.end(), 0);
  std::fill(up_keys_.begin(), up_keys_.end(), 0);
}

void InputSystem::KeyDownEvent(const events::Key &event) noexcept
{
  ZoneScopedC(0x4907da);

  down_keys_[event.scancode] = true;
  keys_[event.scancode] = true;
  ++pressed_keys_;
  last_pressed_key_ = event.scancode;
}

void InputSystem::KeyUpEvent(const events::Key &event) noexcept
{
  ZoneScopedC(0x4907da);

  up_keys_[event.scancode] = true;
  keys_[event.scancode] = false;
  --pressed_keys_;
}

void InputSystem::MouseButtonDownEvent(const events::MouseButton &event) noexcept
{
  down_keys_[static_cast<std::size_t>(event.button) + MouseButton::kOffset] = true;
  keys_[static_cast<std::size_t>(event.button) + MouseButton::kOffset] = true;
}

void InputSystem::MouseButtonUpEvent(const events::MouseButton &event) noexcept
{
  up_keys_[static_cast<std::size_t>(event.button) + MouseButton::kOffset] = true;
  keys_[static_cast<std::size_t>(event.button) + MouseButton::kOffset] = false;
}

void InputSystem::MouseMotionEvent(const events::MouseMotion &event) noexcept
{
  mouse_position_ = Vector2i{event.position_x, event.position_y};
  mouse_delta_ = Vector2i{event.delta_x, event.delta_y};
}
} // game