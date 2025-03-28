#ifndef GAME_GAME_HPP
#define GAME_GAME_HPP

#include "Setup.hpp"

#include "Utils/FlagParser.hpp"
#include "Core/Window.hpp"


class SDL_Window;

int main(int argc, char **argv);

namespace game
{
/// Main class of a game
class Game
{
  friend int ::main(int argc, char **argv);
public:
  [[nodiscard]] constexpr inline auto GetFlags() noexcept -> Flags& { return flags_; }
  [[nodiscard]] constexpr inline auto GetFlags() const noexcept -> const Flags& { return flags_; }
  [[nodiscard]] constexpr inline auto GetWindow() noexcept -> Window& { return window_; }
  [[nodiscard]] constexpr inline auto GetWindow() const noexcept -> const Window& { return window_; }

private:
  /// Initializes game
  Game(const int argc, const char * const *argv) noexcept;

  /// Main game loop
  void Run() noexcept;
  /// Close application
  void Exit() noexcept;

  bool running_ = false;
  Flags flags_;
  Window window_;
};
} // game

#endif // GAME_GAME_HPP