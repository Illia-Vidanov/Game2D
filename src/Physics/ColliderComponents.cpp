#include "ColliderComponents.hpp"

#include "Setup.hpp"

#include "Core/Game.hpp"
#include "Utils/Math.hpp"


namespace game
{
RectangleColliderComponent::RectangleColliderComponent(entt::entity self, Game &game) noexcept
  : self_{self}
  , game_{game}
{
  ZoneScopedC(0xffa443);

  b2Polygon polygon = Getb2Polygon();
  b2ShapeDef shape_definition = b2DefaultShapeDef();

  b2BodyId body_id = game_.GetPhysicsSystem().Getb2BodyId(self_);
  if(PhysicsSystem::Isb2BodyIdNull(body_id))
  {
    b2BodyDef body_definition = b2DefaultBodyDef();
    body_definition.position = game_.GetRegistry().get<TransformComponent>(self_).Getb2Position();
    body_definition.rotation = game_.GetRegistry().get<TransformComponent>(self_).Getb2Rotation();
    game_.GetPhysicsSystem().Addb2BodyId(self_, body_id = b2CreateBody(game_.GetPhysicsSystem().GetWorldId(), &body_definition));
  }

  shape_id_ = b2CreatePolygonShape(body_id, &shape_definition, &polygon);
}

void RectangleColliderComponent::Updateb2() const noexcept
{
  ZoneScopedC(0xffa443);

  b2Polygon polygon = Getb2Polygon();
  b2Shape_SetPolygon(shape_id_, &polygon);
}

auto RectangleColliderComponent::Getb2Polygon() const noexcept -> b2Polygon
{
  ZoneScopedC(0xffa443);

  if(TransformComponent *transform = game_.GetRegistry().try_get<TransformComponent>(self_))
    return b2MakeOffsetBox(half_size_.x() * transform->GetScale().x(), half_size_.y() * transform->GetScale().y(), Tob2Vec2(offset_), Tob2Rot(angle_ + transform->GetRotationAngle()));
  else
    return b2MakeOffsetBox(half_size_.x(), half_size_.y(), Tob2Vec2(offset_), Tob2Rot(angle_));
}
} // game