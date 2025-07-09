#ifndef GAME_INPUT_SYSTEM_HPP
#define GAME_INPUT_SYSTEM_HPP

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Core/EventSystem.hpp"


namespace game
{
class Game;

enum class MouseButton
{
  kOffset = 300,
  kLeft,
  kMiddle,
  kRight
};

class InputSystem
{
public:
  InputSystem(Game &game) noexcept;
  InputSystem(const InputSystem &) noexcept = delete;
  InputSystem(InputSystem &&) noexcept = delete;
  InputSystem &operator=(const InputSystem &) noexcept = delete;
  InputSystem &operator=(InputSystem &&) noexcept = delete;

  void Update() noexcept;

  [[nodiscard]] auto GetKey(SDL_Scancode scancode) const noexcept -> bool { return keys_[scancode]; }
  [[nodiscard]] auto GetKey(SDL_Keycode keycode) const noexcept -> bool { return keys_[SDL_GetScancodeFromKey(keycode)]; }
  [[nodiscard]] auto GetKeyDown(SDL_Scancode scancode) const noexcept -> bool { return down_keys_[scancode]; }
  [[nodiscard]] auto GetKeyDown(SDL_Keycode keycode) const noexcept -> bool { return down_keys_[SDL_GetScancodeFromKey(keycode)]; }
  [[nodiscard]] auto GetKeyUp(SDL_Scancode scancode) const noexcept -> bool { return up_keys_[scancode]; }
  [[nodiscard]] auto GetKeyUp(SDL_Keycode keycode) const noexcept -> bool { return up_keys_[SDL_GetScancodeFromKey(keycode)]; }
  [[nodiscard]] auto GetMouseButtonDown(const MouseButton mouse_button) const noexcept -> bool { return down_keys_[static_cast<std::size_t>(mouse_button)]; }
  [[nodiscard]] auto GetMouseButtonUp(const MouseButton mouse_button) const noexcept -> bool { return up_keys_[static_cast<std::size_t>(mouse_button)]; }
  [[nodiscard]] auto GetMouseButton(const MouseButton mouse_button) const noexcept -> bool { return keys_[static_cast<std::size_t>(mouse_button)]; }

  auto KeyDownEvent(const Event &event) noexcept -> bool;
  auto KeyUpEvent(const Event &event) noexcept -> bool;
  auto MouseButtonDownEvent(const Event &event) noexcept -> bool;
  auto MouseButtonUpEvent(const Event &event) noexcept -> bool;
  auto MouseMotionEvent(const Event &event) noexcept -> bool;

  [[nodiscard]] auto GetMousePosition() const noexcept -> const Vector2i & { return mouse_position_; }
  [[nodiscard]] auto GetMousePositionX() const noexcept -> int { return mouse_position_.x(); }
  [[nodiscard]] auto GetMousePositionY() const noexcept -> int { return mouse_position_.y(); }
  [[nodiscard]] auto GetMouseDelta() const noexcept -> const Vector2i & { return mouse_delta_; }
  [[nodiscard]] auto GetMouseDeltaX() const noexcept -> int { return mouse_delta_.x(); }
  [[nodiscard]] auto GetMouseDeltaY() const noexcept -> int { return mouse_delta_.y(); }


private:
  int pressed_keys_ = 0;
  int last_pressed_key_ = 0;
  std::array<bool, 512> keys_ = {};
  std::array<bool, 512> down_keys_ = {};
  std::array<bool, 512> up_keys_ = {};

  Vector2i mouse_position_ = Vector2i::Zero();
  Vector2i mouse_delta_ = Vector2i::Zero();

  Game &game_;
  EventCleaner event_cleaner_;
};
} // game

#endif // GAME_INPUT_SYSTEM_HPP