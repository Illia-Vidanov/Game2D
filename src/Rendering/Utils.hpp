#ifndef GAME_UTILS_GL_HPP
#define GAME_UTILS_GL_HPP

#include "Setup.hpp"




#ifndef NDEBUG
#define GL_CALL(call) do { \
    call; \
    while(int error = glGetError()) \
      GAME_VLOG(1, LogType::kError) << __LINE__ << ' ' << __FILE__ << " OpenGL error: " << error; \
  } while(0)
#else
#define GL_CALL(call) call
#endif

namespace game
{
constexpr inline std::size_t GetSizeFromGLType(uint32_t type) noexcept
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
constexpr inline uint32_t GetGLTypeFromType() noexcept { GAME_ASSERT(false) << "No GLType fot type " << typeid(Type).name(); return 0; }
template<> constexpr inline uint32_t GetGLTypeFromType<int32_t>() noexcept  { return GL_INT; }
template<> constexpr inline uint32_t GetGLTypeFromType<uint32_t>() noexcept { return GL_UNSIGNED_INT; }
template<> constexpr inline uint32_t GetGLTypeFromType<int16_t>() noexcept  { return GL_SHORT; }
template<> constexpr inline uint32_t GetGLTypeFromType<uint16_t>() noexcept { return GL_UNSIGNED_SHORT; }
template<> constexpr inline uint32_t GetGLTypeFromType<int8_t>() noexcept   { return GL_BYTE; }
template<> constexpr inline uint32_t GetGLTypeFromType<uint8_t>() noexcept  { return GL_UNSIGNED_BYTE; }
} // game

#endif // GAME_UTILS_GL_HPP