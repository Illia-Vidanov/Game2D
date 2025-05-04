#ifndef GAME_SPRITE_COMPONENT_HPP
#define GAME_SPRITE_COMPONENT_HPP

#include "Setup.hpp"

#include "Rendering/DataStructures.hpp"


namespace game
{
class Texture;
class Shader;
class Game;

class SpriteComponent
{
public:
  SpriteComponent(entt::entity self, Game &game) noexcept;

  constexpr inline void SetShader(const Shader &shader) noexcept { shader_ = &shader; }
  constexpr inline void SetTexture(const Texture &texture) noexcept { texture_ = &texture; }
  [[nodiscard]] constexpr inline auto GetShader() const noexcept -> const Shader & { return *shader_; }
  [[nodiscard]] constexpr inline auto GetTexture() const noexcept -> const Texture & { return *texture_; }

private:
  entt::entity self_;
  Game &game_;

  const Shader *shader_;
  const Texture *texture_;
};
} // game

#endif // GAME_SPRITE_COMPONENT_HPP