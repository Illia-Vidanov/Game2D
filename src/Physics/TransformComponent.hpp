#ifndef GAME_TRANSFORM_COMPONENT_HPP
#define GAME_TRANSFORM_COMPONENT_HPP

#include "Setup.hpp"

#include "Utils/MathConstants.hpp"

namespace game
{
class TransformComponent : public Eigen::Transform<DefFloatType, 2, 2>
{
public:
  inline TransformComponent() noexcept : Eigen::Transform<DefFloatType, 2, 2>{Eigen::Transform<DefFloatType, 2, 2>::Identity()} {}
  inline TransformComponent(const Eigen::Transform<DefFloatType, 2, 2> &other) noexcept : Eigen::Transform<DefFloatType, 2, 2>{other} {}

  [[nodiscard]] static auto OrthographicProjection(float left, float right, float bottom, float top, float near, float far) noexcept -> Matrix4;
  [[nodiscard]] static auto OrthographicProjection2D(float left, float right, float bottom, float top) noexcept -> Matrix3;
  [[nodiscard]] static auto To4x4TransformationMatrix(const TransformComponent &transform) noexcept -> Matrix4;
};
} // game

#endif // GAME_TRANSFORM_COMPONENT_HPP