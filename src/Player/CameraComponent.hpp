#ifndef GAME_CAMERA_COMPONENT_HPP
#define GAME_CAMERA_COMPONENT_HPP

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Physics/TransformComponent.hpp"
#include "Core/ComponentBase.hpp"
#include "Core/Entity.hpp"


namespace game
{
class CameraComponent : public ComponentBase
{
public:
  CameraComponent(Entity *entity) noexcept : ComponentBase{entity} {}

  [[nodiscard]] auto GetViewMatrix() const noexcept -> const Matrix3& { return entity_->GetComponent<TransformComponent>().matrix(); }
};
} // game

#endif // GAME_CAMERA_COMPONENT_HPP