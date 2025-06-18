#ifndef GAME_SHADER_HPP
#define GAME_SHADER_HPP

#include "Setup.hpp"

#include "Rendering/Utils.hpp"
#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"


namespace game
{
class Shader
{
public:
  Shader() noexcept { GAME_GL_CALL(id_ = glCreateProgram()); }
  void Delete() const noexcept { GAME_GL_CALL(glDeleteProgram(id_)); }
  
  // Temp. Not sure if needed
  uint32_t GetID() const noexcept { return id_; }

  void Use() const noexcept { GAME_GL_CALL(glUseProgram(id_)); }
  
  [[nodiscard]] auto HasUniform(const std::string &name) const noexcept -> bool { return locations_.find(name) != locations_.end(); }
  template<typename T> void SetUniform(const std::string &name, const T t0) noexcept { GAME_ASSERT(false) << "No overload with such parametrs exists"; }
  template<typename T> void SetUniform(const std::string &name, const T t0, const T t1) noexcept { GAME_ASSERT(false) << "No overload with such parametrs exists"; }
  template<typename T> void SetUniform(const std::string &name, const T t0, const T t1, const T t2) noexcept { GAME_ASSERT(false) << "No overload with such parametrs exists"; }
  template<typename T> void SetUniform(const std::string &name, const T t0, const T t1, const T t2, const T t3) noexcept { GAME_ASSERT(false) << "No overload with such parametrs exists"; }
  template<typename T> void SetUniform1v(const std::string &name, const int count, const T *t) noexcept { GAME_ASSERT(false) << "No overload with such parametrs exists"; }
  template<typename T> void SetUniform2v(const std::string &name, const int count, const T *t) noexcept { GAME_ASSERT(false) << "No overload with such parametrs exists"; }
  template<typename T> void SetUniform3v(const std::string &name, const int count, const T *t) noexcept { GAME_ASSERT(false) << "No overload with such parametrs exists"; }
  template<typename T> void SetUniform4v(const std::string &name, const int count, const T *t) noexcept { GAME_ASSERT(false) << "No overload with such parametrs exists"; }
  void SetUniformMatrix2(const std::string &name, const int count, const bool transpose, const float *t) noexcept;
  void SetUniformMatrix3(const std::string &name, const int count, const bool transpose, const float *t) noexcept;
  void SetUniformMatrix4(const std::string &name, const int count, const bool transpose, const float *t) noexcept;
  void SetUniformMatrix2x3(const std::string &name, const int count, const bool transpose, const float *t) noexcept;
  void SetUniformMatrix3x2(const std::string &name, const int count, const bool transpose, const float *t) noexcept;
  void SetUniformMatrix2x4(const std::string &name, const int count, const bool transpose, const float *t) noexcept;
  void SetUniformMatrix4x2(const std::string &name, const int count, const bool transpose, const float *t) noexcept;
  void SetUniformMatrix3x4(const std::string &name, const int count, const bool transpose, const float *t) noexcept;
  void SetUniformMatrix4x3(const std::string &name, const int count, const bool transpose, const float *t) noexcept;

  bool operator==(const Shader &other) noexcept { return id_ == other.id_; }
  bool operator!=(const Shader &other) noexcept { return id_ != other.id_; }

  void CompileShaders(const std::string *begin, const std::string *end) noexcept { GAME_ASSERT(begin <= end); for(; begin != end; ++begin) CompileShader(*begin); }
  void CompileShader(const std::string &path) noexcept;
  void LinkProgram() noexcept;
  
  
  private:
  static inline auto ExtensionToShaderType(const std::string &extension) noexcept -> uint32_t { GAME_ASSERT(kExtensionToShaderType.find(extension) != kExtensionToShaderType.end()); return kExtensionToShaderType.at(extension); }

  static inline const std::unordered_map<std::string, uint32_t> kExtensionToShaderType = 
  {
    { ".vert", GL_VERTEX_SHADER },
    { ".frag", GL_FRAGMENT_SHADER },
    { ".geom", GL_GEOMETRY_SHADER },
    { ".vs", GL_VERTEX_SHADER },
    { ".fs", GL_FRAGMENT_SHADER },
    { ".gs", GL_GEOMETRY_SHADER },
  };

  
  uint32_t id_;
  std::vector<uint32_t> compiled_shaders_;
  std::unordered_map<std::string, std::pair<GLuint, std::any>> locations_;
};

template<>
inline void Shader::SetUniform<int>(const std::string &name, int t0) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);
  
  std::pair<GLuint, std::any> &data = locations_.at(name);
  if(data.second.type() == typeid(int) && std::any_cast<int>(data.second) == t0)
    return;
  
  data.second.emplace<int>(t0);

  Use();
  GAME_GL_CALL(glUniform1i(data.first, t0));
}

template<>
inline void Shader::SetUniform<int>(const std::string &name, int t0, int t1) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);
  
  std::pair<GLuint, std::any> &data = locations_.at(name);
  if(data.second.type() == typeid(std::array<int, 2>) && std::any_cast<std::array<int, 2>>(data.second) == std::array<int, 2>{t0, t1})
    return;
  
  data.second.emplace<std::array<int, 2>>(std::array<int, 2>{t0, t1});

  Use();
  GAME_GL_CALL(glUniform2i(data.first, t0, t1));
}

template<>
inline void Shader::SetUniform<int>(const std::string &name, int t0, int t1, int t2) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);
  
  std::pair<GLuint, std::any> &data = locations_.at(name);
  if(data.second.type() == typeid(std::array<int, 3>) && std::any_cast<std::array<int, 3>>(data.second) == std::array<int, 3>{t0, t1, t2})
    return;
  
  data.second.emplace<std::array<int, 3>>(std::array<int, 3>{t0, t1, t2});

  Use();
  GAME_GL_CALL(glUniform3i(data.first, t0, t1, t2));
}

template<>
inline void Shader::SetUniform<int>(const std::string &name, int t0, int t1, int t2, int t3) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);
  
  std::pair<GLuint, std::any> &data = locations_.at(name);
  if(data.second.type() == typeid(std::array<int, 4>) && std::any_cast<std::array<int, 4>>(data.second) == std::array<int, 4>{t0, t1, t2, t3})
    return;
  
  data.second.emplace<std::array<int, 4>>(std::array<int, 4>{t0, t1, t2, t3});

  Use();
  GAME_GL_CALL(glUniform4i(data.first, t0, t1, t2, t3));
}

template<>
inline void Shader::SetUniform1v<int>(const std::string &name, int count, const int *t) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);
  
  Use();
  GAME_GL_CALL(glUniform1iv(locations_.at(name).first, count, t));
}

template<>
inline void Shader::SetUniform2v<int>(const std::string &name, int count, const int *t) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);
  
  Use();
  GAME_GL_CALL(glUniform2iv(locations_.at(name).first, count, t));
}

template<>
inline void Shader::SetUniform3v<int>(const std::string &name, int count, const int *t) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);
  
  Use();
  GAME_GL_CALL(glUniform3iv(locations_.at(name).first, count, t));
}

template<>
inline void Shader::SetUniform4v<int>(const std::string &name, int count, const int *t) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);
  
  Use();
  GAME_GL_CALL(glUniform4iv(locations_.at(name).first, count, t));
}


template<>
inline void Shader::SetUniform<float>(const std::string &name, float t0) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);
  
  std::pair<GLuint, std::any> &data = locations_.at(name);
  if(data.second.type() == typeid(float) && std::any_cast<float>(data.second) == t0)
    return;
  
  data.second.emplace<float>(t0);

  Use();
  GAME_GL_CALL(glUniform1f(data.first, t0));
}

template<>
inline void Shader::SetUniform<float>(const std::string &name, float t0, float t1) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);
  
  std::pair<GLuint, std::any> &data = locations_.at(name);
  if(data.second.type() == typeid(std::array<float, 2>) && std::any_cast<std::array<float, 2>>(data.second) == std::array<float, 2>{t0, t1})
    return;
  
  data.second.emplace<std::array<float, 2>>(std::array<float, 2>{t0, t1});

  Use();
  GAME_GL_CALL(glUniform2f(data.first, t0, t1));
}

template<>
inline void Shader::SetUniform<float>(const std::string &name, float t0, float t1, float t2) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);
  
  std::pair<GLuint, std::any> &data = locations_.at(name);
  if(data.second.type() == typeid(std::array<float, 3>) && std::any_cast<std::array<float, 3>>(data.second) == std::array<float, 3>{t0, t1, t2})
    return;
  
  data.second.emplace<std::array<float, 3>>(std::array<float, 3>{t0, t1, t2});

  Use();
  GAME_GL_CALL(glUniform3f(data.first, t0, t1, t2));
}

template<>
inline void Shader::SetUniform<float>(const std::string &name, float t0, float t1, float t2, float t3) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);
  
  std::pair<GLuint, std::any> &data = locations_.at(name);
  if(data.second.type() == typeid(std::array<float, 4>) && std::any_cast<std::array<float, 4>>(data.second) == std::array<float, 4>{t0, t1, t2, t3})
    return;
  
  data.second.emplace<std::array<float, 4>>(std::array<float, 4>{t0, t1, t2, t3});

  Use();
  GAME_GL_CALL(glUniform4f(data.first, t0, t1, t2, t3));
}

template<>
inline void Shader::SetUniform1v<float>(const std::string &name, int count, const float *t) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);
  
  Use();
  GAME_GL_CALL(glUniform1fv(locations_.at(name).first, count, t));
}

template<>
inline void Shader::SetUniform2v<float>(const std::string &name, int count, const float *t) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);
  
  Use();
  GAME_GL_CALL(glUniform2fv(locations_.at(name).first, count, t));
}

template<>
inline void Shader::SetUniform3v<float>(const std::string &name, int count, const float *t) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);
  
  Use();
  GAME_GL_CALL(glUniform3fv(locations_.at(name).first, count, t));
}

template<>
inline void Shader::SetUniform4v<float>(const std::string &name, int count, const float *t) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);
  
  Use();
  GAME_GL_CALL(glUniform4fv(locations_.at(name).first, count, t));
}


template<>
inline void Shader::SetUniform<uint32_t>(const std::string &name, uint32_t t0) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);
  
  std::pair<GLuint, std::any> &data = locations_.at(name);
  if(data.second.type() == typeid(uint32_t) && std::any_cast<uint32_t>(data.second) == t0)
    return;
  
  data.second.emplace<uint32_t>(t0);

  Use();
  GAME_GL_CALL(glUniform1ui(data.first, t0));
}

template<>
inline void Shader::SetUniform<uint32_t>(const std::string &name, uint32_t t0, uint32_t t1) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);
  
  std::pair<GLuint, std::any> &data = locations_.at(name);
  if(data.second.type() == typeid(std::array<uint32_t, 2>) && std::any_cast<std::array<uint32_t, 2>>(data.second) == std::array<uint32_t, 2>{t0, t1})
    return;
  
  data.second.emplace<std::array<uint32_t, 2>>(std::array<uint32_t, 2>{t0, t1});

  Use();
  GAME_GL_CALL(glUniform2ui(data.first, t0, t1));
}

template<>
inline void Shader::SetUniform<uint32_t>(const std::string &name, uint32_t t0, uint32_t t1, uint32_t t2) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);
  
  std::pair<GLuint, std::any> &data = locations_.at(name);
  if(data.second.type() == typeid(std::array<uint32_t, 3>) && std::any_cast<std::array<uint32_t, 3>>(data.second) == std::array<uint32_t, 3>{t0, t1, t2})
    return;
  
  data.second.emplace<std::array<uint32_t, 3>>(std::array<uint32_t, 3>{t0, t1, t2});

  Use();
  GAME_GL_CALL(glUniform3ui(data.first, t0, t1, t2));
}

template<>
inline void Shader::SetUniform<uint32_t>(const std::string &name, uint32_t t0, uint32_t t1, uint32_t t2, uint32_t t3) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);
  
  std::pair<GLuint, std::any> &data = locations_.at(name);
  if(data.second.type() == typeid(std::array<uint32_t, 4>) && std::any_cast<std::array<uint32_t, 4>>(data.second) == std::array<uint32_t, 4>{t0, t1, t2, t3})
    return;
  
  data.second.emplace<std::array<uint32_t, 4>>(std::array<uint32_t, 4>{t0, t1, t2, t3});

  Use();
  GAME_GL_CALL(glUniform4ui(data.first, t0, t1, t2, t3));
}

template<>
inline void Shader::SetUniform1v<uint32_t>(const std::string &name, int count, const uint32_t *t) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);

  Use();
  GAME_GL_CALL(glUniform1uiv(locations_.at(name).first, count, t));
}

template<>
inline void Shader::SetUniform2v<uint32_t>(const std::string &name, int count, const uint32_t *t) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);

  Use();
  GAME_GL_CALL(glUniform2uiv(locations_.at(name).first, count, t));
}

template<>
inline void Shader::SetUniform3v<uint32_t>(const std::string &name, int count, const uint32_t *t) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);
  
  Use();
  GAME_GL_CALL(glUniform3uiv(locations_.at(name).first, count, t));
}

template<>
inline void Shader::SetUniform4v<uint32_t>(const std::string &name, int count, const uint32_t *t) noexcept
{
  GAME_ASSERT(HasUniform(name));
  ZoneScopedC(0xDD9B00);
  
  Use();
  GAME_GL_CALL(glUniform4uiv(locations_.at(name).first, count, t));
}
} // game

#endif // GAME_SHADER_HPP