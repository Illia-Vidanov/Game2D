#ifndef GAME_CAMERA_COMPONENT_HPP
#define GAME_CAMERA_COMPONENT_HPP

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Physics/TransformComponent.hpp"
#include "Core/Entity.hpp"


namespace game
{
class CameraComponent
{
public:
  CameraComponent(Entity *entity) noexcept : entity_{entity} {}

  [[nodiscard]] auto GetViewMatrix() const noexcept -> const Matrix3& { return entity_->GetComponent<TransformComponent>().matrix(); }

  [[nodiscard]] constexpr auto GetEntity() const noexcept -> const Entity * { return entity_; }
  [[nodiscard]] constexpr auto GetEntity() noexcept -> Entity * { return entity_; }
  
private:
  Entity *entity_;
};
} // game

#endif // GAME_CAMERA_COMPONENT_HPP