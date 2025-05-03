#include "PhysicsSystem.hpp"

#include "Setup.hpp"

#include "Physics/ColliderComponents.hpp"
#include "Physics/TransformComponent.hpp"
#include "Physics/RigidbodyComponent.hpp"
#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Core/Game.hpp"


namespace game
{
PhysicsSystem::PhysicsSystem(Game &game)
  : game_{game}
{
  ZoneScopedC(0xfc940a);

  b2WorldDef world_def = b2DefaultWorldDef();
  world_id_ = b2CreateWorld(&world_def);
}

void PhysicsSystem::Update() noexcept
{
  ZoneScopedC(0xfc940a);

  entt::view<entt::get_t<RigidbodyComponent>> rigidbodies;
  for(entt::entity entity : rigidbodies)
  {
    TransformComponent &transform = game_.GetRegistry().get<TransformComponent>(entity);
    transform.SetPositionWithoutRigidbodyUpdate(transform.GetPosition() + ToNormalVector2(b2Body_GetLinearVelocity(Getb2BodyId(entity))));
    transform.SetRotationAngleWithoutRigidbodyUpdate(transform.GetRotationAngle() + b2Body_GetAngularVelocity(Getb2BodyId(entity)));
  }
}
} // game