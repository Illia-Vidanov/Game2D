#include "Input.hpp"

#include "Setup.hpp"

#include <array>

#include "Core/Game.hpp"
#include "Core/EventHandler.hpp"

namespace game
{
Input::Input(Game &game) noexcept
  : game_(game)
  , event_cleaner_(game_.GetEventHandler())
{
  game_.GetEventHandler().AddListener(event_cleaner_, EventType::kKeyDown, this, [](const Event &event, void *input) -> bool { return reinterpret_cast<Input*>(input)->KeyDownEvent(event); });
  game_.GetEventHandler().AddListener(event_cleaner_, EventType::kKeyUp, this, [](const Event &event, void *input) -> bool { return reinterpret_cast<Input*>(input)->KeyUpEvent(event); });
}

void Input::InitEvents() noexcept
{
}

void Input::Update() noexcept
{
  std::fill(down_keys_.begin(), down_keys_.end(), 0);
  std::fill(up_keys_.begin(), up_keys_.end(), 0);
}

auto Input::KeyDownEvent(const Event &event) noexcept -> bool
{
  down_keys_[event.GetScancode()] = true;
  keys_[event.GetScancode()] = true;
  ++pressed_keys_;
  last_pressed_key_ = event.GetScancode();

  return false;
}

auto Input::KeyUpEvent(const Event &event) noexcept -> bool
{
  up_keys_[event.GetScancode()] = false;
  keys_[event.GetScancode()] = false;
  --pressed_keys_;

  return false;
}
} // game
