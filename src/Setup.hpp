#ifndef GAME_SETUP_HPP
#define GAME_SETUP_HPP

// std c++17
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <type_traits>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <array>
#include <unordered_map>
#include <map>
#include <typeindex>
#include <cstring>
#include <sstream>
#include <fstream>
#include <limits>
#include <functional>
#include <memory>
#include <queue>
#include <set>
#include <unordered_set>
#include <cassert>
#include <initializer_list>
#include <locale>
#include <bitset>
#include <filesystem>

// std windows
#include <windows.h>

// Eigen - math
#include <Eigen/Dense>
#include <Eigen/Geometry>

// gcem - constexpr math
#include <gcem/gcem.hpp>

// SDL2 - window
#include <SDL2/SDL.h>

// glad - opengl support
#include <glad/glad.h>

// Tracy - profiling
#include <tracy/Tracy.hpp>

// EnTT - ECS
#include <entt.hpp>

// STB Image - image reading
#include <stb_image.h>

// Box2D - 2d physics library
#include <box2d/box2d.h>

// Used to show who owns the memory
template<typename T>
using Owner = T;

// Used to show that pointer can't be nullptr
template<typename T>
using NotNull = T;

// Wraper around __builtin_expect
#define GAME_IS_LIKELY(condition) __builtin_expect(!!(condition), 1)
// Wraper around __builtin_expect
#define GAME_IS_UNLIKELY(condition) __builtin_expect(!!(condition), 0)

// Converts expression in prentecies to string
#define GAME_TO_STRING(x) #x

#endif // GAME_SETUP_HPP