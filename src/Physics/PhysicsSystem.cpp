#include "PhysicsSystem.hpp"

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Core/AllComponents.hpp"
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

    transform.SetPosition(ToVector2(b2Body_GetPosition(body_id)));
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

auto PhysicsSystem::Createb2Body(Entity *entity, const b2BodyDef &body_defenition) noexcept -> b2BodyId
{
  GAME_ASSERT(!Hasb2BodyId(entity));

  b2BodyId body_id = b2CreateBody(world_id_, &body_defenition);
  entity_to_body_map_[entity] = body_id;
  body_to_entity_map_[body_id] = entity;

  return body_id;
}

auto PhysicsSystem::TestPoint(const Vector2 &position, const b2QueryFilter &filter) noexcept -> std::vector<Entity*>
{
  std::vector<Entity*> entities;
  void *context = new std::pair<Game&, std::vector<Entity*>&>{game_, entities};
  b2World_OverlapAABB(world_id_,
                      b2AABB{b2Vec2{position.x() - (kPointTestSize / 2.0f), position.y() - (kPointTestSize / 2.0f)}, b2Vec2{position.x() + (kPointTestSize / 2.0f), position.y() + (kPointTestSize / 2.0f)}},
                      filter,
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

auto PhysicsSystem::Raycast(const Vector2 &origin, const Vector2 &translation, const b2QueryFilter &filter) noexcept -> RaycastHit
{
  RaycastHit hit;
  std::pair<Game&, RaycastHit&> context = std::pair<Game&, RaycastHit&>{game_, hit};
  b2World_CastRay(world_id_,
                  Tob2Vec2(origin),
                  Tob2Vec2(translation),
                  filter,
                  [](b2ShapeId shape_id, b2Vec2 point, b2Vec2 normal, float fraction, void *context) -> float
                  {
                    const std::pair<Game&, RaycastHit&> &data = *reinterpret_cast<std::pair<Game&, RaycastHit&>*>(context);
                    data.second = RaycastHit{data.first.GetPhysicsSystem().GetEntity(b2Shape_GetBody(shape_id)), ToVector2(point), ToVector2(normal)};
                    return 0.0f;
                  },
                  &context);
  return hit;
}
} // game