#ifndef GAME_SPRITE_COMPONENT_HPP
#define GAME_SPRITE_COMPONENT_HPP

#include "Setup.hpp"

#include <glad/glad.h>
#include <Eigen/Dense>
#include <Eigen/Geometry>

#include "Rendering/DataStructures.hpp"
#include "Utils/MathConstants.hpp"


namespace game
{
class Texture;
class Shader;

class SpriteComponent
{
public:
  constexpr inline void SetShader(const Shader &shader) noexcept { shader_ = &shader; }
  constexpr inline void SetTexture(const Texture &texture) noexcept { texture_ = &texture; }
  [[nodiscard]] constexpr inline auto GetShader() const noexcept -> const Shader & { return *shader_; }
  [[nodiscard]] constexpr inline auto GetTexture() const noexcept -> const Texture & { return *texture_; }

private:
  const Shader *shader_;
  const Texture *texture_;
};
} // game

#endif // GAME_SPRITE_COMPONENT_HPP