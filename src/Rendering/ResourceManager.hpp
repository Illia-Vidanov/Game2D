#ifndef GAME_RESOURCE_MANAGER
#define GAME_RESOURCE_MANAGER

#include "Setup.hpp"

#include <string>
#include <unordered_map>

#include "Rendering/VAO.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Buffers.hpp"
#include "Rendering/DataStructures.hpp"
#include "Core/EventHandler.hpp"


namespace game
{
class Game;

enum class ShaderType : uint32_t
{
  kDefault = 0
};

class ResourceManager
{
public:
  ResourceManager(Game &game) noexcept;
  void Exit() noexcept;
  void InitEvents() noexcept;

  inline auto LoadShader(ShaderType type) noexcept -> Shader & { return LoadShader(type, nullptr, nullptr); }
  inline auto LoadShader(ShaderType type, std::initializer_list<std::string> paths) noexcept -> Shader & { return LoadShader(type, paths.begin(), paths.end()); }
  auto LoadShader(ShaderType type, const std::string *begin, const std::string *end) noexcept -> Shader &;
  [[nodiscard]] inline auto GetShader(ShaderType type) const noexcept -> const Shader & { GAME_ASSERT(shaders_.find(type) != shaders_.end()) << "No shader with type: \'" << type << "\'"; return shaders_.at(type); }
  [[nodiscard]] inline auto GetShader(ShaderType type) noexcept -> Shader & { GAME_ASSERT(shaders_.find(type) != shaders_.end()) << "No shader with type: \'" << type << "\'"; return shaders_.at(type); }
  [[nodiscard]] inline auto GetSpriteIndexCount() -> Index { return sprite_ebo_.GetCount(); }
  inline void BindSpriteVAO() { sprite_vao_.Bind(); }

  
private:
  using ShaderContainerType = std::unordered_map<ShaderType, Shader>;
  
  ShaderContainerType shaders_;
  VBO sprite_vbo_;
  EBO sprite_ebo_;
  VAO sprite_vao_;

  Game &game_;
  EventCleaner event_cleaner_;
};
} // game

#endif // GAME_RESOURCE_MANAGER