#include "Shader.hpp"

#include "Setup.hpp"


#include "Utils/FileReader.hpp"


namespace game
{
void Shader::CompileShader(const std::string &path) noexcept
{
  ZoneScopedC(0xDD9B00);

  std::string source = FileReader::Txt(path);
  const char *char_source = source.c_str();

  int succes;
  uint32_t shader_id;
  GL_CALL(shader_id = glCreateShader(ExtensionToShaderType(std::filesystem::path{path}.extension().generic_u8string())));
  GL_CALL(glShaderSource(shader_id, 1, &char_source, NULL));
  GL_CALL(glCompileShader(shader_id));
  GL_CALL(glGetShaderiv(shader_id, GL_COMPILE_STATUS, &succes));
  if(!succes)
  {
    char buffer[512];
    GL_CALL(glGetShaderInfoLog(shader_id, 512, NULL, buffer));
    GAME_VLOG(1, LogType::kError) << "During compiling of: \'" << path << "\'\n"
                                    "Got OpenGL error: " << buffer;
  }

  GL_CALL(glAttachShader(id_, shader_id));

  compiled_shaders_.push_back(shader_id);
}

void Shader::LinkProgram() noexcept
{
  ZoneScopedC(0xDD9B00);
  
  int succes;
  GL_CALL(glLinkProgram(id_));
  GL_CALL(glGetProgramiv(id_, GL_LINK_STATUS, &succes));
  if(!succes)
  {
    char buffer[512];
    GL_CALL(glGetProgramInfoLog(id_, 512, NULL, buffer));
    GAME_VLOG(1, LogType::kError) << "During linking shaders\n"
                                    "Got OpenGL error: " << buffer;
  }

  for(std::size_t i = 0; i < compiled_shaders_.size(); i++)
    GL_CALL(glDeleteShader(compiled_shaders_[i]));

  compiled_shaders_.clear();
}

auto Shader::ExtensionToShaderType(const std::string &extension) noexcept -> uint32_t
{
  GAME_ASSERT(kExtensionToShaderType.find(extension) != kExtensionToShaderType.end()) << "Invalid Shader extension: " << extension << "\n"
                                                                                         "Should be one of the folowing: vert, frag, geom, vs, fs, gs";
  return kExtensionToShaderType.at(extension);
}
} // game