#ifndef GAME_PLAYER_HPP
#define GAME_PLAYER_HPP

#include "Setup.hpp"

#include <Eigen/Dense>
#include <Eigen/Geometry>

#include "Utils/Math.hpp"
#include "Utils/MathConstants.hpp"
#include "Rendering/Sprite.hpp"
#include "Core/EventHandler.hpp"

namespace game
{
class Game;

class Player
{
public:
  Player(Game &game_) noexcept;

  [[nodiscard]] auto GetTransform() const noexcept -> const Transform & { return transform_; }
  [[nodiscard]] auto GetTransform() noexcept -> Transform & { return transform_; }

  void Update() noexcept;

private:
  Game &game_;
  Sprite sprite_;
  Transform transform_ = Transform::Identity();
  EventCleaner event_cleaner_;
};
}

#endif