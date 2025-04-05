#include "ResourceManager.hpp"

#include <string>
#include <unordered_map>

#include <Eigen/Dense>
#include <Eigen/Geometry>

#include "Rendering/VAO.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Buffers.hpp"
#include "Core/Game.hpp"
#include "Core/EventHandler.hpp"
#include "Utils/Math.hpp"


namespace game
{
ResourceManager::ResourceManager(Game &game) noexcept
  : game_{game}
  , event_cleaner_{game_.GetEventHandler()}
{
  ZoneScopedC(0xC4DD00);

  sprite_vbo_.BufferData({
    {-1, -1}, {1, -1}, {-1, 1}, {1, 1}
  });
  sprite_ebo_.BufferData({
    0, 1, 2,
    1, 2, 3
  });
  sprite_vao_.AddVBO(sprite_vbo_, BufferLayout{GL_FLOAT, 2, GL_FALSE});
  sprite_vao_.AddEBO(sprite_ebo_);

  LoadShader(ShaderType::kDefault, {"res/Shaders/Default.vert", "res/Shaders/Default.frag"});
}

void ResourceManager::Exit() noexcept
{
  ZoneScopedC(0xC4DD00);

  sprite_vao_.Delete();
  sprite_ebo_.Delete();
  sprite_vbo_.Delete();

  for(ShaderContainerType::value_type type_shader_pair : shaders_)
    type_shader_pair.second.Delete();
}

void ResourceManager::InitEvents() noexcept
{
  game_.GetEventHandler().AddListener(event_cleaner_, EventType::kWindowResize, this, [](const Event &event, void *resource_manager){ return reinterpret_cast<ResourceManager*>(resource_manager)->ResizeEvent(event); });
}

auto ResourceManager::LoadShader(ShaderType type, const std::string *begin, const std::string *end) noexcept -> Shader &
{
  ZoneScopedC(0xC4DD00);

  GAME_ASSERT(shaders_.find(type) == shaders_.end()) << "Shader with name: \'" << type << "\' is already loaded";
  Shader &shader = shaders_.emplace(type, Shader()).first->second;

  shader.CompileShaders(begin, end);
  shader.LinkProgram();

  return shader;
}

auto ResourceManager::ResizeEvent(const Event &event) noexcept -> bool
{ 
  Transform orthographic_projection = Transform{OrthographicProjection(
    0,
    game_.GetWindow().GetRenderWidth(),
    0,
    game_.GetWindow().GetRenderHeight(),
    -1, 1)};
  //orthographic_projection.translation() += Vector3{
  //  (event.GetNewResolutionX() - game_.GetWindow().GetRenderWidth()) / 2,
  //  (event.GetNewResolutionY() - game_.GetWindow().GetRenderHeight()) / 2
  //};
  
  GetShader(ShaderType::kDefault).SetUniformMatrix4("projection", 1, false, orthographic_projection.data());
  
  return false;
}
} // game