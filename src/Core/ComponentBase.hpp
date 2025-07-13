#ifndef GAME_COMPONENT_BASE_HPP
#define GAME_COMPONENT_BASE_HPP

#include "Setup.hpp"


namespace game
{
class Entity;

class ComponentBase
{
public:
  ComponentBase(Entity *entity) noexcept : entity_{entity} {}

  [[nodiscard]] constexpr auto GetEntity() noexcept -> Entity & { return *entity_; }
  [[nodiscard]] constexpr auto GetEntity() const noexcept -> const Entity & { return *entity_; }

  [[nodiscard]] constexpr auto GetActive() const noexcept -> bool { return active_; }
  constexpr auto SetActive(bool active) noexcept { active_ = active; }

protected:
  Entity *entity_;
  bool active_ = true;
};
} // game

#endif // GAME_COMPONENT_BASE_HPP