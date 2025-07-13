#ifndef GAME_UI_HPP
#define GAME_UI_HPP

#include "Setup.hpp"

#include "Core/EventSystem.hpp"
#include "Utils/MathConstants.hpp"


namespace game
{
class Game;
class Entity;
class TransformComponent;

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
  auto WindowResizeEvent(const Event &event) noexcept -> bool;
  void UpdateWindow() const noexcept;

  void DrawDebugWindow() const noexcept;
  void DrawTransformComponent(TransformComponent *transform, Entity *entity) const noexcept;

  Game &game_;
  EventCleaner event_cleaner_;

  Vector2 global_scale_;
  Vector2 scale_multiplier_;
  static constexpr inline DefaultFloatType kUnitScale = 100.0f;

  bool window_resized_ = false;
};
} // game

#endif // GAME_UI_HPP