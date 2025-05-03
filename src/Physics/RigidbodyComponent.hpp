#ifndef GAME_RIGIDBODY_COMPONENT_HPP
#define GAME_RIGIDBODY_COMPONENT_HPP

#include "Setup.hpp"

#include "Physics/TransformComponent.hpp"
#include "Core/Game.hpp"


namespace game
{
class RigidbodyComponent
{
public:
  RigidbodyComponent(entt::entity self, Game &game) noexcept;
  inline void Updateb2Transform() const noexcept { b2Body_SetTransform(game_.GetPhysicsSystem().Getb2BodyId(self_), game_.GetRegistry().get<TransformComponent>(self_).Getb2Position(), game_.GetRegistry().get<TransformComponent>(self_).Getb2Rotation()); }

  [[nodiscard]] inline auto GetVelocity() const noexcept -> Vector2 { return ToNormalVector2(b2Body_GetLinearVelocity(body_id_)); }
  inline void SetVelocity(const Vector2 &velocity) const { b2Body_SetLinearVelocity(body_id_, Tob2Vec2(velocity)); }

private:
  entt::entity self_;
  Game &game_;

  b2BodyId body_id_;
};
} // game

#endif // GAME_RIGIDBODY_COMPONENT_HPP