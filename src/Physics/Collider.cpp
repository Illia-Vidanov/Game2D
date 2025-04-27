#include "Collider.hpp"

#include "Setup.hpp"

#include <Eigen/Dense>

#include "Core/Game.hpp"
#include "Utils/MathConstants.hpp"


namespace game
{
SquareCollider::SquareCollider(Game &game, Transform &transform) noexcept
  : game_{game}
  , transform_{transform}
{
  game_.GetPhysics().AddSquareCollider(this);
}

SquareCollider::~SquareCollider() noexcept
{
  game_.GetPhysics().RemoveSquareCollider(this);
}

} // game