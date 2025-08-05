#ifndef GAME_GAME_STATE_HPP
#define GAME_GAME_STATE_HPP

#include "Setup.hpp"


namespace game
{
class Game;

class Scene
{
public:
  Scene(Game &game) noexcept : game_{game} {}
  virtual ~Scene() noexcept {}

  virtual void Update() noexcept = 0;

protected:
  Game &game_;
};

class Scene1 : public Scene
{
public:
  Scene1(Game &game) noexcept;

  void Update() noexcept override;
};
}

#endif // GAME_GAME_STATE_HPP