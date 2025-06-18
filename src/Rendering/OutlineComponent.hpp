#ifndef GAME_OUTLINE_COMPONENT_HPP
#define GAME_OUTLINE_COMPONENT_HPP

#include "Setup.hpp"

#include "Rendering/SpriteComponents.hpp"
#include "Core/Entity.hpp"


namespace game
{
class OutlineComponent
{
public:
  OutlineComponent(Entity *entity) : entity_{entity} {}

  constexpr void SetWidth(DefaultFloatType width) noexcept { width_ = width; }
  [[nodiscard]] constexpr auto GetWidth() const noexcept -> DefaultFloatType { return width_; }
  void SetColor(const Vector4 &color) noexcept { color_ = color; }
  [[nodiscard]] auto GetColor() const noexcept -> const Vector4 & { return color_; }

private:
  Entity *entity_;

  Vector4 color_ = Vector4::Ones();
  DefaultFloatType width_ = 1.0f; // in units
};
} // game

#endif // GAME_OUTLINE_COMPONENT_HPP