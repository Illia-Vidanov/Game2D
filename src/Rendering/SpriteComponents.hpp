#ifndef GAME_SPRITE_COMPONENTS_HPP
#define GAME_SPRITE_COMPONENTS_HPP

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Rendering/DataStructures.hpp"
#include "Physics/TransformComponent.hpp"
#include "Core/Entity.hpp"
#include "Core/ComponentBase.hpp"


namespace game
{
class Texture;
class Shader;
class Game;

enum class SpriteType : uint32_t
{
  kNone = 0,
  kDefault,
  kTextured,
  kAnimated
};


class SpriteDataBase
{
public:
  virtual ~SpriteDataBase() noexcept = default;
  [[nodiscard]] virtual auto GetCopy() const noexcept -> Owner<SpriteDataBase*> { return new SpriteDataBase{*this}; }
};

class SpriteComponent : public ComponentBase
{
public:
  SpriteComponent(Entity *entity) : ComponentBase{entity} {}
  ~SpriteComponent() noexcept { delete data_; }
  SpriteComponent(const SpriteComponent &other) noexcept : ComponentBase{other.entity_}, type_{other.type_}, data_{other.data_->GetCopy()}, layer_{other.layer_} {}
  SpriteComponent(SpriteComponent &&other) noexcept : ComponentBase{other.entity_}, type_{other.type_}, data_{other.data_}, layer_{other.layer_} { other.data_ = nullptr; }
  SpriteComponent &operator=(const SpriteComponent &other) noexcept { ComponentBase::operator=(other); type_ = other.type_; data_ = other.data_->GetCopy(); layer_ = other.layer_;  return *this; };
  SpriteComponent &operator=(SpriteComponent &&other) noexcept { ComponentBase::operator=(other); type_ = other.type_; data_ = other.data_; other.data_ = nullptr; layer_ = other.layer_;  return *this; }

  [[nodiscard]] constexpr auto GetType() const noexcept -> SpriteType { return type_; }
  constexpr void SetType(const SpriteType type) noexcept { type_ = type; }
  [[nodiscard]] constexpr auto GetData() const noexcept -> const SpriteDataBase * { return data_; }
  [[nodiscard]] constexpr auto GetData() noexcept -> SpriteDataBase * { return data_; }
  constexpr void SetData(Owner<SpriteDataBase*> data) noexcept { data_ = data; }
  [[nodiscard]] constexpr auto GetLayer() const noexcept -> int { return layer_; }
  constexpr void SetLayer(int layer) noexcept { layer_ = layer; }

private:
  SpriteType type_;
  Owner<SpriteDataBase*> data_;
  int layer_ = 0;
};


class DefaultSpriteData : public SpriteDataBase
{
public:
  [[nodiscard]] virtual auto GetCopy() const noexcept -> Owner<SpriteDataBase*> override { return new DefaultSpriteData{*this}; }
};


class TexturedSpriteData : public SpriteDataBase
{
public:
  [[nodiscard]] virtual auto GetCopy() const noexcept -> Owner<SpriteDataBase*> override { return new TexturedSpriteData{*this}; }

  constexpr void SetShader(Shader *shader) noexcept { shader_ = shader; }
  [[nodiscard]] auto GetShader() noexcept -> Shader & { GAME_ASSERT(shader_ != nullptr); return *shader_; }
  [[nodiscard]] auto GetShader() const noexcept -> const Shader & { GAME_ASSERT(shader_ != nullptr); return *shader_; }
  constexpr void SetTexture(const Texture *texture) noexcept { texture_ = texture; }
  [[nodiscard]] auto GetTexture() const noexcept -> const Texture & { GAME_ASSERT(texture_ != nullptr); return *texture_; }

private:
  Shader *shader_ = nullptr;
  const Texture *texture_ = nullptr;
};


class AnimatedSpriteData : public SpriteDataBase
{
public:
  [[nodiscard]] virtual auto GetCopy() const noexcept -> Owner<SpriteDataBase*> override { return new AnimatedSpriteData{*this}; }

  void SetSpriteIndex(const Vector2i &sprite_index) noexcept { sprite_index_ = sprite_index; }
  [[nodiscard]] auto GetSpriteIndex() const noexcept -> const Vector2i & { return sprite_index_; }
  // Atlas step is 1 / max(width, height)
  constexpr void SetAtlasStep(const DefaultFloatType &atlas_step) noexcept { atlas_step_ = atlas_step; }
  [[nodiscard]] auto GetAtlasStep() const noexcept -> DefaultFloatType { return atlas_step_; }
  constexpr void SetShader(Shader *shader) noexcept { shader_ = shader; }
  [[nodiscard]] auto GetShader() noexcept -> Shader & { GAME_ASSERT(shader_ != nullptr); return *shader_; }
  [[nodiscard]] auto GetShader() const noexcept -> const Shader & { GAME_ASSERT(shader_ != nullptr); return *shader_; }
  constexpr void SetTexture(const Texture *texture) noexcept { texture_ = texture; }
  [[nodiscard]] auto GetTexture() const noexcept -> const Texture & { GAME_ASSERT(texture_ != nullptr); return *texture_; }

private:
  Vector2i sprite_index_ = Vector2i::Zero();
  DefaultFloatType atlas_step_ = 1;
  Shader *shader_ = nullptr;
  const Texture *texture_ = nullptr;
};
} // game

#endif // GAME_SPRITE_COMPONENT_HPP