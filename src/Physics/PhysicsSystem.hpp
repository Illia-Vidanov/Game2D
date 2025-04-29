#ifndef GAME_PHYSICS_SYSTEM_HPP
#define GAME_PHYSICS_SYSTEM_HPP

#include "Setup.hpp"

#include <vector>
#include <algorithm>

#include <entt.hpp>
#include <Eigen/Dense>
#include <Eigen/Geometry>

#include "Physics/ColliderComponents.hpp"
#include "Physics/TransformComponent.hpp"
#include "Utils/MathConstants.hpp"


namespace game
{
class Game;

class PhysicsSystem
{
public:
  PhysicsSystem(Game &game);

  void Update() noexcept;

private:
  decltype(std::declval<entt::registry>().group<const SquareColliderComponent>(entt::get<TransformComponent>)) square_colliders_;

  [[nodiscard]] inline auto ToWorldRect(const Vector4 &rect, const TransformComponent &transform) noexcept { return Vector4{rect(0) + transform.translation()(0), rect(1) + transform.translation()(1), rect(2) * transform.linear().diagonal()(0), rect(2) * transform.linear().diagonal()(1)}; }
};
} // game

#endif // GAME_PHYSICS_SYSTEM_HPP