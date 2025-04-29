#include "ResourceManager.hpp"

#include <string>
#include <unordered_map>

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <glad/glad.h>

#include "Rendering/VAO.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Buffers.hpp"
#include "Rendering/Texture.hpp"
#include "Rendering/DataStructures.hpp"
#include "Core/Game.hpp"
#include "Core/EventSystem.hpp"
#include "Utils/Math.hpp"


namespace game
{
ResourceManager::ResourceManager(Game &game) noexcept
  : game_{game}
  , event_cleaner_{game_.GetEventSystem()}
{
  ZoneScopedC(0xC4DD00);

  sprite_vbo_.BufferData({
    {-1, -1, 0, 0}, {1, -1, 1, 0}, {-1, 1, 0, 1}, {1, 1, 1, 1}
  });
  sprite_ebo_.BufferData({
    0, 1, 2,
    1, 2, 3
  });
  sprite_vao_.AddVBO(sprite_vbo_, BufferLayout{GL_FLOAT, 4, GL_FALSE});
  sprite_vao_.AddEBO(sprite_ebo_);

  LoadShader(ShaderType::kDefault, {"res/Shaders/Default.vert", "res/Shaders/Default.frag"});

  Matrix4 orthographic_projection = OrthographicProjection(0, Window::kUnitsPerScreenX, 0, Window::kUnitsPerScreenY, -1, 1);
  orthographic_projection.block<2, 1>(0, 3) += Vector2{1.0f, 1.0f}; // shift projection such that 0 0 is at the center
  GetShader(ShaderType::kDefault).SetUniformMatrix4("projection", 1, false, orthographic_projection.data());

  LoadTexture(TextureType::kNoImage64, GL_TEXTURE_2D, "res/Textures/NoTexture64.png");
  LoadTexture(TextureType::kWhite, GL_TEXTURE_2D, "res/Textures/White.png");
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
  ZoneScopedC(0xC4DD00);
  
}

auto ResourceManager::LoadTexture(TextureType type, uint32_t texture_type, const std::string &path) noexcept -> Texture &
{
  ZoneScopedC(0xC4DD00);
  
  GAME_ASSERT(textures_.find(type) == textures_.end()) << "Texture with type \'" << type << "\' is already loaded";
  Texture &texture = textures_.emplace(type, Texture{texture_type}).first->second;

  texture.BufferData(path);

  return texture;
}

auto ResourceManager::LoadShader(ShaderType type, const std::string *begin, const std::string *end) noexcept -> Shader &
{
  ZoneScopedC(0xC4DD00);

  GAME_ASSERT(shaders_.find(type) == shaders_.end()) << "Shader with type \'" << type << "\' is already loaded";
  Shader &shader = shaders_.emplace(type, Shader{}).first->second;

  shader.CompileShaders(begin, end);
  shader.LinkProgram();

  return shader;
}
} // game