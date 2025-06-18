#ifndef GAME_TEXTURE_HPP
#define GAME_TEXTURE_HPP

#include "Setup.hpp"

#include "Rendering/Utils.hpp"
#include "Utils/Math.hpp"
#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"


namespace game
{
struct TextureDefinition
{
public:
  bool is_empty = false;
  uint32_t type = GL_TEXTURE_2D;
  const char *source_path;
  uint32_t internal_format = GL_RGBA8;
  uint32_t format = GL_RGBA;
  uint32_t mipmap_count = static_cast<uint32_t>(-1);
  std::vector<std::pair<GLenum, int>> parametrs_int;
  std::vector<std::pair<GLenum, float>> parametrs_float;
  std::vector<std::pair<GLenum, int*>> parametrs_int_vector;
  std::vector<std::pair<GLenum, float*>> parametrs_float_vector;

  constexpr auto SetIsEmpty(const bool new_is_empty) noexcept -> TextureDefinition & { is_empty = new_is_empty; return *this; }
  constexpr auto SetType(const uint32_t new_type) noexcept -> TextureDefinition & { type = new_type; return *this; }
  constexpr auto SetSourcePath(const char *new_source_path) noexcept -> TextureDefinition & { source_path = new_source_path; return *this; }
  constexpr auto SetInternalFormat(const uint32_t new_internal_format) noexcept -> TextureDefinition & { internal_format = new_internal_format; return *this; }
  constexpr auto SetFormat(const uint32_t new_format) noexcept -> TextureDefinition & { format = new_format; return *this; }
  constexpr auto SetMipmapCount(const uint32_t new_mipmap_count) noexcept -> TextureDefinition & { mipmap_count = new_mipmap_count; return *this; }
  auto AddParametrInt(const std::pair<GLenum, int> &parametr) noexcept -> TextureDefinition & { parametrs_int.emplace_back(parametr); return *this; }
  auto AddParametrFloat(const std::pair<GLenum, float> &parametr) noexcept -> TextureDefinition & { parametrs_float.emplace_back(parametr); return *this; }
  auto AddParametrIntVector(const std::pair<GLenum, int*> &parametr) noexcept -> TextureDefinition & { parametrs_int_vector.emplace_back(parametr); return *this; }
  auto AddParametrFloatVector(const std::pair<GLenum, float*> &parametr) noexcept -> TextureDefinition & { parametrs_float_vector.emplace_back(parametr); return *this; }
};

class Texture
{
public:
  Texture(const TextureDefinition &texture_definition) noexcept;

  void Delete() noexcept { GAME_GL_CALL(glDeleteTextures(1, &id_)); }

  void Bind() const noexcept { GAME_GL_CALL(glBindTexture(type_, id_)); }
  void Unbind() const noexcept { GAME_GL_CALL(glBindTexture(type_, 0)); }

  static void UnbindAll() noexcept;

private:
  const uint32_t type_;
  uint32_t id_ = 0;
};
} // game

#endif // GAME_TEXTURE_HPP