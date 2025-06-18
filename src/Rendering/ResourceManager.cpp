#include "ResourceManager.hpp"

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Rendering/VAO.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Buffers.hpp"
#include "Rendering/Texture.hpp"
#include "Rendering/DataStructures.hpp"
#include "Core/Game.hpp"
#include "Core/EventSystem.hpp"
#include "Utils/Math.hpp"
#include "Physics/TransformComponent.hpp"


namespace game
{
ResourceManager::ResourceManager(Game &game) noexcept
  : game_{game}
{
  ZoneScopedC(0xC4DD00);

  sprite_vbo_.BufferData({
    {-0.5f, -0.5f}, {0.5f, -0.5f}, {-0.5f, 0.5f}, {0.5f, 0.5f}
  });
  sprite_ebo_.BufferData({
    0, 1, 2,
    1, 2, 3
  });
  sprite_vao_.AddVBO(sprite_vbo_, BufferLayout{GL_FLOAT, 2, GL_FALSE});
  sprite_vao_.AddEBO(sprite_ebo_);

  screen_plane_vbo_.BufferData({
    {-1.0f, -1.0f}, {1.0f, -1.0f}, {-1.0f, 1.0f}, {1.0f, 1.0f}
  });
  screen_plane_ebo_.BufferData({
    0, 1, 2,
    1, 2, 3
  });
  screen_plane_vao_.AddVBO(screen_plane_vbo_, BufferLayout{GL_FLOAT, 2, GL_FALSE});
  screen_plane_vao_.AddEBO(screen_plane_ebo_);

  LoadShader(ShaderType::kDefaultSprite, {"res/Shaders/DefaultSprite.vert", "res/Shaders/DefaultSprite.frag"});
  LoadShader(ShaderType::kTexturedSprite, {"res/Shaders/TexturedSprite.vert", "res/Shaders/TexturedSprite.frag"});
  LoadShader(ShaderType::kAnimatedSprite, {"res/Shaders/AnimatedSprite.vert", "res/Shaders/AnimatedSprite.frag"});
  LoadShader(ShaderType::kColoredSprite, {"res/Shaders/ColoredSprite.vert", "res/Shaders/ColoredSprite.frag"});

  orthographic_projection_ = TransformComponent::OrthographicProjection2D(0, Window::kUnitsPerScreenX, 0, Window::kUnitsPerScreenY);
  orthographic_projection_.block<2, 1>(0, 2) += Vector2{1.0f, 1.0f}; // shift projection such that 0 0 is at the center

  LoadTexture(TextureType::kNoTexture64, TextureDefinition{"res/Textures/NoTexture64.png"});
  LoadTexture(TextureType::kWhite, TextureDefinition{"res/Textures/White.png"});
  LoadTexture(TextureType::kPlayer, TextureDefinition{"res/Textures/Player/Idle.png"});
}

ResourceManager::~ResourceManager() noexcept
{
  ZoneScopedC(0xC4DD00);

  sprite_vao_.Delete();
  sprite_ebo_.Delete();
  sprite_vbo_.Delete();

  for(ShaderContainerType::value_type type_shader_pair : shaders_)
    type_shader_pair.second.Delete();
  for(TextureContainerType::value_type type_texture_pair : textures_)
    type_texture_pair.second.Delete();
}

auto ResourceManager::LoadTexture(TextureType type, const TextureDefinition &texture_definition) noexcept -> Texture &
{
  ZoneScopedC(0xC4DD00);
  
  GAME_ASSERT(textures_.find(type) == textures_.end()) << "Texture with type \'" << type << "\' is already loaded";
  return textures_.emplace(type, Texture{texture_definition}).first->second;
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