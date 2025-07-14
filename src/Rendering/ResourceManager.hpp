#ifndef GAME_RESOURCE_MANAGER
#define GAME_RESOURCE_MANAGER

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Rendering/VAO.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Texture.hpp"
#include "Rendering/Buffers.hpp"
#include "Rendering/DataStructures.hpp"


namespace game
{
class Game;

enum class ShaderType : uint32_t
{
  kDefaultSprite = 0,
  kTexturedSprite,
  kAnimatedSprite,
  kColoredSprite,
};

enum class TextureType : uint32_t
{
  kDefault = 0,
  kNoTexture64 = kDefault,
  kWhite,
  kPlayer,
};

class ResourceManager
{
public:
  ResourceManager(Game &game) noexcept;
  ~ResourceManager() noexcept;
  ResourceManager(const ResourceManager &other) = delete;
  ResourceManager(ResourceManager &&other) = delete;
  ResourceManager &operator=(const ResourceManager &other) = delete;
  ResourceManager &operator=(ResourceManager &&other) = delete;

  auto LoadTexture(TextureType type, const TextureDefinition &texture_definition) noexcept -> Texture &;
  [[nodiscard]] auto GetTexture(TextureType type) const noexcept -> const Texture & { GAME_ASSERT(textures_.find(type) != textures_.end()) << "No texture with type \'" << type << "\'"; return textures_.at(type); }
  [[nodiscard]] auto GetTexture(TextureType type) noexcept -> Texture & { GAME_ASSERT(textures_.find(type) != textures_.end()) << "No texture with type \'" << type << "\'"; return textures_.at(type); }

  auto LoadShader(ShaderType type) noexcept -> Shader & { return LoadShader(type, nullptr, nullptr); }
  auto LoadShader(ShaderType type, std::initializer_list<std::string> paths) noexcept -> Shader & { return LoadShader(type, paths.begin(), paths.end()); }
  auto LoadShader(ShaderType type, const std::string *begin, const std::string *end) noexcept -> Shader &;
  [[nodiscard]] auto GetShader(ShaderType type) const noexcept -> const Shader & { GAME_ASSERT(shaders_.find(type) != shaders_.end()) << "No shader with type \'" << type << "\'"; return shaders_.at(type); }
  [[nodiscard]] auto GetShader(ShaderType type) noexcept -> Shader & { GAME_ASSERT(shaders_.find(type) != shaders_.end()) << "No shader with type \'" << type << "\'"; return shaders_.at(type); }
  
  [[nodiscard]] auto GetSpriteIndexCount() -> Index { return sprite_ebo_.GetCount(); }
  void BindSpriteVAO() { sprite_vao_.Bind(); }
  [[nodiscard]] auto GetScreenPlaneIndexCount() -> Index { return screen_plane_ebo_.GetCount(); }
  void BindScreenPlaneVAO() { screen_plane_vao_.Bind(); }
  [[nodiscard]] auto GetOrthographicProjection() const noexcept -> Matrix3 { return orthographic_projection_; }
  
  
  private:
  using ShaderContainerType = std::unordered_map<ShaderType, Shader>;
  using TextureContainerType = std::unordered_map<TextureType, Texture>;
  
  ShaderContainerType shaders_;
  TextureContainerType textures_;
  VBO sprite_vbo_;
  EBO sprite_ebo_;
  VAO sprite_vao_;
  VBO screen_plane_vbo_;
  EBO screen_plane_ebo_;
  VAO screen_plane_vao_;
  Matrix3 orthographic_projection_;
  
  Game &game_;
};
} // game

#endif // GAME_RESOURCE_MANAGER