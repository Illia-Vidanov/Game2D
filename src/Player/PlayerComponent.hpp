#ifndef GAME_PLAYER_COMPONENT_HPP
#define GAME_PLAYER_COMPONENT_HPP

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"


namespace game
{
class Entity;

class PlayerComponent
{
public:
  PlayerComponent(Entity *entity) noexcept;
  void Update() noexcept;

  [[nodiscard]] constexpr auto GetEntity() const noexcept -> const Entity & { return *entity_; }
  [[nodiscard]] constexpr auto GetEntity() noexcept -> Entity & { return *entity_; }

private:
  Entity *entity_;
};
} // game

#endif // GAME_PLAYER_COMPONENT_HPP