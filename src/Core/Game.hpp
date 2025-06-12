#ifndef GAME_GAME_HPP
#define GAME_GAME_HPP

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Utils/FlagParser.hpp"
#include "Core/Window.hpp"
#include "Core/EventSystem.hpp"
#include "Rendering/RenderSystem.hpp"
#include "UI/UI.hpp"
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
  ~Game() noexcept;
  Game &operator=(const Game &) noexcept = default;
  Game(Game &&) noexcept = default;
  Game &operator=(Game &&) noexcept = default;

  void Run() noexcept;

  [[nodiscard]] constexpr auto GetUI() const noexcept -> const UI & { return ui_; }
  [[nodiscard]] constexpr auto GetUI() noexcept -> UI & { return ui_; }
  [[nodiscard]] constexpr auto GetFlags() noexcept -> Flags & { return flags_; }
  [[nodiscard]] constexpr auto GetFlags() const noexcept -> const Flags & { return flags_; }
  [[nodiscard]] constexpr auto GetCamera() noexcept -> Camera & { return camera_; }
  [[nodiscard]] constexpr auto GetCamera() const noexcept -> const Camera & { return camera_; }
  [[nodiscard]] constexpr auto GetWindow() noexcept -> Window & { return window_; }
  [[nodiscard]] constexpr auto GetWindow() const noexcept -> const Window & { return window_; }
  [[nodiscard]] constexpr auto GetRegistry() noexcept -> entt::registry & { return registry_; }
  [[nodiscard]] constexpr auto GetRegistry() const noexcept -> const entt::registry & { return registry_; }
  [[nodiscard]] constexpr auto GetInputSystem() noexcept -> InputSystem & { return input_system_; }
  [[nodiscard]] constexpr auto GetInputSystem() const noexcept -> const InputSystem & { return input_system_; }
  [[nodiscard]] constexpr auto GetEventSystem() noexcept -> EventSystem & { return event_system_; }
  [[nodiscard]] constexpr auto GetEventSystem() const noexcept -> const EventSystem & { return event_system_; }
  [[nodiscard]] constexpr auto GetRenderSystem() noexcept -> RenderSystem & { return render_system_; }
  [[nodiscard]] constexpr auto GetRenderSystem() const noexcept -> const RenderSystem & { return render_system_; }
  [[nodiscard]] constexpr auto GetPhysicsSystem() noexcept -> PhysicsSystem & { return physics_system_; }
  [[nodiscard]] constexpr auto GetPhysicsSystem() const noexcept -> const PhysicsSystem& { return physics_system_; }
  [[nodiscard]] constexpr auto GetResourceManager() noexcept -> ResourceManager & { return resource_manager_; }
  [[nodiscard]] constexpr auto GetResourceManager() const noexcept -> const ResourceManager & { return resource_manager_; }
  [[nodiscard]] constexpr auto GetDeltaTime() const noexcept -> std::chrono::duration<double> { return delta_time_; }
  [[nodiscard]] constexpr auto GetFixedDeltaTime() const noexcept -> std::chrono::duration<double> { return fixed_delta_time_; }
  [[nodiscard]] constexpr auto GetEntity(const std::string &name) noexcept -> Entity & { return entities_.at(name); }
  [[nodiscard]] constexpr auto GetEntity(const std::string &name) const noexcept -> const Entity & { return entities_.at(name); }

private:
  auto QuitEvent() noexcept -> bool;

  bool running_ = false;
  Flags flags_;
  
  EventSystem event_system_;
  EventCleaner event_cleaner_;
  Window window_;
  RenderSystem render_system_;
  UI ui_;
  ResourceManager resource_manager_;
  
  entt::registry registry_;
  InputSystem input_system_;
  PhysicsSystem physics_system_;
  Camera camera_;

  std::unordered_map<std::string, Entity> entities_;

  static constexpr inline uint64_t kFixedFPS = 60;
  static constexpr inline std::chrono::duration<double> kFixedDeltaTime = std::chrono::duration<double>{1.0f / static_cast<double>(kFixedFPS)};
  std::chrono::duration<double> delta_time_;
  std::chrono::duration<double> fixed_delta_time_ = kFixedDeltaTime;
  std::chrono::duration<double> fixed_delta_time_counter_ = kFixedDeltaTime;
  std::chrono::time_point<std::chrono::high_resolution_clock> frame_start_;
};
} // game

#endif // GAME_GAME_HPP