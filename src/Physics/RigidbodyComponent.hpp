#ifndef GAME_RIGIDBODY_COMPONENT_HPP
#define GAME_RIGIDBODY_COMPONENT_HPP

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Physics/TransformComponent.hpp"
#include "Core/Game.hpp"
#include "Core/Entity.hpp"


namespace game
{
class Entity;

class RigidbodyComponent
{
public:
  RigidbodyComponent(Entity &entity) noexcept;
  void Updateb2Transform() const noexcept { b2Body_SetTransform(entity_.GetGame().GetPhysicsSystem().Getb2BodyId(entity_.GetID()), entity_.GetComponent<TransformComponent>().Getb2Position(), entity_.GetComponent<TransformComponent>().Getb2Rotation()); }

  [[nodiscard]] auto GetVelocity() const noexcept -> Vector2 { return ToNormalVector2(b2Body_GetLinearVelocity(body_id_)); }
  void SetVelocity(const Vector2 &velocity) const { b2Body_SetLinearVelocity(body_id_, Tob2Vec2(velocity)); }
  void AddForce(const Vector2 &force) const { b2Body_ApplyForceToCenter(body_id_, Tob2Vec2(force), true); }
  void SetGravityScale(DefaultFloatType gravity_scale) const { b2Body_SetGravityScale(body_id_, gravity_scale); }

private:
  Entity &entity_;

  b2BodyId body_id_;
};
} // game

#endif // GAME_RIGIDBODY_COMPONENT_HPP