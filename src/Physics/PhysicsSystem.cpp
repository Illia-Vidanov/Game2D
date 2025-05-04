#include "PhysicsSystem.hpp"

#include "Setup.hpp"

#include "Physics/ColliderComponents.hpp"
#include "Physics/TransformComponent.hpp"
#include "Physics/RigidbodyComponent.hpp"
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

  b2World_Step(world_id_, 1.0f / 60.0f, 4);

  entt::view<entt::get_t<RigidbodyComponent>> rigidbodies = game_.GetRegistry().view<RigidbodyComponent>();
  for(entt::entity entity : rigidbodies)
  {
    TransformComponent &transform = game_.GetRegistry().get<TransformComponent>(entity);
    transform.SetPosition(ToNormalVector2(b2Body_GetPosition(body_ids_[entity])));
    b2Rot rotation = b2Body_GetRotation(body_ids_[entity]);
    transform.SetSinAndCos(Vector2{rotation.s, rotation.c});
  }
  
  //for(auto it : body_ids_)
  //{
  //  GAME_DLOG(LogType::kInfo) << ToNormalVector2(b2Body_GetPosition(it.second)).transpose();
  //}
  //GAME_DLOG(LogType::kInfo) << ' ';
}
} // game