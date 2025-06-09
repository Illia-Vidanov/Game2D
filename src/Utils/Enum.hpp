#ifndef GAME_ENUM_HPP
#define GAME_ENUM_HPP

#include "Setup.hpp"


namespace game
{
namespace detail
{
template<typename T, typename U>
constexpr bool kCanDefineEnumOperator = (std::is_enum_v<T> && std::is_arithmetic_v<U>) || (std::is_enum_v<U> && std::is_arithmetic_v<T>);
} // detail

template<typename T, std::enable_if_t<!(std::is_enum_v<T> || std::is_arithmetic_v<T>), bool> = true>
constexpr T ToUnderlying(T value)
{ static_assert(false, "Can't cast to underlying type non enum"); }

template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
constexpr T ToUnderlying(T value)
{ return value; }

template<typename T>
constexpr std::underlying_type_t<T> ToUnderlying(T value)
{ return static_cast<std::underlying_type_t<T>>(value); }


// TODO: SFINAE if defenition of operator between T and U exists

// We need here std::enable_if_t in order to not overload existing operators
// Value can also be arithmetic if we want to apply it between value and enum

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<T, U>, bool> = true>
constexpr T operator|(T value1, U value2) 
{ return static_cast<T>(ToUnderlying(value1) | ToUnderlying(value2)); }

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<T, U>, bool> = true>
constexpr T operator&(T value1, U value2) 
{ return static_cast<T>(ToUnderlying(value1) & ToUnderlying(value2)); }

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<T, U>, bool> = true>
constexpr T operator^(T value1, U value2) 
{ return static_cast<T>(ToUnderlying(value1) ^ ToUnderlying(value2)); }


template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<T, U>, bool> = true>
constexpr bool operator==(T value1, U value2) 
{ return ToUnderlying(value1) == ToUnderlying(value2); }

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<T, U>, bool> = true>
constexpr bool operator!=(T value1, U value2) 
{ return ToUnderlying(value1) != ToUnderlying(value2); }

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<T, U>, bool> = true>
constexpr bool operator<(T value1, U value2) 
{ return ToUnderlying(value1) < ToUnderlying(value2); }

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<T, U>, bool> = true>
constexpr bool operator>(T value1, U value2) 
{ return ToUnderlying(value1) > ToUnderlying(value2); }


template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<T, U>, bool> = true>
constexpr T &operator|=(T &value1, U value2)
{ value1 = static_cast<T>(ToUnderlying(value1) | ToUnderlying(value2)); return value1; }

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<T, U>, bool> = true>
constexpr T &operator&=(T &value1, U value2)
{ value1 = static_cast<T>(ToUnderlying(value1) & ToUnderlying(value2)); return value1; }

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<T, U>, bool> = true>
constexpr T &operator^=(T &value1, U value2)
{ value1 = static_cast<T>(ToUnderlying(value1) ^ ToUnderlying(value2)); return value1; }


template<typename T, typename std::enable_if_t<std::is_enum_v<T>, bool> = true>
std::ostream& operator<<(std::ostream& os, T self)
{ return os << ToUnderlying(self); }
} // game

#endif // GAME_ENUM_HPP