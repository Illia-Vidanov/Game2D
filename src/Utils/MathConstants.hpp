#ifndef GAME_MATH_CONSTANTS_HPP
#define GAME_MATH_CONSTANTS_HPP

#include "Setup.hpp"


namespace game
{
using DefaultFloatType = float;
using Transform = Eigen::Transform<DefaultFloatType, 2, 2>;
using Vector2 = Eigen::Vector2<DefaultFloatType>;
using Vector3 = Eigen::Vector3<DefaultFloatType>;
using Vector4 = Eigen::Vector4<DefaultFloatType>;
using Vector2i = Eigen::Vector2<int>;
using Vector3i = Eigen::Vector3<int>;
using Vector4i = Eigen::Vector4<int>;
using Matrix2 = Eigen::Matrix2<DefaultFloatType>;
using Matrix3 = Eigen::Matrix3<DefaultFloatType>;
using Matrix4 = Eigen::Matrix4<DefaultFloatType>;

constexpr inline DefaultFloatType kPI = static_cast<DefaultFloatType>(3.141592);
constexpr inline DefaultFloatType kHalfPI = kPI / static_cast<DefaultFloatType>(2);
constexpr inline DefaultFloatType kElipson = static_cast<DefaultFloatType>(0.000001);
} // game

#endif // GAME_MATH_CONSTANTS_HPP