#ifndef GAME_COLLIDER_COMPONENTS_HPP
#define GAME_COLLIDER_COMPONENTS_HPP

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Core/ComponentBase.hpp"
#include "Utils/Math.hpp"


namespace game
{
class Entity;

class RectangleColliderComponent : public ComponentBase
{
public:
  RectangleColliderComponent(Entity *entity) noexcept;
  RectangleColliderComponent(Entity *entity, const b2ShapeDef &shape_definition) noexcept;
  ~RectangleColliderComponent() noexcept;

  [[nodiscard]] constexpr auto GetHalfSize() const noexcept -> const Vector2 & { return half_size_; }
  void SetHalfSize(const Vector2 &half_size) noexcept { half_size_ = half_size; Updateb2(); }
  [[nodiscard]] constexpr auto GetOffset() const noexcept -> const Vector2 & { return offset_; }
  void SetOffset(const Vector2 &offset) noexcept { offset_ = offset; Updateb2(); }
  [[nodiscard]] constexpr auto GetAngleRadians() const noexcept -> DefaultFloatType { return angle_radians_; }
  void SetAngleRadians(DefaultFloatType angle_radians) noexcept { angle_radians_ = angle_radians; Updateb2(); }
  [[nodiscard]] constexpr auto GetAngleDegrees() const noexcept -> DefaultFloatType { return ToDegrees(angle_radians_); }
  void SetAngleDegrees(DefaultFloatType angle_degrees) noexcept { angle_radians_ = ToRadians(angle_degrees); Updateb2(); }

  void SetFriction(DefaultFloatType friction) const noexcept { b2Shape_SetFriction(shape_id_, friction); }
  void SetRestitution(DefaultFloatType restitution) const noexcept { b2Shape_SetRestitution(shape_id_, restitution); }
  void SetDensity(DefaultFloatType density) const noexcept { b2Shape_SetDensity(shape_id_, density, true); }
  
  void Updateb2() const noexcept;

private:
  [[nodiscard]] auto Calculateb2Polygon() const noexcept -> b2Polygon;
  [[nodiscard]] auto GetOrCreateb2BodyId() const noexcept -> b2BodyId;

  b2ShapeId shape_id_;
  Vector2 half_size_ = Vector2{0.5f, 0.5f};
  Vector2 offset_ = Vector2::Zero();
  DefaultFloatType angle_radians_ = 0;
};
} // game

#endif // GAME_COLLIDER_COMPONENTS_HPP