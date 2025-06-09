#ifndef GAME_SPRITE_COMPONENTS_HPP
#define GAME_SPRITE_COMPONENTS_HPP

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Rendering/DataStructures.hpp"
#include "Physics/TransformComponent.hpp"
#include "Core/Entity.hpp"


namespace game
{
class Texture;
class Shader;
class Game;


class DefaultSpriteComponent
{
public:
  DefaultSpriteComponent(Entity &entity) noexcept : entity_{entity} { GAME_ASSERT(entity_.HasComponent<TransformComponent>()); }

private:
  Entity &entity_;
};


class TexturedSpriteComponent
{
public:
  TexturedSpriteComponent(Entity &entity) noexcept : entity_{entity} { GAME_ASSERT(entity_.HasComponent<TransformComponent>()); }

  constexpr void SetShader(const Shader *shader) noexcept { shader_ = shader; }
  [[nodiscard]] auto GetShader() const noexcept -> const Shader & { GAME_ASSERT(shader_ != nullptr); return *shader_; }
  constexpr void SetTexture(const Texture *texture) noexcept { texture_ = texture; }
  [[nodiscard]] auto GetTexture() const noexcept -> const Texture & { GAME_ASSERT(texture_ != nullptr); return *texture_; }

private:
  Entity &entity_;

  const Shader *shader_ = nullptr;
  const Texture *texture_ = nullptr;
};


class AnimatedSpriteComponent
{
public:
  AnimatedSpriteComponent(Entity &entity) noexcept : entity_{entity} { GAME_ASSERT(entity_.HasComponent<TransformComponent>()); }

  void SetSpriteIndex(const Vector2i &sprite_index) noexcept { sprite_index_ = sprite_index; }
  [[nodiscard]] auto GetSpriteIndex() const noexcept -> const Vector2i & { return sprite_index_; }
  // Atlas step is 1 / max(width, height)
  constexpr void SetAtlasStep(const DefaultFloatType &atlas_step) noexcept { atlas_step_ = atlas_step; }
  [[nodiscard]] auto GetAtlasStep() const noexcept -> DefaultFloatType { return atlas_step_; }
  constexpr void SetShader(const Shader *shader) noexcept { shader_ = shader; }
  [[nodiscard]] auto GetShader() const noexcept -> const Shader & { GAME_ASSERT(shader_ != nullptr); return *shader_; }
  constexpr void SetTexture(const Texture *texture) noexcept { texture_ = texture; }
  [[nodiscard]] auto GetTexture() const noexcept -> const Texture & { GAME_ASSERT(texture_ != nullptr); return *texture_; }

private:
  Entity &entity_;

  Vector2i sprite_index_ = Vector2i::Zero();
  DefaultFloatType atlas_step_ = 1;
  const Shader *shader_ = nullptr;
  const Texture *texture_ = nullptr;
};
} // game

#endif // GAME_SPRITE_COMPONENT_HPP