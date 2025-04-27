#ifndef GAME_SQUARE_COLLIDER_HPP
#define GAME_SQUARE_COLLIDER_HPP

#include "Setup.hpp"

#include <Eigen/Dense>

#include "Utils/MathConstants.hpp"


namespace game
{
class Game;

class SquareCollider
{
public:
  SquareCollider(Game &game, Transform &transform) noexcept;
  ~SquareCollider() noexcept;

  [[nodiscard]] constexpr inline auto GetRect() noexcept -> Vector4 & { return rect_; }
  [[nodiscard]] constexpr inline auto GetRect() const noexcept -> const Vector4 & { return rect_; }
  [[nodiscard]] inline auto GetWorldRect() const noexcept -> Vector4 { return Vector4{rect_(0) + transform_.translation()(0), rect_(1) + transform_.translation()(1), rect_(2) * transform_.linear().diagonal()(0), rect_(2) * transform_.linear().diagonal()(1)}; }

private:
  Game &game_;
  Transform &transform_;
  Vector4 rect_ = Vector4::Ones();
};
} // game

#endif