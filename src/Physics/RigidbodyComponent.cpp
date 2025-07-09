#include "RigidbodyComponent.hpp"

#include "Setup.hpp"

#include "Utils/MathConstants.hpp"
#include "Utils/Logger.hpp"
#include "Core/Game.hpp"
#include "Core/Entity.hpp"
#include "Physics/TransformComponent.hpp"


namespace game
{
RigidbodyComponent::RigidbodyComponent(Entity *entity) noexcept
  : entity_{entity}
{
  ZoneScopedC(0xd4ff8f);

  body_id_ = entity_->GetGame().GetPhysicsSystem().Getb2BodyId(entity_);
  if(!PhysicsSystem::Isb2BodyIdNull(body_id_))
  {
    b2Body_SetType(body_id_, b2_dynamicBody);
    return;
  }

  b2BodyDef body_definition = b2DefaultBodyDef();
  body_definition.type = b2_dynamicBody;
  const TransformComponent &transform = entity_->GetComponent<TransformComponent>();
  body_definition.position = transform.Getb2Position();
  body_definition.rotation = transform.Getb2Rotation();

  body_id_ = b2CreateBody(entity_->GetGame().GetPhysicsSystem().GetWorldId(), &body_definition);
  entity_->GetGame().GetPhysicsSystem().Addb2BodyId(entity_, body_id_);
}

RigidbodyComponent::~RigidbodyComponent() noexcept
{
  entity_->GetGame().GetPhysicsSystem().RemoveEntity(entity_);
}
} // game