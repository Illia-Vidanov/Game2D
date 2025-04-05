#include "Sprite.hpp"

#include "Setup.hpp"

#include <glad/glad.h>
#include <Eigen/Dense>
#include <Eigen/Geometry>

#include "Rendering/DataStructures.hpp"
#include "Utils/MathConstants.hpp"


namespace game
{
Sprite::Sprite(const Shader &shader, const Transform &transform) noexcept
  : shader_{shader}
  , transform_{transform}
{}
} // game