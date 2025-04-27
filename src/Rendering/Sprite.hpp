#ifndef GAME_SPRITE_HPP
#define GAME_SPRITE_HPP

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

class Sprite
{
public:
  Sprite(const Shader &shader, const Texture &texture, const Transform &transform) noexcept;

  [[nodiscard]] constexpr inline auto GetShader() const noexcept -> const Shader & { return shader_; }
  [[nodiscard]] constexpr inline auto GetTransform() const noexcept -> const Transform & { return transform_; }
  [[nodiscard]] constexpr inline auto GetTexture() const noexcept -> const Texture & { return texture_; }

  private:
  const Shader &shader_;
  const Texture &texture_;
  const Transform &transform_;
};
} // game

#endif // GAME_SPRITE_HPP