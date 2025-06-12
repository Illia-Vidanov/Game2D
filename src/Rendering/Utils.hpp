#ifndef GAME_UTILS_GL_HPP
#define GAME_UTILS_GL_HPP

#include "Setup.hpp"
#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"


#ifndef NDEBUG
#define GAME_GL_CALL(call) do { \
    call; \
    while(int error = glGetError()) \
      GAME_VLOG(1, LogType::kError) << __LINE__ << ' ' << __FILE__ << " OpenGL error: " << error; \
  } while(0)
#else
#define GAME_GL_CALL(call) call
#endif

namespace game
{
template<typename Type>
constexpr uint32_t GetGLTypeFromType() { static_assert(false, "Unsupported type"); return 0; }

template<> constexpr uint32_t GetGLTypeFromType<int32_t>()  { return GL_INT; }
template<> constexpr uint32_t GetGLTypeFromType<uint32_t>() { return GL_UNSIGNED_INT; }
template<> constexpr uint32_t GetGLTypeFromType<int16_t>()  { return GL_SHORT; }
template<> constexpr uint32_t GetGLTypeFromType<uint16_t>() { return GL_UNSIGNED_SHORT; }
template<> constexpr uint32_t GetGLTypeFromType<int8_t>()   { return GL_BYTE; }
template<> constexpr uint32_t GetGLTypeFromType<uint8_t>()  { return GL_UNSIGNED_BYTE; }

constexpr std::size_t GetSizeFromGLType(uint32_t type) noexcept
{
  switch(type)
  {
  case GL_DOUBLE:
    return 8;
  case GL_FLOAT:
  case GL_UNSIGNED_INT:
  case GL_INT:
    return 4;
  case GL_SHORT:
  case GL_UNSIGNED_SHORT:
    return 2;
  case GL_BYTE:
  case GL_UNSIGNED_BYTE:
    return 1;
  default:
    GAME_ASSERT(false) << "No GLType with index: " << type;
    return 4;
  }
}

template<typename Type>
constexpr uint32_t TypeToGLType() noexcept { GAME_ASSERT(false) << "No GLType fot type " << typeid(Type).name(); return 0; }
template<> constexpr uint32_t TypeToGLType<int32_t>() noexcept { return GL_INT; }
template<> constexpr uint32_t TypeToGLType<uint32_t>() noexcept { return GL_UNSIGNED_INT; }
template<> constexpr uint32_t TypeToGLType<int16_t>() noexcept { return GL_SHORT; }
template<> constexpr uint32_t TypeToGLType<uint16_t>() noexcept { return GL_UNSIGNED_SHORT; }
template<> constexpr uint32_t TypeToGLType<int8_t>() noexcept { return GL_BYTE; }
template<> constexpr uint32_t TypeToGLType<uint8_t>() noexcept { return GL_UNSIGNED_BYTE; }
} // game

#endif // GAME_UTILS_GL_HPP