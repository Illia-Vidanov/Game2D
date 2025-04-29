#include "Rendering/Texture.hpp"

#include "Setup.hpp"

#include "Rendering/Utils.hpp"
#include "Utils/FileReader.hpp"
#include "Texture.hpp"


namespace game
{
Texture::Texture(uint32_t type)
  : type_(type)
{
  ZoneScopedC(0x57005A);
  GL_CALL(glGenTextures(1, &id_));
}

void Texture::BufferData(const std::string &path)
{
  ZoneScopedC(0x57005A);

  
  ImageData image_data = FileReader::Image(path);
  
  Bind();
  GL_CALL(glTexStorage2D(type_, 1, GL_RGBA8, image_data.width, image_data.height));
  GL_CALL(glTexSubImage2D(type_, 0, 0, 0, image_data.width, image_data.height, GL_BGRA, GL_UNSIGNED_BYTE, image_data.data));
  GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));

  image_data.Free();
}

void Texture::UpdateTexture() noexcept
{
}
} // game