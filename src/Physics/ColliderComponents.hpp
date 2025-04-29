#ifndef GAME_COLLIDER_COMPONENTS_HPP
#define GAME_COLLIDER_COMPONENTS_HPP

#include "Setup.hpp"

#include <Eigen/Dense>

#include "Utils/MathConstants.hpp"


namespace game
{
class Game;

class SquareColliderComponent
{
public:
  [[nodiscard]] constexpr inline auto GetRect() noexcept -> Vector4 & { return rect_; }
  [[nodiscard]] constexpr inline auto GetRect() const noexcept -> const Vector4 & { return rect_; }

private:
  Vector4 rect_ = Vector4::Ones();
};
} // game

#endif // GAME_COLLIDER_COMPONENTS_HPP