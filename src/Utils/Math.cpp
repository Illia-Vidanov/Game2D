#include "Math.hpp"

#include "Setup.hpp"

#include <type_traits>
#include <cmath>

#include <gcem/gcem.hpp>
#include <Eigen/Dense>

#include "Utils/MathConstants.hpp"


namespace game
{
auto OrthographicProjection(float left, float right, float bottom, float top, float near, float far) noexcept -> Eigen::Matrix4<DefFloatType>
{
  Eigen::Matrix4<DefFloatType> projection = Eigen::Matrix4<DefFloatType>::Identity();
  
  projection(0, 0) = 2.0f / (right - left);
  projection(1, 1) = 2.0f / (top - bottom);
  projection(2, 2) = -2.0f / (far - near);
  
  projection(0, 3) = -(right + left) / (right - left);
  projection(1, 3) = -(top + bottom) / (top - bottom);
  projection(2, 3) = -(far + near) / (far - near);

  return projection;
}

auto OrthographicProjection2D(float left, float right, float bottom, float top) noexcept -> Eigen::Matrix3<DefFloatType>
{
  Eigen::Matrix3<DefFloatType> projection = Eigen::Matrix3<DefFloatType>::Identity();
  
  projection(0, 0) = 2.0f / (right - left);
  projection(1, 1) = 2.0f / (top - bottom);
  
  projection(0, 2) = -(right + left) / (right - left);
  projection(1, 2) = -(top + bottom) / (top - bottom);

  return projection;
}
} // game