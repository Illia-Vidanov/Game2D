#ifndef GAME_RENDER_SYSTEM_HPP
#define GAME_RENDER_SYSTEM_HPP

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Core/EventSystem.hpp"


namespace game
{
class Game;

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
  void DrawUI() noexcept;
  void DrawSprites() noexcept;

  auto WindowResizeEvent(const Event &event) const noexcept -> bool;

  Game &game_;
  SDL_GLContext sdl_gl_context_;

  EventCleaner event_cleaner_;
};
} // game

#endif // GAME_RENDER_SYSTEM_HPP