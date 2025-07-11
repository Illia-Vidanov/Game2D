#ifndef GAME_BUFFERS_HPP
#define GAME_BUFFERS_HPP

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Rendering/Utils.hpp"
#include "Rendering/DataStructures.hpp"


namespace game
{
struct Vertex;

class VBO
{
public:
  VBO() noexcept { GAME_GL_CALL(glGenBuffers(1, &id_)); }
  VBO(std::vector<Vertex> verts) noexcept { GAME_GL_CALL(glGenBuffers(1, &id_)); BufferData(verts); }
  void Delete() const noexcept { GAME_GL_CALL(glDeleteBuffers(1, &id_)); }

  void Bind() const noexcept { GAME_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, id_)); }
  void Unbind() const noexcept { GAME_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0)); }

  void BufferData(const std::vector<Vertex> &verts) const noexcept;

private:
  uint32_t id_;
};


class EBO
{
public:
  EBO() noexcept { GAME_GL_CALL(glGenBuffers(1, &id_)); }
  void Delete() const noexcept { GAME_GL_CALL(glDeleteBuffers(1, &id_)); }

  void Bind() const noexcept { GAME_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_)); }
  void Unbind() const noexcept { GAME_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }

  void BufferData(const std::vector<Index> &inds) noexcept;
  constexpr auto GetCount() const noexcept -> Index { return count_; }
  constexpr auto GetGLType() const noexcept -> uint32_t { return GetGLTypeFromType<Index>(); }

private:
  uint32_t id_;
  Index count_; // Use Index type because it's the smallest type that can fit all of the indexes
};

inline void VBO::BufferData(const std::vector<Vertex> &verts) const noexcept
{
  ZoneScopedC(0x4400FF);

  Bind();
  GAME_GL_CALL(glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), verts.data(), GL_STATIC_DRAW));
}

inline void EBO::BufferData(const std::vector<Index> &inds) noexcept
{
  ZoneScopedC(0x4400FF);

  Bind();
  count_ = inds.size();
  GAME_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, inds.size() * sizeof(Index), inds.data(), GL_STATIC_DRAW));
}
} // game

#endif // GAME_BUFFERS_HPP