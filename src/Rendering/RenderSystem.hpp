#ifndef GAME_RENDER_SYSTEM_HPP
#define GAME_RENDER_SYSTEM_HPP

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Core/EventSystem.hpp"


namespace game
{
class Game;
class TransformComponent;
class SpriteComponent;
class OutlineComponent;

class RenderSystem
{
public:
  RenderSystem(Game &game) noexcept;
  ~RenderSystem() noexcept;
  RenderSystem(const RenderSystem &) noexcept = delete;
  RenderSystem(RenderSystem &&) noexcept = delete;
  RenderSystem &operator=(const RenderSystem &) noexcept = delete;
  RenderSystem &operator=(RenderSystem &&) noexcept = delete;
  
  void StartFrame() noexcept;
  void EndFrame() noexcept;

  [[nodiscard]] auto GetSDLGLContext() const noexcept -> SDL_GLContext { return sdl_gl_context_; }
  
private:
  void DrawUI() const noexcept;
  void DrawSprites() const noexcept;
  void DrawColoredScreenQuad(const Vector4 &color) const noexcept;

  void DrawDefaultSprite(SpriteComponent &sprite, TransformComponent &transform, const OutlineComponent *outline, const Matrix3 &view_projection_matrix) const noexcept;
  void DrawTexturedSprite(SpriteComponent &sprite, TransformComponent &transform, const OutlineComponent *outline, const Matrix3 &view_projection_matrix) const noexcept;
  void DrawAnimatedSprite(SpriteComponent &sprite, TransformComponent &transform, const OutlineComponent *outline, const Matrix3 &view_projection_matrix) const noexcept;

  void InitializeOutline() const noexcept;

  auto WindowResizeEvent(const Event &event) const noexcept -> bool;

  Game &game_;
  SDL_GLContext sdl_gl_context_;

  EventCleaner event_cleaner_;
};
} // game

#endif // GAME_RENDER_SYSTEM_HPP