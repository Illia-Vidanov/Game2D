#ifndef GAME_COLLIDER_COMPONENTS_HPP
#define GAME_COLLIDER_COMPONENTS_HPP

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"


namespace game
{
class Game;
class Entity;

class RectangleColliderComponent
{
public:
  RectangleColliderComponent(Entity *entity) noexcept;
  ~RectangleColliderComponent() noexcept;

  [[nodiscard]] constexpr auto GetHalfSize() const noexcept -> const Vector2 & { return half_size_; }
  void SetHalfSize(const Vector2 &half_size) noexcept { half_size_ = half_size; Updateb2(); }
  [[nodiscard]] constexpr auto GetOffset() const noexcept -> const Vector2 & { return offset_; }
  void SetOffset(const Vector2 &offset) noexcept { offset_ = offset; Updateb2(); }
  [[nodiscard]] constexpr auto GetAngle() const noexcept -> DefaultFloatType { return angle_; }
  void SetAngle(DefaultFloatType angle) noexcept { angle_ = angle; Updateb2(); }

  void SetFriction(DefaultFloatType friction) const noexcept { b2Shape_SetFriction(shape_id_, friction); }
  void SetRestitution(DefaultFloatType restitution) const noexcept { b2Shape_SetRestitution(shape_id_, restitution); }
  void SetDensity(DefaultFloatType density) const noexcept { b2Shape_SetDensity(shape_id_, density, true); }
  
  void Updateb2() const noexcept;

  [[nodiscard]] constexpr auto GetEntity() const noexcept -> const Entity & { return *entity_; }
  [[nodiscard]] constexpr auto GetEntity() noexcept -> Entity & { return *entity_; }
  
private:
  Entity *entity_;

  b2ShapeId shape_id_;
  Vector2 half_size_ = Vector2{0.5f, 0.5f};
  Vector2 offset_ = Vector2::Zero();
  DefaultFloatType angle_ = 0;

  [[nodiscard]] auto Getb2Polygon() const noexcept -> b2Polygon;
};
} // game

#endif // GAME_COLLIDER_COMPONENTS_HPP