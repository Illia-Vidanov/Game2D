#include "Math.hpp"

#include "Setup.hpp"

#include "Utils/MathConstants.hpp"
#include "Utils/Logger.hpp"
#include "Physics/TransformComponent.hpp"


namespace game
{
// near_ and far_ are written with underline because some windows library defines near and far

auto OrthographicProjection(float left, float right, float bottom, float top, float near_, float far_) noexcept -> Matrix4
{
  Matrix4 projection = Matrix4::Identity();
  
  projection(0, 0) = 2.0f / (right - left);
  projection(1, 1) = 2.0f / (top - bottom);
  projection(2, 2) = -2.0f / (far_ - near_);
  
  projection(0, 3) = -(right + left) / (right - left);
  projection(1, 3) = -(top + bottom) / (top - bottom);
  projection(2, 3) = -(far_ + near_) / (far_ - near_);

  return projection;
}

auto OrthographicProjection2D(float left, float right, float bottom, float top) noexcept -> Matrix3
{
  Matrix3 projection = Matrix3::Identity();
  
  projection(0, 0) = 2.0f / (right - left);
  projection(1, 1) = 2.0f / (top - bottom);
  
  projection(0, 2) = -(right + left) / (right - left);
  projection(1, 2) = -(top + bottom) / (top - bottom);

  return projection;
}

auto TransformTo4x4TransformationMatrix(const TransformComponent &transform) noexcept -> Matrix4
{
  Matrix4 result = Matrix4::Identity();
  result.block<2, 2>(0, 0) = transform.linear();
  result.block<2, 1>(0, 3) = transform.translation();
  GAME_DLOG(LogType::kInfo) << result.transpose();
  return result;
}
} // game