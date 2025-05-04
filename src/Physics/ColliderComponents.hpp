#ifndef GAME_COLLIDER_COMPONENTS_HPP
#define GAME_COLLIDER_COMPONENTS_HPP

#include "Setup.hpp"


namespace game
{
class Game;

// Position is updated with tranform
// Size and rotation are not!

class RectangleColliderComponent
{
public:
  RectangleColliderComponent(entt::entity self, Game &game) noexcept;

  [[nodiscard]] constexpr inline auto GetHalfSize() const noexcept -> const Vector2 & { return half_size_; }
  inline void SetHalfSize(const Vector2 &half_size) noexcept { half_size_ = half_size; Updateb2(); }
  [[nodiscard]] constexpr inline auto GetOffset() const noexcept -> const Vector2 & { return offset_; }
  inline void SetOffset(const Vector2 &offset) noexcept { offset_ = offset; Updateb2(); }
  [[nodiscard]] constexpr inline auto GetAngle() const noexcept -> DefFloatType { return angle_; }
  inline void SetAngle(DefFloatType angle) noexcept { angle_ = angle; Updateb2(); }
  
  void Updateb2() const noexcept;
  
private:
  entt::entity self_;
  Game &game_;

  b2ShapeId shape_id_;
  Vector2 half_size_ = Vector2{0.5f, 0.5f};
  Vector2 offset_ = Vector2::Zero();
  DefFloatType angle_ = 0;

  [[nodiscard]] auto Getb2Polygon() const noexcept -> b2Polygon;
};
} // game

#endif // GAME_COLLIDER_COMPONENTS_HPP