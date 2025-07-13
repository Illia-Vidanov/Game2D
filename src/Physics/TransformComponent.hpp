#ifndef GAME_TRANSFORM_COMPONENT_HPP
#define GAME_TRANSFORM_COMPONENT_HPP

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Core/ComponentBase.hpp"
#include "Utils/MathConstants.hpp"
#include "Utils/Math.hpp"


namespace game
{
class Entity;

class TransformComponent : public Eigen::Transform<DefaultFloatType, 2, 2>, public ComponentBase
{
public:
  TransformComponent(Entity *entity) noexcept : Eigen::Transform<DefaultFloatType, 2, 2>{Eigen::Transform<DefaultFloatType, 2, 2>::Identity()}, ComponentBase{entity} {}
  ~TransformComponent() noexcept = default;
  TransformComponent &operator=(const TransformComponent &) noexcept = default;
  TransformComponent(TransformComponent &&) noexcept = default;
  TransformComponent &operator=(TransformComponent &&) noexcept = default;
  
  TransformComponent &operator=(const Transform &other) noexcept;
  TransformComponent &operator=(Transform &&other) noexcept;

  [[nodiscard]] auto GetPosition() const noexcept -> Vector2 { return translation(); }
  void SetPosition(const Vector2 &position) noexcept { translation() = position; }
  [[nodiscard]] constexpr auto GetRotationDegrees() const noexcept -> DefaultFloatType { return RadiansToDegrees(angle_radians_); }
  [[nodiscard]] constexpr auto GetRotationRadians() const noexcept -> DefaultFloatType { return angle_radians_; }
  void SetRotationDegrees(DefaultFloatType angle_degrees) noexcept { angle_radians_ = DegreesToRadians(angle_degrees); sin_ = std::sin(angle_radians_); cos_ = std::cos(angle_radians_); linear() = Matrix2{ {cos_, -sin_}, {sin_, cos_} } * Eigen::Scaling(scale_); }
  void SetRotationRadians(DefaultFloatType angle_radians) noexcept { angle_radians_ = angle_radians; sin_ = std::sin(angle_radians_); cos_ = std::cos(angle_radians_); linear() = Matrix2{ {cos_, -sin_}, {sin_, cos_} } * Eigen::Scaling(scale_); }

  [[nodiscard]] auto GetSinAndCos() const noexcept -> Vector2 { return Vector2{sin_, cos_}; }
  void SetSinAndCos(const Vector2 &sin_and_cos) noexcept { sin_ = sin_and_cos.x(); cos_ = sin_and_cos.y(); angle_radians_ = std::atan2(sin_, cos_); linear() = Matrix2{ {cos_, -sin_}, {sin_, cos_} } * Eigen::Scaling(scale_); }
  [[nodiscard]] auto GetScale() const noexcept -> Vector2 { return scale_; }
  void SetScale(const Vector2 &scale) noexcept { scale_ = scale; linear() = Matrix2{ {cos_, -sin_}, {sin_, cos_} } * Eigen::Scaling(scale_); }

  [[nodiscard]] static auto OrthographicProjection(float left, float right, float bottom, float top, float near, float far) noexcept -> Matrix4;
  [[nodiscard]] static auto OrthographicProjection2D(float left, float right, float bottom, float top) noexcept -> Matrix3;
  [[nodiscard]] static auto To4x4TransformationMatrix(const Transform &transform) noexcept -> Matrix4;

  [[nodiscard]] auto Getb2Position() const noexcept -> b2Vec2 { return b2Vec2{translation().x(), translation().y()}; }
  [[nodiscard]] auto Getb2Rotation() const noexcept -> b2Rot { return b2Rot{cos_, sin_}; }

private:
  DefaultFloatType angle_radians_ = 0;
  DefaultFloatType sin_ = 0;
  DefaultFloatType cos_ = 1;
  Vector2 scale_ = Vector2::Ones();
};
} // game

#endif // GAME_TRANSFORM_COMPONENT_HPP