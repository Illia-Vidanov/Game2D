#ifndef GAME_TRANSFORM_COMPONENT_HPP
#define GAME_TRANSFORM_COMPONENT_HPP

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"


namespace game
{
class Entity;

class TransformComponent : public Eigen::Transform<DefaultFloatType, 2, 2>
{
public:
  TransformComponent(Entity &entity) noexcept : Eigen::Transform<DefaultFloatType, 2, 2>{Eigen::Transform<DefaultFloatType, 2, 2>::Identity()}, entity_{entity} {}
  TransformComponent &operator=(const Transform &other) noexcept;
  TransformComponent &operator=(Transform &&other) noexcept;
  TransformComponent &operator=(const TransformComponent &) noexcept = default;
  ~TransformComponent() noexcept = default;
  TransformComponent(TransformComponent &&) noexcept = default;
  TransformComponent &operator=(TransformComponent &&) noexcept = default;

  [[nodiscard]] auto GetPosition() noexcept -> Vector2 { return translation(); }
  void SetPosition(const Vector2 &position) noexcept { translation() = position; }
  [[nodiscard]] constexpr auto GetRotationAngle() const noexcept -> DefaultFloatType { return angle_; }
  void SetRotationAngle(DefaultFloatType angle) noexcept { angle_ = angle; sin_ = std::sin(angle_); cos_ = std::cos(angle_); linear() = Eigen::Scaling(scale_) * Matrix2{ {cos_, -sin_}, {sin_, cos_} }; }

  [[nodiscard]] auto GetSinAndCos() const noexcept -> Vector2 { return Vector2{sin_, cos_}; }
  void SetSinAndCos(const Vector2 &sin_and_cos) noexcept { sin_ = sin_and_cos.x(); cos_ = sin_and_cos.y(); angle_ = std::atan2(sin_, cos_); linear() = Eigen::Scaling(scale_) * Matrix2{ {cos_, -sin_}, {sin_, cos_} }; }
  [[nodiscard]] auto GetScale() const noexcept -> Vector2 { return scale_; }
  void SetScale(const Vector2 &scale) noexcept { scale_ = scale; linear() = Eigen::Scaling(scale_) * Matrix2{ {cos_, -sin_}, {sin_, cos_} }; }

  [[nodiscard]] static auto OrthographicProjection(float left, float right, float bottom, float top, float near, float far) noexcept -> Matrix4;
  [[nodiscard]] static auto OrthographicProjection2D(float left, float right, float bottom, float top) noexcept -> Matrix3;
  [[nodiscard]] static auto To4x4TransformationMatrix(const Transform &transform) noexcept -> Matrix4;

  [[nodiscard]] auto Getb2Position() const noexcept -> b2Vec2 { return b2Vec2{translation().x(), translation().y()}; }
  [[nodiscard]] auto Getb2Rotation() const noexcept -> b2Rot { return b2Rot{cos_, sin_}; }

private:
  Entity &entity_;

  DefaultFloatType angle_ = 0;
  DefaultFloatType sin_ = 0;
  DefaultFloatType cos_ = 1;
  Vector2 scale_ = Vector2::Ones();
};
} // game

#endif // GAME_TRANSFORM_COMPONENT_HPP