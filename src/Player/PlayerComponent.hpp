#ifndef GAME_PLAYER_COMPONENT_HPP
#define GAME_PLAYER_COMPONENT_HPP

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Core/ComponentBase.hpp"
#include "Utils/MathConstants.hpp"


namespace game
{
class Entity;

class PlayerComponent : public ComponentBase
{
public:
  PlayerComponent(Entity *entity) noexcept;
  void Update() noexcept;
};
} // game

#endif // GAME_PLAYER_COMPONENT_HPP