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

private:
  Entity *entity_;
};
} // game

#endif // GAME_PLAYER_COMPONENT_HPP