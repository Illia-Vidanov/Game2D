#ifndef GAME_VECTOR_MATH_HPP
#define GAME_VECTOR_MATH_HPP

#include "Setup.hpp"

#include "Utils/Math.hpp"


namespace game
{
[[nodiscard]] inline auto Tob2Vec2(const Vector2 &vector) noexcept -> b2Vec2 { return b2Vec2{vector.x(), vector.y()}; }
[[nodiscard]] inline auto Tob2Rot(DefaultFloatType angle) noexcept -> b2Rot { return b2Rot{std::cos(angle), std::sin(angle)}; }
[[nodiscard]] inline auto Tob2Rot(const Vector2 &sin_and_cos) noexcept -> b2Rot { return b2Rot{sin_and_cos.y(), sin_and_cos.x()}; }
[[nodiscard]] inline auto ToVector2(const b2Vec2 &vector) noexcept -> Vector2 { return Vector2{vector.x, vector.y}; }
[[nodiscard]] inline auto ToRotationAngle(const b2Rot &rotation) noexcept -> DefaultFloatType { return std::atan2(rotation.s, rotation.c); }
[[nodiscard]] inline auto VectorLerp(const Vector2 &from, const Vector2 &to, DefaultFloatType t) noexcept -> Vector2 { return Vector2{Lerp(from.x(), to.x(), t), Lerp(from.x(), to.x(), t)}; }
} // game

#endif // GAME_VECTOR_MATH_HPP