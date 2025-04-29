#ifndef GAME_BUFFERS_HPP
#define GAME_BUFFERS_HPP

#include "Setup.hpp"

#include "Rendering/Utils.hpp"
#include "Rendering/DataStructures.hpp"


namespace game
{
struct Vertex;

class VBO
{
public:
  inline VBO() noexcept { GL_CALL(glGenBuffers(1, &id_)); }
  inline VBO(std::vector<Vertex> verts) noexcept { GL_CALL(glGenBuffers(1, &id_)); BufferData(verts); }
  inline void Delete() const noexcept { GL_CALL(glDeleteBuffers(1, &id_)); }

  inline void Bind() const noexcept { GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, id_)); }
  inline void Unbind() const noexcept { GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0)); }

  inline void BufferData(const std::vector<Vertex> &verts) const noexcept;

private:
  uint32_t id_;
};


class EBO
{
public:
  inline EBO() noexcept { GL_CALL(glGenBuffers(1, &id_)); }
  inline void Delete() const noexcept { GL_CALL(glDeleteBuffers(1, &id_)); }

  inline void Bind() const noexcept { GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_)); }
  inline void Unbind() const noexcept { GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }

  inline void BufferData(const std::vector<Index> &inds) noexcept;
  inline constexpr auto GetCount() const noexcept -> Index { return count_; }
  inline constexpr auto GetGLType() const noexcept -> uint32_t { return GetGLTypeFromType<Index>(); }

private:
  uint32_t id_;
  Index count_; // Use Index type because it's the smallest type that can fit all of the indexes
};

inline void VBO::BufferData(const std::vector<Vertex> &verts) const noexcept
{
  ZoneScopedC(0x4400FF);

  Bind();
  GL_CALL(glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), verts.data(), GL_STATIC_DRAW));
}

inline void EBO::BufferData(const std::vector<Index> &inds) noexcept
{
  ZoneScopedC(0x4400FF);

  Bind();
  count_ = inds.size();
  GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, inds.size() * sizeof(Index), inds.data(), GL_STATIC_DRAW));
}
} // game

#endif // GAME_BUFFERS_HPP