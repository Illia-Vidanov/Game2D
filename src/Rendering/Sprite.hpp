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
class VAO;
class Shader;

class Sprite
{
public:
  Sprite(const Shader &shader, const Transform &transform) noexcept;

  [[nodiscard]] constexpr inline auto GetShader() const noexcept -> const Shader & { return shader_; }
  [[nodiscard]] constexpr inline auto GetTransform() const noexcept -> const Transform & { return transform_; }

  private:
  //const Texture &texture_;
  const Shader &shader_;
  const Transform &transform_;
  
};
} // game

#endif // GAME_SPRITE_HPP