#include "Rendering/Texture.hpp"

#include "Setup.hpp"

#include "Rendering/Utils.hpp"
#include "Utils/FileReader.hpp"
#include "Texture.hpp"
#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"


namespace game
{
Texture::Texture(const TextureDefinition &texture_definition) noexcept
  : type_{texture_definition.type}
{
  ZoneScopedC(0x57005A);

  if(texture_definition.is_empty)
    return;

  GAME_GL_CALL(glGenTextures(1, &id_));
  Bind();

  for(const std::pair<GLenum, int> &parametr : texture_definition.parametrs_int)
    GAME_GL_CALL(glTexParameteri(type_, parametr.first, parametr.second));
  
  for(const std::pair<GLenum, float> &parametr : texture_definition.parametrs_float)
    GAME_GL_CALL(glTexParameterf(type_, parametr.first, parametr.second));
  
  for(const std::pair<GLenum, int*> &parametr : texture_definition.parametrs_int_vector)
    GAME_GL_CALL(glTexParameteriv(type_, parametr.first, parametr.second));
    
  for(const std::pair<GLenum, float*> &parametr : texture_definition.parametrs_float_vector)
    GAME_GL_CALL(glTexParameterfv(type_, parametr.first, parametr.second));

  ImageData image_data = FileReader::Image(texture_definition.source_path);
  GAME_GL_CALL(glTexStorage2D(type_, 1, texture_definition.internal_format, image_data.width, image_data.height));
  GAME_GL_CALL(glTexSubImage2D(type_, 0, 0, 0, image_data.width, image_data.height, texture_definition.format, TypeToGLType<ImageData::DataType>(), image_data.data));
  image_data.Free();
  
  switch(texture_definition.mipmap_count)
  {
  default:
    GAME_GL_CALL(glTexParameteri(texture_definition.type, GL_TEXTURE_MAX_LEVEL, texture_definition.mipmap_count));
    GAME_GL_CALL(glGenerateMipmap(type_));
    break;
    
  case static_cast<uint32_t>(-1):
    GAME_GL_CALL(glGenerateMipmap(type_));
    break;

  case static_cast<uint32_t>(0):
    break;
  }
}

void Texture::UnbindAll() noexcept
{
  GAME_GL_CALL(glBindTexture(GL_TEXTURE_1D, 0));
  GAME_GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
  GAME_GL_CALL(glBindTexture(GL_TEXTURE_3D, 0));
  GAME_GL_CALL(glBindTexture(GL_TEXTURE_1D_ARRAY, 0));
  GAME_GL_CALL(glBindTexture(GL_TEXTURE_2D_ARRAY, 0));
  GAME_GL_CALL(glBindTexture(GL_TEXTURE_RECTANGLE, 0));
  GAME_GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
  GAME_GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, 0));
  GAME_GL_CALL(glBindTexture(GL_TEXTURE_BUFFER, 0));
  GAME_GL_CALL(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
  GAME_GL_CALL(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE_ARRAY, 0));
}
} // game