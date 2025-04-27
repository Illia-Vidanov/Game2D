#ifndef GAME_PHYSICS_HPP
#define GAME_PHYSICS_HPP

#include "Setup.hpp"

#include <vector>
#include <algorithm>

namespace game
{
class SquareCollider;

class Physics
{
public:
  inline void AddSquareCollider(const SquareCollider *collider) noexcept { square_colliders_.push_back(collider); }
  inline void RemoveSquareCollider(const SquareCollider *collider) noexcept { std::swap(square_colliders_.back(), *std::find(square_colliders_.begin(), square_colliders_.end(), collider)); square_colliders_.pop_back(); }

  void Update() noexcept;

private:
  std::vector<const SquareCollider*> square_colliders_;


};
} // game

#endif // GAME_PHYSICS_HPP