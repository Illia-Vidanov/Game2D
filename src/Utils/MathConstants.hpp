#ifndef GAME_MATH_CONSTANTS_HPP
#define GAME_MATH_CONSTANTS_HPP

#include "Setup.hpp"

#include <Eigen/Dense>
#include <Eigen/Geometry>

namespace game
{
using DefFloatType = float;
using Transform = Eigen::Transform<DefFloatType, 3, 2>;

constexpr inline DefFloatType kPI = static_cast<DefFloatType>(3.141592);
constexpr inline DefFloatType kHalfPI = kPI / static_cast<DefFloatType>(2);
constexpr inline DefFloatType kElipson = static_cast<DefFloatType>(0.000001);
} // Tolik

#endif // GAME_MATH_CONSTANTS_HPP