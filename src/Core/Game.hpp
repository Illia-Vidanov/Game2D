#ifndef GAME_GAME_HPP
#define GAME_GAME_HPP

#include "Setup.hpp"

#include "entt.hpp"

#include "Utils/FlagParser.hpp"
#include "Core/Window.hpp"
#include "Core/EventSystem.hpp"
#include "Rendering/RenderSystem.hpp"
#include "Rendering/ResourceManager.hpp"
#include "Player/InputSystem.hpp"
#include "Physics/PhysicsSystem.hpp"


class SDL_Window;

int main(int argc, char **argv);

namespace game
{
// Main class of a game
class Game
{
  friend int ::main(int argc, char **argv);
public:
  [[nodiscard]] constexpr inline auto GetFlags() noexcept -> Flags& { return flags_; }
  [[nodiscard]] constexpr inline auto GetFlags() const noexcept -> const Flags& { return flags_; }
  [[nodiscard]] constexpr inline auto GetWindow() noexcept -> Window& { return window_; }
  [[nodiscard]] constexpr inline auto GetWindow() const noexcept -> const Window& { return window_; }
  [[nodiscard]] constexpr inline auto GetRenderSystem() noexcept -> RenderSystem& { return render_system_; }
  [[nodiscard]] constexpr inline auto GetRenderSystem() const noexcept -> const RenderSystem& { return render_system_; }
  [[nodiscard]] constexpr inline auto GetRegistry() noexcept -> entt::registry& { return registry_; }
  [[nodiscard]] constexpr inline auto GetRegistry() const noexcept -> const entt::registry& { return registry_; }
  [[nodiscard]] constexpr inline auto GetInputSystem() noexcept -> InputSystem& { return input_system_; }
  [[nodiscard]] constexpr inline auto GetInputSystem() const noexcept -> const InputSystem& { return input_system_; }
  [[nodiscard]] constexpr inline auto GetEventSystem() noexcept -> EventSystem& { return event_system_; }
  [[nodiscard]] constexpr inline auto GetEventSystem() const noexcept -> const EventSystem& { return event_system_; }
  [[nodiscard]] constexpr inline auto GetResourceManager() noexcept -> ResourceManager& { return resource_manager_; }
  [[nodiscard]] constexpr inline auto GetResourceManager() const noexcept -> const ResourceManager& { return resource_manager_; }

private:
  // Initializes game
  Game(const int argc, const char * const *argv) noexcept;

  // Main game loop
  void Run() noexcept;
  // Close application
  void Exit() noexcept;

  // Function to executre on Quit event
  auto QuitEvent() noexcept -> bool;

  bool running_ = false;
  Flags flags_;
  
  entt::registry registry_;
  entt::entity player_;

  EventSystem event_system_;
  EventCleaner event_cleaner_;
  Window window_;
  RenderSystem render_system_;
  ResourceManager resource_manager_;

  InputSystem input_system_;
  PhysicsSystem physics_system_;

};
} // game

#endif // GAME_GAME_HPP