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
  Shader() noexcept { GL_CALL(id_ = glCreateProgram()); }
  void Delete() const noexcept { GL_CALL(glDeleteProgram(id_)); }
  
  // Temp. Not sure if needed
  uint32_t GetID() const noexcept { return id_; }

  void Use() const noexcept { GL_CALL(glUseProgram(id_)); }
  
  [[nodiscard]] auto HasUniform(const std::string &name) const noexcept -> bool { int result; GL_CALL(result = glGetUniformLocation(id_, name.c_str())); return result != -1; }
  template<typename T> constexpr void SetUniform(const std::string &name, T t0) const noexcept { GAME_ASSERT_STD(false, "No overload with such parametrs exists"); }
  template<typename T> constexpr void SetUniform(const std::string &name, T t0, T t1) const noexcept { GAME_ASSERT_STD(false, "No overload with such parametrs exists"); }
  template<typename T> constexpr void SetUniform(const std::string &name, T t0, T t1, T t2) const noexcept { GAME_ASSERT_STD(false, "No overload with such parametrs exists"); }
  template<typename T> constexpr void SetUniform(const std::string &name, T t0, T t1, T t2, T t3) const noexcept { GAME_ASSERT_STD(false, "No overload with such parametrs exists"); }
  template<typename T> constexpr void SetUniform1v(const std::string &name, int count, const T *t) const noexcept { GAME_ASSERT_STD(false, "No overload with such parametrs exists"); }
  template<typename T> constexpr void SetUniform2v(const std::string &name, int count, const T *t) const noexcept { GAME_ASSERT_STD(false, "No overload with such parametrs exists"); }
  template<typename T> constexpr void SetUniform3v(const std::string &name, int count, const T *t) const noexcept { GAME_ASSERT_STD(false, "No overload with such parametrs exists"); }
  template<typename T> constexpr void SetUniform4v(const std::string &name, int count, const T *t) const noexcept { GAME_ASSERT_STD(false, "No overload with such parametrs exists"); }
  void SetUniformMatrix2(const std::string &name, int count, bool transpose, const float *t) const noexcept { Use(); GL_CALL(glUniformMatrix2fv(GetLocation(name), count, transpose, t)); }
  void SetUniformMatrix3(const std::string &name, int count, bool transpose, const float *t) const noexcept { Use(); GL_CALL(glUniformMatrix3fv(GetLocation(name), count, transpose, t)); }
  void SetUniformMatrix4(const std::string &name, int count, bool transpose, const float *t) const noexcept { Use(); GL_CALL(glUniformMatrix4fv(GetLocation(name), count, transpose, t)); }
  void SetUniformMatrix2x3(const std::string &name, int count, bool transpose, const float *t) const noexcept { Use(); GL_CALL(glUniformMatrix2x3fv(GetLocation(name), count, transpose, t)); }
  void SetUniformMatrix3x2(const std::string &name, int count, bool transpose, const float *t) const noexcept { Use(); GL_CALL(glUniformMatrix3x2fv(GetLocation(name), count, transpose, t)); }
  void SetUniformMatrix2x4(const std::string &name, int count, bool transpose, const float *t) const noexcept { Use(); GL_CALL(glUniformMatrix2x4fv(GetLocation(name), count, transpose, t)); }
  void SetUniformMatrix4x2(const std::string &name, int count, bool transpose, const float *t) const noexcept { Use(); GL_CALL(glUniformMatrix4x2fv(GetLocation(name), count, transpose, t)); }
  void SetUniformMatrix3x4(const std::string &name, int count, bool transpose, const float *t) const noexcept { Use(); GL_CALL(glUniformMatrix3x4fv(GetLocation(name), count, transpose, t)); }
  void SetUniformMatrix4x3(const std::string &name, int count, bool transpose, const float *t) const noexcept { Use(); GL_CALL(glUniformMatrix4x3fv(GetLocation(name), count, transpose, t)); }

  bool operator==(const Shader &other) noexcept { return id_ == other.id_; }
  bool operator!=(const Shader &other) noexcept { return id_ != other.id_; }

  void CompileShaders(const std::string *begin, const std::string *end) noexcept { GAME_ASSERT(begin <= end) << "Invalid range"; for(; begin != end; begin++) CompileShader(*begin); }
  void CompileShader(const std::string &path) noexcept;
  void LinkProgram() noexcept;

private:
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

  auto GetLocation(const std::string &name) const noexcept -> int;
  static auto ExtensionToShaderType(const std::string &extension) noexcept -> uint32_t;
};

template<>
inline void Shader::SetUniform<int>(const std::string &name, int t0) const noexcept
{
  Use();
  GL_CALL(glUniform1i(GetLocation(name), t0));
}

template<>
inline void Shader::SetUniform<int>(const std::string &name, int t0, int t1) const noexcept
{
  Use();
  GL_CALL(glUniform2i(GetLocation(name), t0, t1));
}

template<>
inline void Shader::SetUniform<int>(const std::string &name, int t0, int t1, int t2) const noexcept
{
  Use();
  GL_CALL(glUniform3i(GetLocation(name), t0, t1, t2));
}

template<>
inline void Shader::SetUniform<int>(const std::string &name, int t0, int t1, int t2, int t3) const noexcept
{
  Use();
  GL_CALL(glUniform4i(GetLocation(name), t0, t1, t2, t3));
}

template<>
inline void Shader::SetUniform1v<int>(const std::string &name, int count, const int *t) const noexcept
{
  Use();
  GL_CALL(glUniform1iv(GetLocation(name), count, t));
}

template<>
inline void Shader::SetUniform2v<int>(const std::string &name, int count, const int *t) const noexcept
{
  Use();
  GL_CALL(glUniform2iv(GetLocation(name), count, t));
}

template<>
inline void Shader::SetUniform3v<int>(const std::string &name, int count, const int *t) const noexcept
{
  Use();
  GL_CALL(glUniform3iv(GetLocation(name), count, t));
}

template<>
inline void Shader::SetUniform4v<int>(const std::string &name, int count, const int *t) const noexcept
{
  Use();
  GL_CALL(glUniform4iv(GetLocation(name), count, t));
}


template<>
inline void Shader::SetUniform<float>(const std::string &name, float t0) const noexcept
{
  Use();
  GL_CALL(glUniform1f(GetLocation(name), t0));
}

template<>
inline void Shader::SetUniform<float>(const std::string &name, float t0, float t1) const noexcept
{
  Use();
  GL_CALL(glUniform2f(GetLocation(name), t0, t1));
}

template<>
inline void Shader::SetUniform<float>(const std::string &name, float t0, float t1, float t2) const noexcept
{
  Use();
  GL_CALL(glUniform3f(GetLocation(name), t0, t1, t2));
}

template<>
inline void Shader::SetUniform<float>(const std::string &name, float t0, float t1, float t2, float t3) const noexcept
{
  Use();
  GL_CALL(glUniform4f(GetLocation(name), t0, t1, t2, t3));
}

template<>
inline void Shader::SetUniform1v<float>(const std::string &name, int count, const float *t) const noexcept
{
  Use();
  GL_CALL(glUniform1fv(GetLocation(name), count, t));
}

template<>
inline void Shader::SetUniform2v<float>(const std::string &name, int count, const float *t) const noexcept
{
  Use();
  GL_CALL(glUniform2fv(GetLocation(name), count, t));
}

template<>
inline void Shader::SetUniform3v<float>(const std::string &name, int count, const float *t) const noexcept
{
  Use();
  GL_CALL(glUniform3fv(GetLocation(name), count, t));
}

template<>
inline void Shader::SetUniform4v<float>(const std::string &name, int count, const float *t) const noexcept
{
  Use();
  GL_CALL(glUniform4fv(GetLocation(name), count, t));
}


template<>
inline void Shader::SetUniform<uint32_t>(const std::string &name, uint32_t t0) const noexcept
{
  Use();
  GL_CALL(glUniform1ui(GetLocation(name), t0));
}

template<>
inline void Shader::SetUniform<uint32_t>(const std::string &name, uint32_t t0, uint32_t t1) const noexcept
{
  Use();
  GL_CALL(glUniform2ui(GetLocation(name), t0, t1));
}

template<>
inline void Shader::SetUniform<uint32_t>(const std::string &name, uint32_t t0, uint32_t t1, uint32_t t2) const noexcept
{
  Use();
  GL_CALL(glUniform3ui(GetLocation(name), t0, t1, t2));
}

template<>
inline void Shader::SetUniform<uint32_t>(const std::string &name, uint32_t t0, uint32_t t1, uint32_t t2, uint32_t t3) const noexcept
{
  Use();
  GL_CALL(glUniform4ui(GetLocation(name), t0, t1, t2, t3));
}

template<>
inline void Shader::SetUniform1v<uint32_t>(const std::string &name, int count, const uint32_t *t) const noexcept
{
  Use();
  GL_CALL(glUniform1uiv(GetLocation(name), count, t));
}

template<>
inline void Shader::SetUniform2v<uint32_t>(const std::string &name, int count, const uint32_t *t) const noexcept
{
  Use();
  GL_CALL(glUniform2uiv(GetLocation(name), count, t));
}

template<>
inline void Shader::SetUniform3v<uint32_t>(const std::string &name, int count, const uint32_t *t) const noexcept
{
  Use();
  GL_CALL(glUniform3uiv(GetLocation(name), count, t));
}

template<>
inline void Shader::SetUniform4v<uint32_t>(const std::string &name, int count, const uint32_t *t) const noexcept
{
  Use();
  GL_CALL(glUniform4uiv(GetLocation(name), count, t));
}

inline auto Shader::GetLocation(const std::string &name) const noexcept -> int
{
  int result;
  GL_CALL(result = glGetUniformLocation(id_, name.c_str()));
  GAME_LOG_IF(result == -1, LogType::kError) << '\'' << name << '\'' << " does not correspond to an active uniform variable in program, name starts with the reserved prefix gl_, or name is associated with an atomic counter or a named uniform block.";
  
  return result;
}
} // game

#endif // GAME_SHADER_HPP