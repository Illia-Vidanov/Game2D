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
  TextureDefinition(const std::string &new_source_path) noexcept : source_path{new_source_path} {}

  bool is_empty = false;
  uint32_t type = GL_TEXTURE_2D;
  const std::string &source_path;
  uint32_t internal_format = GL_RGBA8;
  uint32_t format = GL_RGBA;
  uint32_t mipmap_count = static_cast<uint32_t>(-1);
};

class Texture
{
public:
  Texture(const TextureDefinition &texture_definition) noexcept;

  void Delete() noexcept { GAME_GL_CALL(glDeleteTextures(1, &id_)); }

  void Bind() const noexcept { GAME_GL_CALL(glBindTexture(type_, id_)); }
  void Unbind() const noexcept { GAME_GL_CALL(glBindTexture(type_, 0)); }

private:
  const uint32_t type_;
  uint32_t id_ = 0;
};
} // game

#endif // GAME_TEXTURE_HPP