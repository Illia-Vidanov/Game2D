#ifndef GAME_RESOURCE_MANAGER
#define GAME_RESOURCE_MANAGER

#include "Setup.hpp"

#include "Rendering/VAO.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Texture.hpp"
#include "Rendering/Buffers.hpp"
#include "Rendering/DataStructures.hpp"
#include "Core/EventSystem.hpp"


namespace game
{
class Game;

enum class ShaderType : uint32_t
{
  kDefault = 0
};

enum class TextureType : uint32_t
{
  kDefault = 0,
  kNoImage64 = kDefault,
  kWhite,
};

class ResourceManager
{
public:
  ResourceManager(Game &game) noexcept;
  void Exit() noexcept;
  void InitEvents() noexcept;

  auto LoadTexture(TextureType type, uint32_t texture_type, const std::string &path) noexcept -> Texture &;
  [[nodiscard]] inline auto GetTexture(TextureType type) const noexcept -> const Texture & { GAME_ASSERT(textures_.find(type) != textures_.end()) << "No texture with type \'" << type << "\'"; return textures_.at(type); }
  [[nodiscard]] inline auto GetTexture(TextureType type) noexcept -> Texture & { GAME_ASSERT(textures_.find(type) != textures_.end()) << "No texture with type \'" << type << "\'"; return textures_.at(type); }

  inline auto LoadShader(ShaderType type) noexcept -> Shader & { return LoadShader(type, nullptr, nullptr); }
  inline auto LoadShader(ShaderType type, std::initializer_list<std::string> paths) noexcept -> Shader & { return LoadShader(type, paths.begin(), paths.end()); }
  auto LoadShader(ShaderType type, const std::string *begin, const std::string *end) noexcept -> Shader &;
  [[nodiscard]] inline auto GetShader(ShaderType type) const noexcept -> const Shader & { GAME_ASSERT(shaders_.find(type) != shaders_.end()) << "No shader with type \'" << type << "\'"; return shaders_.at(type); }
  [[nodiscard]] inline auto GetShader(ShaderType type) noexcept -> Shader & { GAME_ASSERT(shaders_.find(type) != shaders_.end()) << "No shader with type \'" << type << "\'"; return shaders_.at(type); }
  
  [[nodiscard]] inline auto GetSpriteIndexCount() -> Index { return sprite_ebo_.GetCount(); }
  inline void BindSpriteVAO() { sprite_vao_.Bind(); }

  
private:
  using ShaderContainerType = std::unordered_map<ShaderType, Shader>;
  using TextureContainerType = std::unordered_map<TextureType, Texture>;
  
  ShaderContainerType shaders_;
  TextureContainerType textures_;
  VBO sprite_vbo_;
  EBO sprite_ebo_;
  VAO sprite_vao_;

  Game &game_;
  EventCleaner event_cleaner_;
};
} // game

#endif // GAME_RESOURCE_MANAGER