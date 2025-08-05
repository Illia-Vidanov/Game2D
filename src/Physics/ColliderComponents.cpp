#include "ColliderComponents.hpp"

#include "Setup.hpp"

#include "Core/Game.hpp"
#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Core/Entity.hpp"
#include "Utils/Math.hpp"
#include "Physics/VectorMath.hpp"
#include "Core/AllComponents.hpp"


namespace game
{
RectangleColliderComponent::RectangleColliderComponent(Entity *entity) noexcept
  : ComponentBase{entity}
{
  ZoneScopedC(0xffa443);

  b2Polygon polygon = Calculateb2Polygon();
  b2ShapeDef shape_definition = b2DefaultShapeDef();
  b2BodyId body_id = GetOrCreateb2BodyId();
  shape_id_ = b2CreatePolygonShape(body_id, &shape_definition, &polygon);
}

RectangleColliderComponent::RectangleColliderComponent(Entity *entity, const b2ShapeDef &shape_definition) noexcept
  : ComponentBase{entity}
{
  ZoneScopedC(0xffa443);

  b2Polygon polygon = Calculateb2Polygon();
  b2BodyId body_id = GetOrCreateb2BodyId();
  shape_id_ = b2CreatePolygonShape(body_id, &shape_definition, &polygon);
}

RectangleColliderComponent::~RectangleColliderComponent() noexcept
{
  ZoneScopedC(0xffa443);

  b2DestroyShape(shape_id_, true);
  
  if(!entity_->HasAnyComponentPack<PhysicsSystem::UseBodyIdComponentsPack>())
    entity_->GetGame().GetPhysicsSystem().RemoveEntity(entity_);
}

void RectangleColliderComponent::Updateb2() const noexcept
{
  ZoneScopedC(0xffa443);

  b2Polygon polygon = Calculateb2Polygon();
  b2Shape_SetPolygon(shape_id_, &polygon);
}

auto RectangleColliderComponent::Calculateb2Polygon() const noexcept -> b2Polygon
{
  ZoneScopedC(0xffa443);

  const TransformComponent &transform = entity_->GetComponent<TransformComponent>();
  return b2MakeOffsetBox(half_size_.x() * transform.GetScale().x(), half_size_.y() * transform.GetScale().y(), Tob2Vec2(offset_), Tob2Rot(angle_radians_ + transform.GetRotationRadians()));
}

auto RectangleColliderComponent::GetOrCreateb2BodyId() const noexcept -> b2BodyId
{
  b2BodyId body_id = entity_->GetGame().GetPhysicsSystem().TryGetb2BodyId(entity_);
  if(PhysicsSystem::Isb2BodyIdNull(body_id))
  {
    b2BodyDef body_definition = b2DefaultBodyDef();
    const TransformComponent &transform = entity_->GetComponent<TransformComponent>();
    body_definition.position = transform.Getb2Position();
    body_definition.rotation = transform.Getb2Rotation();
    body_definition.type = b2_kinematicBody;

    body_id = entity_->GetGame().GetPhysicsSystem().Createb2Body(entity_, body_definition);
  }

  return body_id;
}
} // game