#ifndef GAME_PLAYER_COMPONENT_HPP
#define GAME_PLAYER_COMPONENT_HPP

#include "Setup.hpp"

#include <entt.hpp>


namespace game
{
class Game;

class PlayerComponent
{
public:
  void Initialize(entt::entity self, Game &game);
  void Update() noexcept;

private:
  entt::entity self_;
  Game *game_;
};
} // game

#endif // GAME_PLAYER_COMPONENT_HPP