#ifndef GAME_OPEN_GL_RENDERER
#define GAME_OPEN_GL_RENDERER

#include "Setup.hpp"

#include <queue>

namespace game
{
class Window;
class Game;
class Sprite;

class Renderer
{
public:
  Renderer(Game &game) noexcept;
  void Exit() noexcept;
  
  void StartFrame() noexcept;
  void AddToRenderQueue(const Sprite &sprite) noexcept { sprite_queue.push(&sprite); }
  void EndFrame() noexcept;
  
  private:
  void Render(const Sprite &sprite) noexcept;
  void PrintDebugInfo() const noexcept;

  Game &game_;
  Owner<void> *context_; // even in implementation SDL_GLContext is just typedef to void*
  std::queue<const Sprite *> sprite_queue;
};
} // game

#endif