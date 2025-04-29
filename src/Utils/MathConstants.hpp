#ifndef GAME_MATH_CONSTANTS_HPP
#define GAME_MATH_CONSTANTS_HPP

#include "Setup.hpp"

#include <Eigen/Dense>
#include <Eigen/Geometry>

namespace game
{
using DefFloatType = float;
using Vector2 = Eigen::Vector2<DefFloatType>;
using Vector3 = Eigen::Vector3<DefFloatType>;
using Vector4 = Eigen::Vector4<DefFloatType>;
using Vector2i = Eigen::Vector2<int>;
using Vector3i = Eigen::Vector3<int>;
using Vector4i = Eigen::Vector4<int>;
using Matrix3 = Eigen::Matrix3<DefFloatType>;
using Matrix4 = Eigen::Matrix4<DefFloatType>;

constexpr inline DefFloatType kPI = static_cast<DefFloatType>(3.141592);
constexpr inline DefFloatType kHalfPI = kPI / static_cast<DefFloatType>(2);
constexpr inline DefFloatType kElipson = static_cast<DefFloatType>(0.000001);
} // Tolik

#endif // GAME_MATH_CONSTANTS_HPP