#ifndef GAME_TRANSFORM_COMPONENT_HPP
#define GAME_TRANSFORM_COMPONENT_HPP

#include "Setup.hpp"


namespace game
{
class Game;

class TransformComponent : public Eigen::Transform<DefFloatType, 2, 2>
{
public:
  inline TransformComponent(entt::entity self, Game &game) noexcept : Eigen::Transform<DefFloatType, 2, 2>{Eigen::Transform<DefFloatType, 2, 2>::Identity()}, self_{self}, game_{game} {}
  TransformComponent &operator=(const TransformComponent &other) noexcept;
  TransformComponent &operator=(const Transform &other) noexcept;

  [[nodiscard]] inline auto GetPosition() noexcept -> Vector2 { return translation(); }
  inline void SetPosition(const Vector2 &position) noexcept { translation() = position; }
  [[nodiscard]] constexpr inline auto GetRotationAngle() const noexcept -> DefFloatType { return angle_; }
  inline void SetRotationAngle(DefFloatType angle) noexcept { angle_ = angle; sin_ = std::sin(angle_); cos_ = std::cos(angle_); linear() = Eigen::Scaling(scale_) * Matrix2{ {cos_, -sin_}, {sin_, cos_} }; }

  [[nodiscard]] inline auto GetSinAndCos() const noexcept -> Vector2 { return Vector2{sin_, cos_}; }
  inline void SetSinAndCos(const Vector2 &sin_and_cos) noexcept { sin_ = sin_and_cos.x(); cos_ = sin_and_cos.y(); angle_ = std::atan2(sin_, cos_); linear() = Eigen::Scaling(scale_) * Matrix2{ {cos_, -sin_}, {sin_, cos_} }; }
  [[nodiscard]] inline auto GetScale() const noexcept -> Vector2 { return scale_; }
  inline void SetScale(const Vector2 &scale) noexcept { scale_ = scale; linear() = Eigen::Scaling(scale_) * Matrix2{ {cos_, -sin_}, {sin_, cos_} }; }

  [[nodiscard]] static auto OrthographicProjection(float left, float right, float bottom, float top, float near, float far) noexcept -> Matrix4;
  [[nodiscard]] static auto OrthographicProjection2D(float left, float right, float bottom, float top) noexcept -> Matrix3;
  [[nodiscard]] static auto To4x4TransformationMatrix(const Transform &transform) noexcept -> Matrix4;

  [[nodiscard]] inline auto Getb2Position() const noexcept -> b2Vec2 { return b2Vec2{translation().x(), translation().y()}; }
  [[nodiscard]] inline auto Getb2Rotation() const noexcept -> b2Rot { return b2Rot{cos_, sin_}; }

private:
  entt::entity self_;
  Game &game_;

  DefFloatType angle_ = 0;
  DefFloatType sin_ = 0;
  DefFloatType cos_ = 1;
  Vector2 scale_ = Vector2::Ones();
};
} // game

#endif // GAME_TRANSFORM_COMPONENT_HPP