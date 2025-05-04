#include "Camera.hpp"

#include "Setup.hpp"

#include "Core/Game.hpp"


namespace game
{
Camera::Camera(Game &game) noexcept
  : game_{game}
{}
} // game