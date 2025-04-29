#include "PhysicsSystem.hpp"

#include "Setup.hpp"

#include <vector>
#include <utility>

#include <entt.hpp>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <tracy/Tracy.hpp>

#include "Physics/ColliderComponents.hpp"
#include "Physics/TransformComponent.hpp"
#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Core/Game.hpp"


namespace game
{
PhysicsSystem::PhysicsSystem(Game &game)
  : square_colliders_{game.GetRegistry().group<const SquareColliderComponent>(entt::get<TransformComponent>)}
{
  ZoneScopedC(0xfc940a);
}

void PhysicsSystem::Update() noexcept
{
  ZoneScopedC(0xfc940a);

  for(entt::entity entity : square_colliders_)
  {
    ZoneScopedNC("Check square collider", 0xfc940a);

    const SquareColliderComponent &square_collider = square_colliders_.get<SquareColliderComponent>(entity);
    TransformComponent &transform = square_colliders_.get<TransformComponent>(entity);

    //GAME_DLOG(LogType::kInfo) << ToWorldRect(square_collider.GetRect(), transform).transpose();
  }
}
} // game