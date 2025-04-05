#ifndef GAME_GAME_HPP
#define GAME_GAME_HPP

#include "Setup.hpp"

#include "Utils/FlagParser.hpp"
#include "Core/Window.hpp"
#include "Core/EventHandler.hpp"
#include "Rendering/Renderer.hpp"
#include "Rendering/ResourceManager.hpp"
#include "Game/Player.hpp"
#include "Game/Input.hpp"


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
  [[nodiscard]] constexpr inline auto GetInput() noexcept -> Input& { return input_; }
  [[nodiscard]] constexpr inline auto GetInput() const noexcept -> const Input& { return input_; }
  [[nodiscard]] constexpr inline auto GetWindow() noexcept -> Window& { return window_; }
  [[nodiscard]] constexpr inline auto GetWindow() const noexcept -> const Window& { return window_; }
  [[nodiscard]] constexpr inline auto GetRenderer() noexcept -> Renderer& { return renderer_; }
  [[nodiscard]] constexpr inline auto GetRenderer() const noexcept -> const Renderer& { return renderer_; }
  [[nodiscard]] constexpr inline auto GetEventHandler() noexcept -> EventHandler& { return event_handler_; }
  [[nodiscard]] constexpr inline auto GetEventHandler() const noexcept -> const EventHandler& { return event_handler_; }
  [[nodiscard]] constexpr inline auto GetResourceManager() noexcept -> ResourceManager& { return resource_manager_; }
  [[nodiscard]] constexpr inline auto GetResourceManager() const noexcept -> const ResourceManager& { return resource_manager_; }

private:
  /// Initializes game
  Game(const int argc, const char * const *argv) noexcept;

  /// Main game loop
  void Run() noexcept;
  /// Close application
  void Exit() noexcept;

  /// Function to executre on Quit event
  auto QuitEvent() noexcept -> bool;

  bool running_ = false;
  Flags flags_;
  EventHandler event_handler_;
  EventCleaner event_cleaner_;
  Window window_;
  Renderer renderer_;
  ResourceManager resource_manager_;

  Input input_;
  Player player_;
};
} // game

#endif // GAME_GAME_HPP