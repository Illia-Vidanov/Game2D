#ifndef GAME_GAME_HPP
#define GAME_GAME_HPP

#include "Setup.hpp"

#include "Utils/FlagParser.hpp"
#include "Core/Window.hpp"
#include "Core/EventSystem.hpp"
#include "Rendering/RenderSystem.hpp"
#include "Rendering/ResourceManager.hpp"
#include "Player/InputSystem.hpp"
#include "Player/Camera.hpp"
#include "Physics/PhysicsSystem.hpp"


namespace game
{
// Main class of a game
class Game
{
public:
  Game(const int argc, const char * const *argv) noexcept;
  void Run() noexcept;

  [[nodiscard]] constexpr inline auto GetFlags() noexcept -> Flags& { return flags_; }
  [[nodiscard]] constexpr inline auto GetFlags() const noexcept -> const Flags& { return flags_; }
  [[nodiscard]] constexpr inline auto GetCamera() noexcept -> Camera& { return camera_; }
  [[nodiscard]] constexpr inline auto GetCamera() const noexcept -> const Camera& { return camera_; }
  [[nodiscard]] constexpr inline auto GetWindow() noexcept -> Window& { return window_; }
  [[nodiscard]] constexpr inline auto GetWindow() const noexcept -> const Window& { return window_; }
  [[nodiscard]] constexpr inline auto GetRegistry() noexcept -> entt::registry& { return registry_; }
  [[nodiscard]] constexpr inline auto GetRegistry() const noexcept -> const entt::registry& { return registry_; }
  [[nodiscard]] constexpr inline auto GetInputSystem() noexcept -> InputSystem& { return input_system_; }
  [[nodiscard]] constexpr inline auto GetInputSystem() const noexcept -> const InputSystem& { return input_system_; }
  [[nodiscard]] constexpr inline auto GetEventSystem() noexcept -> EventSystem& { return event_system_; }
  [[nodiscard]] constexpr inline auto GetEventSystem() const noexcept -> const EventSystem& { return event_system_; }
  [[nodiscard]] constexpr inline auto GetRenderSystem() noexcept -> RenderSystem& { return render_system_; }
  [[nodiscard]] constexpr inline auto GetRenderSystem() const noexcept -> const RenderSystem& { return render_system_; }
  [[nodiscard]] constexpr inline auto GetPhysicsSystem() noexcept -> PhysicsSystem& { return physics_system_; }
  [[nodiscard]] constexpr inline auto GetPhysicsSystem() const noexcept -> const PhysicsSystem& { return physics_system_; }
  [[nodiscard]] constexpr inline auto GetResourceManager() noexcept -> ResourceManager& { return resource_manager_; }
  [[nodiscard]] constexpr inline auto GetResourceManager() const noexcept -> const ResourceManager& { return resource_manager_; }

private:
  void Exit() noexcept;
  
  auto QuitEvent() noexcept -> bool;

  bool running_ = false;
  Flags flags_;
  
  entt::registry registry_;
  entt::entity player_;
  entt::entity box_;
  
  EventSystem event_system_;
  EventCleaner event_cleaner_;
  Window window_;
  RenderSystem render_system_;
  ResourceManager resource_manager_;
  
  InputSystem input_system_;
  PhysicsSystem physics_system_;
  Camera camera_;
};
} // game

#endif // GAME_GAME_HPP