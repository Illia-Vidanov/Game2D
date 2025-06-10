#include "ColliderComponents.hpp"

#include "Setup.hpp"

#include "Core/Game.hpp"
#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Core/Entity.hpp"
#include "Utils/Math.hpp"
#include "Physics/TransformComponent.hpp"


namespace game
{
RectangleColliderComponent::RectangleColliderComponent(Entity &entity) noexcept
  : entity_{entity}
{
  GAME_ASSERT(entity_.HasComponent<TransformComponent>());

  ZoneScopedC(0xffa443);

  b2Polygon polygon = Getb2Polygon();
  b2ShapeDef shape_definition = b2DefaultShapeDef();

  b2BodyId body_id = entity_.GetGame().GetPhysicsSystem().Getb2BodyId(entity_.GetID());
  if(PhysicsSystem::Isb2BodyIdNull(body_id))
  {
    b2BodyDef body_definition = b2DefaultBodyDef();
    const TransformComponent &transform = entity_.GetComponent<TransformComponent>();
    body_definition.position = transform.Getb2Position();
    body_definition.rotation = transform.Getb2Rotation();
    body_id = b2CreateBody(entity_.GetGame().GetPhysicsSystem().GetWorldId(), &body_definition);
    entity_.GetGame().GetPhysicsSystem().Addb2BodyId(entity_.GetID(), body_id);
  }

  shape_id_ = b2CreatePolygonShape(body_id, &shape_definition, &polygon);
}

RectangleColliderComponent::~RectangleColliderComponent() noexcept
{
  ZoneScopedC(0xffa443);

  b2DestroyShape(shape_id_, true);
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

  const TransformComponent &transform = entity_.GetComponent<TransformComponent>();
  return b2MakeOffsetBox(half_size_.x() * transform.GetScale().x(), half_size_.y() * transform.GetScale().y(), Tob2Vec2(offset_), Tob2Rot(angle_ + transform.GetRotationAngle()));
}
} // game