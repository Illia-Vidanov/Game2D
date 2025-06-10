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
using namespace std::chrono_literals;
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
#include <entt/entt.hpp>

// STB Image - image reading
// Implementation is defined in Utils/FileReader.cpp
#include <stb_image/stb_image.h>

// Box2D - 2d physics library
#include <box2d/box2d.h>

// ImGUI
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

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

#if defined(NDEBUG) && !defined(TRACY_ENABLE)
  #define GAME_ASSERT_STD_MESSAGE(condition, message)
  #define GAME_ASSERT_STD_NO_MESSAGE(condition)
  #define GAME_ASSERT_STD_CHOOSER(x, A, B, FUNC, ...)
  #define GAME_ASSERT_STD(condition, message)
  #define GAME_DEBUG_LINE(command)
#else
  #define GAME_ASSERT_STD_MESSAGE(condition, message) assert(((void)(message), static_cast<bool>(condition)))
  #define GAME_ASSERT_STD_NO_MESSAGE(condition) assert(static_cast<bool>(condition))
  #define GAME_ASSERT_STD_CHOOSER(x, A, B, FUNC, ...) FUNC
  // Assert based on std::assert
  #define GAME_ASSERT_STD(...) GAME_ASSERT_STD_CHOOSER(, ##__VA_ARGS__, GAME_ASSERT_STD_MESSAGE, GAME_ASSERT_STD_NO_MESSAGE, sstatic_assert(false, "Wrong number of arguments"))(__VA_ARGS__)
  // Remove line on build
  #define GAME_DEBUG_LINE(command) do { command; } while(0)
#endif


#endif // GAME_SETUP_HPP