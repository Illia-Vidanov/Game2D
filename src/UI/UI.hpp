#ifndef GAME_UI_HPP
#define GAME_UI_HPP

#include "Setup.hpp"


namespace game
{
class Game;

class UI
{
public:
  UI(Game &game) noexcept;
  ~UI() noexcept;
  UI(const UI &) noexcept = delete;
  UI(UI &&) noexcept = delete;
  UI &operator=(const UI &) noexcept = delete;
  UI &operator=(UI &&) noexcept = delete;

  void Update() noexcept;

private:
  Game &game_;
};
} // game

#endif // GAME_UI_HPP