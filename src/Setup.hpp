#ifndef GAME_SETUP_HPP
#define GAME_SETUP_HPP

#include <cstdint>


/// Used to show who owns the memory
template<typename T>
using Owner = T;

/// Used to show that pointer can't be nullptr
template<typename T>
using NotNull = T;

/// Wraper around __builtin_expect
#define GAME_IS_LIKELY(condition) __builtin_expect(!!(condition), 1)
/// Wraper around __builtin_expect
#define GAME_IS_UNLIKELY(condition) __builtin_expect(!!(condition), 0)

/// Converts expression in prentecies to string
#define GAME_TO_STRING(x) #x

#endif // GAME_SETUP_HPP