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
  Debug(Game &game) : game_{game} {}

  [[nodiscard]] constexpr auto GetActive() const noexcept -> bool { return active_; }
  constexpr void SetActive(bool active) noexcept { active_ = active; }
  [[nodiscard]] constexpr auto GetSelectMode() const noexcept -> bool { return select_mode_; }
  constexpr void SetSelectMode(bool select_mode) noexcept { select_mode_ = select_mode; if(!select_mode_) selected_entities_.clear(); }
  [[nodiscard]] auto GetSelectedEntities() const noexcept -> const std::vector<Entity*> & { return selected_entities_; }

  void Update() noexcept;

private:
  void UpdateSelected() noexcept;

  Game &game_;

  bool active_ = false;
  bool select_mode_ = false;
  std::vector<Entity*> selected_entities_;
};
} // game

#endif // GAME_DEBUG_HPP