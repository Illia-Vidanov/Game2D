#include "TransformComponent.hpp"

#include "Setup.hpp"

#include "Core/Game.hpp"
#include "Physics/RigidbodyComponent.hpp"
#include "Physics/ColliderComponents.hpp"


namespace game
{
TransformComponent &TransformComponent::operator=(const TransformComponent &other) noexcept
{
  ZoneScopedC(0xff65a8);

  angle_ = other.angle_;
  sin_ = other.sin_;
  cos_ = other.cos_;
  scale_ = other.scale_;

  return *this;
}

TransformComponent &TransformComponent::operator=(const Transform &other) noexcept
{
  ZoneScopedC(0xff65a8);

  Transform::operator=(other);
  const Vector2 &sin_and_cos = linear().col(0).normalized();
  sin_ = sin_and_cos(1);
  cos_ = sin_and_cos(0);
  scale_ = Vector2{linear()(1, 1) / sin_, linear()(0, 0) / cos_};
  angle_ = std::atan2(sin_, cos_);

  return *this;
}

// near_ and far_ are written with underline because some windows library defines near and far
auto TransformComponent::OrthographicProjection(float left, float right, float bottom, float top, float near_, float far_) noexcept -> Matrix4
{
  ZoneScopedC(0xff65a8);

  Matrix4 projection = Matrix4::Identity();
  
  projection(0, 0) = 2.0f / (right - left);
  projection(1, 1) = 2.0f / (top - bottom);
  projection(2, 2) = -2.0f / (far_ - near_);
  
  projection(0, 3) = -(right + left) / (right - left);
  projection(1, 3) = -(top + bottom) / (top - bottom);
  projection(2, 3) = -(far_ + near_) / (far_ - near_);

  return projection;
}

auto TransformComponent::OrthographicProjection2D(float left, float right, float bottom, float top) noexcept -> Matrix3
{
  ZoneScopedC(0xff65a8);

  Matrix3 projection = Matrix3::Identity();
  
  projection(0, 0) = 2.0f / (right - left);
  projection(1, 1) = 2.0f / (top - bottom);
  
  projection(0, 2) = -(right + left) / (right - left);
  projection(1, 2) = -(top + bottom) / (top - bottom);

  return projection;
}

auto TransformComponent::To4x4TransformationMatrix(const Transform &transform) noexcept -> Matrix4
{
  ZoneScopedC(0xff65a8);

  Matrix4 result = Matrix4::Identity();
  result.block<2, 2>(0, 0) = transform.linear();
  result.block<2, 1>(0, 3) = transform.translation();

  return result;
}
}