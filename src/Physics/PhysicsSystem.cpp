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
    b2BodyId body_id = entity_to_body_map_[&transform.GetEntity()];

    if(!rigidbodies.get<RigidbodyComponent>(entity).GetActive() || !transform.GetEntity().GetActive())
    {
      if(b2Body_IsEnabled(body_id))
        b2Body_Disable(body_id);
      continue;
    }

    if(!b2Body_IsEnabled(body_id))
      b2Body_Enable(body_id);

    transform.SetPosition(ToNormalVector2(b2Body_GetPosition(body_id)));
    b2Rot rotation = b2Body_GetRotation(body_id);
    transform.SetSinAndCos(Vector2{rotation.s, rotation.c});
  }
}

void PhysicsSystem::Updateb2Transform(Entity *entity) const noexcept
{
  std::unordered_map<Entity*, b2BodyId>::const_iterator body_id = entity_to_body_map_.find(entity);
  if(body_id != entity_to_body_map_.end())
    b2Body_SetTransform(body_id->second, entity->GetComponent<TransformComponent>().Getb2Position(), entity->GetComponent<TransformComponent>().Getb2Rotation());
}

auto PhysicsSystem::TestPoint(const Vector2 &position) noexcept -> std::vector<Entity*>
{
  std::vector<Entity*> entities;
  void *context = new std::pair<Game&, std::vector<Entity*>&>{game_, entities};
  b2World_OverlapAABB(world_id_,
                      b2AABB{b2Vec2{position.x() - (kPointTestSize / 2.0f), position.y() - (kPointTestSize / 2.0f)}, b2Vec2{position.x() + (kPointTestSize / 2.0f), position.y() + (kPointTestSize / 2.0f)}},
                      b2DefaultQueryFilter(),
                      [](b2ShapeId shape_id, void *context) -> bool
                      {
                        const std::pair<Game&, std::vector<Entity*>&> &data = *reinterpret_cast<std::pair<Game&, std::vector<Entity*>&>*>(context);
                        data.second.emplace_back(data.first.GetPhysicsSystem().GetEntity(b2Shape_GetBody(shape_id)));
                        return true;
                      },
                      context);
  delete reinterpret_cast<std::pair<Game&, std::vector<Entity*>&>*>(context);
  return entities;
}
} // game