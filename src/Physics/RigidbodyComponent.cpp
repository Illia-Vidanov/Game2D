#include "RigidbodyComponent.hpp"

#include "Setup.hpp"

#include "Utils/MathConstants.hpp"
#include "Utils/Logger.hpp"
#include "Core/Game.hpp"
#include "Core/Entity.hpp"
#include "Core/AllComponents.hpp"


namespace game
{
RigidbodyComponent::RigidbodyComponent(Entity *entity) noexcept
  : ComponentBase{entity}
{
  ZoneScopedC(0xd4ff8f);

  body_id_ = entity_->GetGame().GetPhysicsSystem().TryGetb2BodyId(entity_);
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

  body_id_ = entity_->GetGame().GetPhysicsSystem().Createb2Body(entity_, body_definition);
}

RigidbodyComponent::RigidbodyComponent(Entity *entity, const b2BodyDef &body_definition) noexcept
  : ComponentBase{entity}
{
  ZoneScopedC(0xd4ff8f);

  body_id_ = entity_->GetGame().GetPhysicsSystem().TryGetb2BodyId(entity_);
  if(!PhysicsSystem::Isb2BodyIdNull(body_id_))
  {
    // No idea if it's really helps but I decided to sisable body before altering it's all states
    b2Body_Disable(body_id_);

    b2Body_SetAngularDamping(body_id_, body_definition.angularDamping);
    b2Body_SetAngularVelocity(body_id_, body_definition.angularVelocity);
    b2Body_SetAwake(body_id_, body_definition.isAwake);
    b2Body_SetBullet(body_id_, body_definition.isBullet);
    b2Body_SetFixedRotation(body_id_, body_definition.fixedRotation);
    b2Body_SetGravityScale(body_id_, body_definition.gravityScale);
    b2Body_SetLinearDamping(body_id_, body_definition.linearDamping);
    b2Body_SetLinearVelocity(body_id_, body_definition.linearVelocity);
    b2Body_SetName(body_id_, body_definition.name);
    b2Body_SetSleepThreshold(body_id_, body_definition.sleepThreshold);
    b2Body_SetTransform(body_id_, body_definition.position, body_definition.rotation);
    b2Body_SetType(body_id_, body_definition.type);
    b2Body_SetUserData(body_id_, body_definition.userData);

    if(body_definition.isEnabled)
      b2Body_Enable(body_id_);
    return;
  }

  body_id_ = entity_->GetGame().GetPhysicsSystem().Createb2Body(entity_, body_definition);
}

RigidbodyComponent::~RigidbodyComponent() noexcept
{
  if(!entity_->HasAnyComponentPack<PhysicsSystem::UseBodyIdComponentsPack>())
    entity_->GetGame().GetPhysicsSystem().RemoveEntity(entity_);
}
} // game