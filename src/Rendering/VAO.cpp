#include "VAO.hpp"

#include "Setup.hpp"

#include "Rendering/Buffers.hpp"
#include "Rendering/Utils.hpp"
#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"


namespace game
{
void VAO::AddVBO(const VBO &vbo, const BufferLayout &layout) const noexcept
{
  ZoneScopedC(0x00DD7A);

  Bind();
  vbo.Bind();
  const std::vector<BufferLayoutElement> &elements = layout.GetLayoutElements();

  for(std::size_t i = 0; i < elements.size(); i++)
  {
    GAME_GL_CALL(glVertexAttribPointer(i, elements[i].size, elements[i].type, elements[i].normalized, layout.GetStride(), reinterpret_cast<void*>(static_cast<uintptr_t>(elements[i].offset))));
    GAME_GL_CALL(glEnableVertexAttribArray(i));
  }
}
} // game