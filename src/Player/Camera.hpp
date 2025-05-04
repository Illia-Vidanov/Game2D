#ifndef GAME_CAMERA_HPP
#define GAME_CAMERA_HPP

#include "Setup.hpp"


namespace game
{
class Game;

class Camera
{
public:
  Camera(Game &game) noexcept;

  [[nodiscard]] inline auto GetViewMatrix() const noexcept -> const Matrix3& { return transform_.matrix(); }

  [[nodiscard]] inline auto GetPosition() noexcept -> Vector2 { return transform_.translation(); }
  void SetPosition(const Vector2 &position) noexcept { transform_.translation() = position; }
  inline void SetPositionWithoutRigidbodyUpdate(const Vector2 &position) noexcept { transform_.translation() = position; }
  [[nodiscard]] constexpr inline auto GetRotationAngle() const noexcept -> DefFloatType { return angle_; }
  inline void SetRotationAngle(DefFloatType angle) noexcept { angle_ = angle; sin_ = std::sin(angle_); cos_ = std::cos(angle_); transform_.linear() = Eigen::Scaling(scale_) * Matrix2{ {cos_, -sin_}, {sin_, cos_} }; }
  inline void SetRotationAngleWithoutRigidbodyUpdate(DefFloatType angle) noexcept { angle_ = angle; sin_ = std::sin(angle_); cos_ = std::cos(angle_); transform_.linear() = Eigen::Scaling(scale_) * Matrix2{ {cos_, -sin_}, {sin_, cos_} }; }
  [[nodiscard]] inline auto GetSinAndCos() const noexcept -> Vector2 { return Vector2{sin_, cos_}; }
  inline void SetSinAndCos(const Vector2 &sin_and_cos) noexcept { sin_ = sin_and_cos.x(); cos_ = sin_and_cos.y(); angle_ = std::atan2(sin_, cos_); transform_.linear() = Eigen::Scaling(scale_) * Matrix2{ {cos_, -sin_}, {sin_, cos_} }; }
  [[nodiscard]] inline auto GetScale() const noexcept -> Vector2 { return scale_; }
  inline void SetScale(const Vector2 &scale) noexcept { scale_ = scale; transform_.linear() = Eigen::Scaling(scale_) * Matrix2{ {cos_, -sin_}, {sin_, cos_} }; }


private:
  Game &game_;

  Transform transform_ = Transform::Identity();

  DefFloatType angle_ = 0;
  DefFloatType sin_ = 0;
  DefFloatType cos_ = 1;
  Vector2 scale_ = Vector2::Ones();
};
} // game

#endif // GAME_CAMERA_HPP