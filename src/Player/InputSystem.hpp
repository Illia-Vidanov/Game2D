#ifndef GAME_INPUT_SYSTEM_HPP
#define GAME_INPUT_SYSTEM_HPP

#include "Setup.hpp"

#include <array>

#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_keyboard.h>

#include "Core/EventSystem.hpp"


namespace game
{
class Game;

class InputSystem
{
public:
  InputSystem(Game &game) noexcept;
  void InitEvents() noexcept;

  void Update() noexcept;

  [[nodiscard]] auto GetKey(SDL_Scancode scancode) const noexcept -> bool { return keys_[scancode]; }
  [[nodiscard]] auto GetKey(SDL_Keycode keycode) const noexcept -> bool { return keys_[SDL_GetScancodeFromKey(keycode)]; }
  [[nodiscard]] auto GetKeyDown(SDL_Scancode scancode) const noexcept -> bool { return down_keys_[scancode]; }
  [[nodiscard]] auto GetKeyDown(SDL_Keycode keycode) const noexcept -> bool { return down_keys_[SDL_GetScancodeFromKey(keycode)]; }
  [[nodiscard]] auto GetKeyUp(SDL_Scancode scancode) const noexcept -> bool { return up_keys_[scancode]; }
  [[nodiscard]] auto GetKeyUp(SDL_Keycode keycode) const noexcept -> bool { return up_keys_[SDL_GetScancodeFromKey(keycode)]; }

  auto KeyDownEvent(const Event &event) noexcept -> bool;
  auto KeyUpEvent(const Event &event) noexcept -> bool;


private:
  int pressed_keys_ = 0;
  int last_pressed_key_ = 0;
  std::array<bool, 512> keys_ = {};
  std::array<bool, 512> down_keys_ = {};
  std::array<bool, 512> up_keys_ = {};

  Game &game_;
  EventCleaner event_cleaner_;
};
} // game

#endif // GAME_INPUT_SYSTEM_HPP