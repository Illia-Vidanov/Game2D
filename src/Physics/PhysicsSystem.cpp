#include "PhysicsSystem.hpp"

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Physics/ColliderComponents.hpp"
#include "Physics/TransformComponent.hpp"
#include "Physics/RigidbodyComponent.hpp"
#include "Core/Game.hpp"


namespace game
{
PhysicsSystem::PhysicsSystem(Game &game) noexcept
  : game_{game}
{
  ZoneScopedC(0xfc940a);

  b2WorldDef world_def = b2DefaultWorldDef();
  world_def.gravity = b2Vec2{0.0f, -10.0f};
  world_id_ = b2CreateWorld(&world_def);
}

PhysicsSystem::~PhysicsSystem() noexcept
{
  ZoneScopedC(0xfc940a);

  b2DestroyWorld(world_id_);
}

void PhysicsSystem::FixedUpdate() noexcept
{
  ZoneScopedC(0xfc940a);

  b2World_Step(world_id_, 1.0f / 60.0f, 4);

  entt::view<entt::get_t<RigidbodyComponent>> rigidbodies = game_.GetRegistry().view<RigidbodyComponent>();
  for(entt::entity entity : rigidbodies)
  {
    TransformComponent &transform = game_.GetRegistry().get<TransformComponent>(entity);
    transform.SetPosition(ToNormalVector2(b2Body_GetPosition(body_ids_[entity])));
    b2Rot rotation = b2Body_GetRotation(body_ids_[entity]);
    transform.SetSinAndCos(Vector2{rotation.s, rotation.c});
  }
}
} // game