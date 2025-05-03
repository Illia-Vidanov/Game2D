#include "RigidbodyComponent.hpp"

#include "Setup.hpp"

#include "Core/Game.hpp"
#include "Physics/TransformComponent.hpp"


namespace game
{
RigidbodyComponent::RigidbodyComponent(entt::entity self, Game &game) noexcept
  : self_{self}
  , game_{game}
{
  ZoneScopedC(0xd4ff8f);

  body_id_ = game_.GetPhysicsSystem().Getb2BodyId(self_);
  if(!PhysicsSystem::Isb2BodyIdNull(body_id_))
  {
    b2Body_SetType(body_id_, b2_dynamicBody);
    return;
  }

  b2BodyDef body_definition = b2DefaultBodyDef();
  body_definition.type = b2_dynamicBody;
  const TransformComponent &transform = game_.GetRegistry().get<TransformComponent>(self_);
  body_definition.position = transform.Getb2Position();
  body_definition.rotation = transform.Getb2Rotation();

  body_id_ = b2CreateBody(game_.GetPhysicsSystem().GetWorldId(), &body_definition);
  game_.GetPhysicsSystem().Addb2BodyId(self_, body_id_);
}
} // game