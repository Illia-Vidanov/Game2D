#ifndef GAME_VAO_HPP
#define GAME_VAO_HPP

#include "Setup.hpp"

#include <vector>

#include <glad/glad.h>

#include "Rendering/Utils.hpp"
#include "Rendering/Buffers.hpp"


namespace game
{
class VBO;
class EBO;

struct BufferLayoutElement
{
  inline BufferLayoutElement(uint32_t new_type, int8_t new_size, int8_t new_normalized, int8_t new_offset) noexcept
  : type(new_type)
  , size(new_size)
  , normalized(new_normalized)
  , offset(new_offset)
  {}

  uint32_t type;
  uint8_t size;
  uint8_t normalized;
  uint16_t offset;
};


class BufferLayout
{
public:
  inline BufferLayout() = default;
  template<typename... Args>
  inline BufferLayout(uint32_t type, uint8_t size, uint8_t normalized, Args... args) noexcept { static_assert(!(sizeof...(Args) % 3), "Number of arguments is not multiple of 3"); AddBufferLayoutElement(type, size, normalized); Iterate(args...); }

  template<typename... Args>
  void Iterate(uint32_t type, uint8_t size, uint8_t normalized, Args... args) noexcept { AddBufferLayoutElement(type, size, normalized); Iterate(args...); }
  constexpr inline void Iterate() noexcept {}

  inline void AddBufferLayoutElement(uint32_t type, uint8_t size, uint8_t normalized) noexcept { layout_elements_.emplace_back(type, size, normalized, stride_); stride_ += GetSizeFromGLType(type) * size; }
  constexpr inline auto GetStride() const noexcept -> std::size_t { return stride_; }
  constexpr inline auto GetLayoutElements() const noexcept -> const std::vector<BufferLayoutElement>& { return layout_elements_; }

private:
  std::vector<BufferLayoutElement> layout_elements_;
  std::size_t stride_ = 0;
};


class VAO
{
public:
  inline VAO() noexcept { GL_CALL(glGenVertexArrays(1, &id_)); }
  inline void Delete() const noexcept { GL_CALL(glDeleteVertexArrays(1, &id_)); }

  inline void Bind() const noexcept { GL_CALL(glBindVertexArray(id_)); }
  inline void Unbind() const noexcept { GL_CALL(glBindVertexArray(0)); }

  void AddVBO(const VBO &vbo, const BufferLayout &layout) const noexcept;
  inline void AddEBO(const EBO &ebo) const noexcept { Bind(); ebo.Bind(); }

private:
  uint32_t id_;
};
} // game

#endif // GAME_VAO_HPP