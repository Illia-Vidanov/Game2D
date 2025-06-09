#ifndef GAME_RENDER_SYSTEM_HPP
#define GAME_RENDER_SYSTEM_HPP

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"


namespace game
{
class Game;

class RenderSystem
{
public:
  RenderSystem(Game &game) noexcept;
  ~RenderSystem() noexcept;
  
  void StartFrame() noexcept;
  void EndFrame() noexcept;
  
private:
  Game &game_;
  Owner<void*> context_; // even in implementation SDL_GLContext is just typedef to void*
};
} // game

#endif // GAME_RENDER_SYSTEM_HPP