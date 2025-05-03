#ifndef GAME_PLAYER_COMPONENT_HPP
#define GAME_PLAYER_COMPONENT_HPP

#include "Setup.hpp"


namespace game
{
class Game;

class PlayerComponent
{
public:
  PlayerComponent(entt::entity self, Game &game) noexcept;
  void Update() noexcept;

private:
  entt::entity self_;
  Game &game_;
};
} // game

#endif // GAME_PLAYER_COMPONENT_HPP