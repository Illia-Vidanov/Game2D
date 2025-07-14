#ifndef GAME_DEBUG_HPP
#define GAME_DEBUG_HPP

#include "Setup.hpp"


namespace game
{
class Game;
class Entity;

class Debug
{
public:
  Debug(Game &game) noexcept;

  [[nodiscard]] constexpr auto GetActive() const noexcept -> bool { return active_; }
  constexpr void SetActive(bool active) noexcept { active_ = active; }
  [[nodiscard]] constexpr auto GetSelectMode() const noexcept -> bool { return select_mode_; }
  constexpr void SetSelectMode(bool select_mode) noexcept { select_mode_ = select_mode; }
  [[nodiscard]] auto GetSelectedEntities() const noexcept -> const std::vector<Entity*> & { return selected_entities_; }

  void Update() noexcept;
  
  private:
  void UpdateSelected() noexcept;

  static constexpr inline int kSquaredSameMousePositionElipson = gcem::pow(2, 2);

  Game &game_;
  bool active_ = false;
  bool select_mode_ = false;
  std::vector<Entity*> selected_entities_;
  Vector2i last_mouse_position_;
  int select_entity_layer_;
};
} // game

#endif // GAME_DEBUG_HPP