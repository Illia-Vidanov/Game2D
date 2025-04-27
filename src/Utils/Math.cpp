#include "Math.hpp"

#include "Setup.hpp"

#include <type_traits>
#include <cmath>

#include <gcem/gcem.hpp>
#include <Eigen/Dense>

#include "Utils/MathConstants.hpp"


namespace game
{
auto OrthographicProjection(float left, float right, float bottom, float top, float near, float far) noexcept -> Matrix4
{
  Matrix4 projection = Matrix4::Identity();
  
  projection(0, 0) = 2.0f / (right - left);
  projection(1, 1) = 2.0f / (top - bottom);
  projection(2, 2) = -2.0f / (far - near);
  
  projection(0, 3) = -(right + left) / (right - left);
  projection(1, 3) = -(top + bottom) / (top - bottom);
  projection(2, 3) = -(far + near) / (far - near);

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

auto TransformTo4x4TransformationMatrix(const Transform &transform) noexcept -> Matrix4
{
  Matrix4 result = Matrix4::Identity();
  result.block<2, 2>(0, 0) = transform.linear();
  result.block<2, 1>(0, 3) = transform.translation();

  return result;
}
} // game