#include "InputSystem.hpp"

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Core/Game.hpp"
#include "Core/EventSystem.hpp"
#include "Utils/Enum.hpp"

namespace game
{
  InputSystem::InputSystem(Game &game) noexcept
  : game_(game)
  , event_cleaner_(game_.GetEventSystem())
{
  ZoneScopedC(0x4907da);

  game_.GetEventSystem().AddListener(event_cleaner_, EventType::kKeyDown, this, [](const Event &event, void *input) -> bool { return reinterpret_cast<InputSystem*>(input)->KeyDownEvent(event); });
  game_.GetEventSystem().AddListener(event_cleaner_, EventType::kKeyUp, this, [](const Event &event, void *input) -> bool { return reinterpret_cast<InputSystem*>(input)->KeyUpEvent(event); });
  game_.GetEventSystem().AddListener(event_cleaner_, EventType::kMouseMotion, this, [](const Event &event, void *input) -> bool { return reinterpret_cast<InputSystem*>(input)->MouseMotionEvent(event); });
  game_.GetEventSystem().AddListener(event_cleaner_, EventType::kMouseButtonDown, this, [](const Event &event, void *input) -> bool { return reinterpret_cast<InputSystem*>(input)->MouseButtonDownEvent(event); });
  game_.GetEventSystem().AddListener(event_cleaner_, EventType::kMouseButtonUp, this, [](const Event &event, void *input) -> bool { return reinterpret_cast<InputSystem*>(input)->MouseButtonUpEvent(event); });
}

void InputSystem::Update() noexcept
{
  ZoneScopedC(0x4907da);

  if(down_keys_[SDL_SCANCODE_GRAVE])
    game_.GetDebug().SetActive(!game_.GetDebug().GetActive());
  
  std::fill(down_keys_.begin(), down_keys_.end(), 0);
  std::fill(up_keys_.begin(), up_keys_.end(), 0);
}

auto InputSystem::KeyDownEvent(const Event &event) noexcept -> bool
{
  ZoneScopedC(0x4907da);

  down_keys_[event.GetScancode()] = true;
  keys_[event.GetScancode()] = true;
  ++pressed_keys_;
  last_pressed_key_ = event.GetScancode();

  return false;
}

auto InputSystem::KeyUpEvent(const Event &event) noexcept -> bool
{
  ZoneScopedC(0x4907da);

  up_keys_[event.GetScancode()] = true;
  keys_[event.GetScancode()] = false;
  --pressed_keys_;

  return false;
}

auto InputSystem::MouseButtonDownEvent(const Event &event) noexcept -> bool
{
  down_keys_[static_cast<std::size_t>(event.GetMouseButton()) + MouseButton::kOffset] = true;
  keys_[static_cast<std::size_t>(event.GetMouseButton()) + MouseButton::kOffset] = true;

  return false;
}

auto InputSystem::MouseButtonUpEvent(const Event &event) noexcept -> bool
{
  up_keys_[static_cast<std::size_t>(event.GetMouseButton()) + MouseButton::kOffset] = true;
  keys_[static_cast<std::size_t>(event.GetMouseButton()) + MouseButton::kOffset] = false;

  return false;
}

auto InputSystem::MouseMotionEvent(const Event &event) noexcept -> bool
{
  mouse_position_ = event.GetNewMousePosition();
  mouse_delta_ = event.GetMouseDelta();

  return false;
}
} // game
