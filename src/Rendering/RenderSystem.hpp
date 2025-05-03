#ifndef GAME_RENDER_SYSTEM_HPP
#define GAME_RENDER_SYSTEM_HPP

#include "Setup.hpp"


namespace game
{
class Game;

class RenderSystem
{
public:
  RenderSystem(Game &game) noexcept;
  void Exit() noexcept;
  
  void StartFrame() noexcept;
  void EndFrame() noexcept;
  
private:
  void PrintDebugInfo() const noexcept;

  Game &game_;
  Owner<void*> context_; // even in implementation SDL_GLContext is just typedef to void*
};
} // game

#endif // GAME_RENDER_SYSTEM_HPP