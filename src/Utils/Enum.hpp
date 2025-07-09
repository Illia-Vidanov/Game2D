#ifndef GAME_ENUM_HPP
#define GAME_ENUM_HPP

#include "Setup.hpp"

#include <type_traits>
#include <stdint.h>

#include "Utils/Type.hpp"


namespace tolik
{
namespace detail
{
template<OperatorType Operator, typename T, typename U>
constexpr bool kCanDefineEnumOperator = ((std::is_enum_v<T> && std::is_arithmetic_v<U>) || (std::is_enum_v<U> && std::is_arithmetic_v<T>)) && !kOperatorDefined<Operator, T, U>;

template<typename T, bool = std::is_enum_v<T>>
struct SoftUnderlyingImpl
{
  using type = T;
};

template<typename T>
struct SoftUnderlyingImpl<T, true>
{
  using type = std::underlying_type_t<T>;
};
} // detail

template<typename T>
struct SoftUnderlying : detail::SoftUnderlyingImpl<T>
{};

template<typename T>
using SoftUnderlyingT = typename SoftUnderlying<T>::type;

template<typename T>
constexpr std::underlying_type_t<T> ToUnderlying(T value)
{ return static_cast<std::underlying_type_t<T>>(value); }

template<typename T>
constexpr auto SoftToUnderlying(T value) noexcept -> SoftUnderlyingT<T>
{ return static_cast<SoftUnderlyingT<T>>(value); }


template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<OperatorType::kBitwiseOr, T, U>, bool> = true>
constexpr auto operator|(T value1, U value2)  noexcept -> T 
{ return static_cast<T>(SoftToUnderlying(value1) | SoftToUnderlying(value2)); }

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<OperatorType::kBitwiseAnd, T, U>, bool> = true>
constexpr auto operator&(T value1, U value2)  noexcept -> T 
{ return static_cast<T>(SoftToUnderlying(value1) & SoftToUnderlying(value2)); }

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<OperatorType::kBitwiseXor, T, U>, bool> = true>
constexpr auto operator^(T value1, U value2)  noexcept -> T 
{ return static_cast<T>(SoftToUnderlying(value1) ^ SoftToUnderlying(value2)); }

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<OperatorType::kAddition, T, U>, bool> = true>
constexpr auto operator+(T value1, U value2)  noexcept -> T 
{ return static_cast<T>(SoftToUnderlying(value1) + SoftToUnderlying(value2)); }

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<OperatorType::kSubtraction, T, U>, bool> = true>
constexpr auto operator-(T value1, U value2)  noexcept -> T 
{ return static_cast<T>(SoftToUnderlying(value1) ^ SoftToUnderlying(value2)); }

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<OperatorType::kMultiplication, T, U>, bool> = true>
constexpr auto operator*(T value1, U value2)  noexcept -> T 
{ return static_cast<T>(SoftToUnderlying(value1) * SoftToUnderlying(value2)); }

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<OperatorType::kDivision, T, U>, bool> = true>
constexpr auto operator/(T value1, U value2)  noexcept -> T 
{ return static_cast<T>(SoftToUnderlying(value1) / SoftToUnderlying(value2)); }


template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<OperatorType::kEqual, T, U>, bool> = true>
constexpr auto operator==(T value1, U value2)  noexcept -> bool 
{ return SoftToUnderlying(value1) == SoftToUnderlying(value2); }

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<OperatorType::kNotEqual, T, U>, bool> = true>
constexpr auto operator!=(T value1, U value2)  noexcept -> bool 
{ return SoftToUnderlying(value1) != SoftToUnderlying(value2); }

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<OperatorType::kLessThen, T, U>, bool> = true>
constexpr auto operator<(T value1, U value2)  noexcept -> bool 
{ return SoftToUnderlying(value1) < SoftToUnderlying(value2); }

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<OperatorType::kMoreThen, T, U>, bool> = true>
constexpr auto operator>(T value1, U value2)  noexcept -> bool 
{ return SoftToUnderlying(value1) > SoftToUnderlying(value2); }


template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<OperatorType::kBitwiseOrAssign, T, U>, bool> = true>
constexpr auto operator|=(T &value1, U value2) noexcept -> T &
{ value1 = static_cast<T>(SoftToUnderlying(value1) | SoftToUnderlying(value2)); return value1; }

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<OperatorType::kBitwiseAndAssign, T, U>, bool> = true>
constexpr auto operator&=(T &value1, U value2) noexcept -> T &
{ value1 = static_cast<T>(SoftToUnderlying(value1) & SoftToUnderlying(value2)); return value1; }

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<OperatorType::kBitwiseXorAssign, T, U>, bool> = true>
constexpr auto operator^=(T &value1, U value2) noexcept -> T &
{ value1 = static_cast<T>(SoftToUnderlying(value1) ^ SoftToUnderlying(value2)); return value1; }

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<OperatorType::kSubtractAssign, T, U>, bool> = true>
constexpr auto operator-=(T &value1, U value2) noexcept -> T &
{ value1 = static_cast<T>(SoftToUnderlying(value1) - SoftToUnderlying(value2)); return value1; }

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<OperatorType::kAddAssign, T, U>, bool> = true>
constexpr auto operator+=(T &value1, U value2) noexcept -> T &
{ value1 = static_cast<T>(SoftToUnderlying(value1) + SoftToUnderlying(value2)); return value1; }

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<OperatorType::kDivideAssign, T, U>, bool> = true>
constexpr auto operator/=(T &value1, U value2) noexcept -> T &
{ value1 = static_cast<T>(SoftToUnderlying(value1) / SoftToUnderlying(value2)); return value1; }

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<OperatorType::kMultiplyAssign, T, U>, bool> = true>
constexpr auto operator*=(T &value1, U value2) noexcept -> T &
{ value1 = static_cast<T>(SoftToUnderlying(value1) * SoftToUnderlying(value2)); return value1; }


template<typename T, typename std::enable_if_t<std::is_enum_v<T> && !kOperatorDefined<OperatorType::kBitwiseLeftShift, std::ostream, T>, bool> = true>
std::ostream& operator<<(std::ostream& os, T self)
{ return os << ToUnderlying(self); }
} // tolik

#endif // FILE_ENUM_HPP