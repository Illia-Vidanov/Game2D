#ifndef FILE_MATH_CONSTANTS_HPP
#define FILE_MATH_CONSTANTS_HPP

#include "Setup.hpp"

#include <stdint.h>


namespace tolik
{
//using DefaultFloatType = float;

constexpr inline DefaultFloatType kPI = static_cast<DefaultFloatType>(3.141592);
constexpr inline DefaultFloatType kHalfPI = kPI / static_cast<DefaultFloatType>(2);
constexpr inline DefaultFloatType kElipson = static_cast<DefaultFloatType>(0.000001);
} // tolik

#endif // FILE_MATH_CONSTANTS_HPP