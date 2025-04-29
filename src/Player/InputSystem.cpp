#include "InputSystem.hpp"

#include "Setup.hpp"

#include "Core/Game.hpp"
#include "Core/EventSystem.hpp"

namespace game
{
  InputSystem::InputSystem(Game &game) noexcept
  : game_(game)
  , event_cleaner_(game_.GetEventSystem())
{
  ZoneScopedC(0x4907da);

  game_.GetEventSystem().AddListener(event_cleaner_, EventType::kKeyDown, this, [](const Event &event, void *input) -> bool { return reinterpret_cast<InputSystem*>(input)->KeyDownEvent(event); });
  game_.GetEventSystem().AddListener(event_cleaner_, EventType::kKeyUp, this, [](const Event &event, void *input) -> bool { return reinterpret_cast<InputSystem*>(input)->KeyUpEvent(event); });
}

void InputSystem::InitEvents() noexcept
{
  ZoneScopedC(0x4907da);
}

void InputSystem::Update() noexcept
{
  ZoneScopedC(0x4907da);

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

  up_keys_[event.GetScancode()] = false;
  keys_[event.GetScancode()] = false;
  --pressed_keys_;

  return false;
}
} // game
