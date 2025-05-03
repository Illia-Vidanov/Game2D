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
  b2BodyId body_id;
  if(PhysicsSystem::Isb2BodyIdNull(body_id = game_.GetPhysicsSystem().Getb2BodyId(self_)))
  {
    b2Body_SetType(body_id, b2_dynamicBody);
    return;
  }

  b2BodyDef body_definition = b2DefaultBodyDef();
  body_definition.type = b2_dynamicBody;
  if(const TransformComponent *transform = game_.GetRegistry().try_get<TransformComponent>(self_))
  {
    body_definition.position = transform->Getb2Position();
    body_definition.rotation = transform->Getb2Rotation();
  }

  game_.GetPhysicsSystem().Addb2BodyId(self_, b2CreateBody(game_.GetPhysicsSystem().GetWorldId(), &body_definition));
}
} // game