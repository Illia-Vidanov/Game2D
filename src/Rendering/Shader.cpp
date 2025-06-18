#include "Shader.hpp"

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
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
  GAME_GL_CALL(shader_id = glCreateShader(ExtensionToShaderType(std::filesystem::path{path}.extension().generic_u8string())));
  GAME_GL_CALL(glShaderSource(shader_id, 1, &char_source, NULL));
  GAME_GL_CALL(glCompileShader(shader_id));
  GAME_GL_CALL(glGetShaderiv(shader_id, GL_COMPILE_STATUS, &succes));
  if(!succes)
  {
    char buffer[512];
    GAME_GL_CALL(glGetShaderInfoLog(shader_id, 512, NULL, buffer));
    GAME_VLOG(1, LogType::kError) << "During compiling of: \'" << path << "\'\n"
                                    "Got OpenGL error: " << buffer;
  }

  GAME_GL_CALL(glAttachShader(id_, shader_id));

  compiled_shaders_.push_back(shader_id);
}

void Shader::LinkProgram() noexcept
{
  ZoneScopedC(0xDD9B00);
  
  int succes;
  GAME_GL_CALL(glLinkProgram(id_));
  GAME_GL_CALL(glGetProgramiv(id_, GL_LINK_STATUS, &succes));
  if(!succes)
  {
    char buffer[512];
    GAME_GL_CALL(glGetProgramInfoLog(id_, 512, NULL, buffer));
    GAME_VLOG(1, LogType::kError) << "During linking shaders\n"
                                    "Got OpenGL error: " << buffer;
  }

  for(std::size_t i = 0; i < compiled_shaders_.size(); i++)
    GAME_GL_CALL(glDeleteShader(compiled_shaders_[i]));

  compiled_shaders_.clear();

  int count;
  GAME_GL_CALL(glGetProgramiv(id_, GL_ACTIVE_UNIFORMS, &count));

  const std::size_t buffer_size = 32;
  GLchar *name = new char[buffer_size];
  for (int i = 0; i < count; ++i)
  {
    GAME_GL_CALL(glGetActiveUniform(id_, static_cast<GLuint>(i), buffer_size, nullptr, nullptr, nullptr, name));
    GAME_GL_CALL(locations_.emplace(std::pair<std::string, std::pair<GLuint, std::any>>(name, std::pair<GLuint, std::any>{glGetUniformLocation(id_, name), std::any{}})));
  }
}

void Shader::SetUniformMatrix2(const std::string &name, const int count, const bool transpose, const float *t) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);

  //const float *end = t + count * 4;
  std::pair<GLuint, std::any> &data = locations_.at(name);
  //if(data.second.has_value() && std::equal(t, end, std::any_cast<float*>(data.second)))
  //  return;
  //std::copy(t, end, data.second.emplace<float*>());
  Use();
  GAME_GL_CALL(glUniformMatrix2fv(data.first, count, transpose, t));
}

void Shader::SetUniformMatrix3(const std::string &name, const int count, const bool transpose, const float *t) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);
  
  Use();
  GAME_GL_CALL(glUniformMatrix3fv(locations_.at(name).first, count, transpose, t));
}

void Shader::SetUniformMatrix4(const std::string &name, const int count, const bool transpose, const float *t) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);

  std::pair<GLuint, std::any> &data = locations_.at(name);

  Use();
  GAME_GL_CALL(glUniformMatrix4fv(data.first, count, transpose, t));
}

void Shader::SetUniformMatrix2x3(const std::string &name, const int count, const bool transpose, const float *t) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);

  std::pair<GLuint, std::any> &data = locations_.at(name);

  Use();
  GAME_GL_CALL(glUniformMatrix2x3fv(data.first, count, transpose, t));
}

void Shader::SetUniformMatrix3x2(const std::string &name, const int count, const bool transpose, const float *t) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);

  std::pair<GLuint, std::any> &data = locations_.at(name);

  Use();
  GAME_GL_CALL(glUniformMatrix3x2fv(data.first, count, transpose, t));
}

void Shader::SetUniformMatrix2x4(const std::string &name, const int count, const bool transpose, const float *t) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);

  std::pair<GLuint, std::any> &data = locations_.at(name);

  Use();
  GAME_GL_CALL(glUniformMatrix2x4fv(data.first, count, transpose, t));
}

void Shader::SetUniformMatrix4x2(const std::string &name, const int count, const bool transpose, const float *t) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);

  std::pair<GLuint, std::any> &data = locations_.at(name);

  Use();
  GAME_GL_CALL(glUniformMatrix4x2fv(data.first, count, transpose, t));
}

void Shader::SetUniformMatrix3x4(const std::string &name, const int count, const bool transpose, const float *t) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);

  std::pair<GLuint, std::any> &data = locations_.at(name);

  Use();
  GAME_GL_CALL(glUniformMatrix3x4fv(data.first, count, transpose, t));
}

void Shader::SetUniformMatrix4x3(const std::string &name, const int count, const bool transpose, const float *t) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);

  std::pair<GLuint, std::any> &data = locations_.at(name);

  Use();
  GAME_GL_CALL(glUniformMatrix4x3fv(data.first, count, transpose, t));
}

} // game