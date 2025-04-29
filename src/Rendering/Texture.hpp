#ifndef GAME_TEXTURE_HPP
#define GAME_TEXTURE_HPP

#include "Setup.hpp"

#include "Rendering/Utils.hpp"
#include "Utils/Math.hpp"


namespace game
{
enum class TextureFlags : int
{
  kNone = 0,

  //kGenerateMipmap = Bitmask(0),
  //kIsTransparent  = Bitmask(1),
};

class Texture
{
public:
  Texture(uint32_t type);

  inline void Delete() { GL_CALL(glDeleteTextures(1, &id_)); }

  inline void Bind() const noexcept { GL_CALL(glBindTexture(type_, id_)); }
  inline void Unbind() const noexcept { GL_CALL(glBindTexture(type_, 0)); }

  void BufferData(const std::string &path);
  void UpdateTexture() noexcept;
  
  [[nodiscard]] constexpr inline auto GetType() const noexcept -> uint32_t { return type_; }
  [[nodiscard]] constexpr inline auto GetFlags() const noexcept -> TextureFlags { return flags_; }
  inline void SetFlags(TextureFlags flags) { flags_ = flags; UpdateTexture(); }
  template<std::size_t Index = 0, typename... Args>
  void SetParametrs(const std::tuple<std::pair<int, Args>...> &data) const;
  template<typename T>
  inline void SetParametr(int name, T data) const { GAME_LOG(LogType::kError) << "No function to set glTexParametr with parametr of type \'" << typeid(T).name() << '\''; }

private:
  uint32_t id_ = 0;
  const uint32_t type_ = 0;
  TextureFlags flags_ = TextureFlags::kNone;
};

template<std::size_t Index, typename... Args>
void Texture::SetParametrs(const std::tuple<std::pair<int, Args>...> &data) const
{
  Bind();
  // Clamp is important because compiler looks ahead and gives errors
  if constexpr(Index != sizeof...(Args) - 1)
		SetParametrs<std::clamp(Index + 1, static_cast<std::size_t>(0), static_cast<std::size_t>(sizeof...(Args) - 1))>(data);

  SetParametr(std::get<0>(std::get<Index>(data)), std::get<1>(std::get<Index>(data)));
}

template<>
inline void Texture::SetParametr<int>(int name, int data) const
{
  GL_CALL(glTexParameteri(type_, name, data));
}

template<>
inline void Texture::SetParametr<float>(int name, float data) const
{
  GL_CALL(glTexParameterf(type_, name, data));
}
} // game

#endif // GAME_TEXTURE_HPP