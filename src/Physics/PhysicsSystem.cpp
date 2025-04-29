#include "PhysicsSystem.hpp"

#include "Setup.hpp"

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

  for(auto container_a = square_colliders_.begin(); container_a != square_colliders_.end(); ++container_a)
  {
    ZoneScopedNC("Check square collider: " + std::to_string(*container_a), 0xfc940a);
    
    for(auto container_b = container_a + 1; container_b != square_colliders_.end(); ++container_b)
    {
      Vector4 rect_a = square_colliders_.get<SquareColliderComponent>(*container_a).GetRect();
      TransformComponent &transform_a = square_colliders_.get<TransformComponent>(*container_a);
      rect_a.z() *= transform_a.linear().diagonal().x();
      rect_a.w() *= transform_a.linear().diagonal().y();
      rect_a.block<2, 1>(0, 0) += transform_a.translation();

      Vector4 rect_b = square_colliders_.get<SquareColliderComponent>(*container_b).GetRect();
      TransformComponent &transform_b = square_colliders_.get<TransformComponent>(*container_b);
      rect_b.z() *= transform_b.linear().diagonal().x();
      rect_b.w() *= transform_b.linear().diagonal().y();
      rect_b.block<2, 1>(0, 0) += transform_b.translation();

      GAME_DLOG(LogType::kInfo) << rect_a.transpose() << "    " << rect_b.transpose();
      GAME_DLOG_IF(rect_a.x() + rect_a.z() > rect_b.x() &&
                   rect_b.x() + rect_b.z() > rect_a.x() &&
                   rect_a.y() + rect_a.w() > rect_b.y() &&
                   rect_b.y() + rect_b.w() > rect_a.y(), LogType::kInfo) << "Hit!";
    }


    //GAME_DLOG(LogType::kInfo) << ToWorldRect(square_collider.GetRect(), transform).transpose();
  }
}
} // game