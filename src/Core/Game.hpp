#ifndef GAME_GAME_HPP
#define GAME_GAME_HPP

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Utils/FlagParser.hpp"
#include "Core/Window.hpp"
#include "Rendering/RenderSystem.hpp"
#include "UI/UI.hpp"
#include "Rendering/ResourceManager.hpp"
#include "Player/InputSystem.hpp"
#include "Player/CameraComponent.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "Core/Entity.hpp"
#include "Core/Debug.hpp"


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
  [[nodiscard]] constexpr auto GetDebug() noexcept -> Debug & { return debug_; }
  [[nodiscard]] constexpr auto GetDebug() const noexcept -> const Debug & { return debug_; }
  [[nodiscard]] constexpr auto GetWindow() noexcept -> Window & { return window_; }
  [[nodiscard]] constexpr auto GetWindow() const noexcept -> const Window & { return window_; }
  [[nodiscard]] constexpr auto GetRegistry() noexcept -> entt::registry & { return registry_; }
  [[nodiscard]] constexpr auto GetRegistry() const noexcept -> const entt::registry & { return registry_; }
  [[nodiscard]] constexpr auto GetInputSystem() noexcept -> InputSystem & { return input_system_; }
  [[nodiscard]] constexpr auto GetInputSystem() const noexcept -> const InputSystem & { return input_system_; }
  [[nodiscard]] constexpr auto GetRenderSystem() noexcept -> RenderSystem & { return render_system_; }
  [[nodiscard]] constexpr auto GetRenderSystem() const noexcept -> const RenderSystem & { return render_system_; }
  [[nodiscard]] constexpr auto GetPhysicsSystem() noexcept -> PhysicsSystem & { return physics_system_; }
  [[nodiscard]] constexpr auto GetPhysicsSystem() const noexcept -> const PhysicsSystem& { return physics_system_; }
  [[nodiscard]] constexpr auto GetResourceManager() noexcept -> ResourceManager & { return resource_manager_; }
  [[nodiscard]] constexpr auto GetResourceManager() const noexcept -> const ResourceManager & { return resource_manager_; }
  [[nodiscard]] constexpr auto GetDeltaTime() const noexcept -> std::chrono::duration<double> { return delta_time_; }
  [[nodiscard]] constexpr auto GetFixedDeltaTime() const noexcept -> std::chrono::duration<double> { return fixed_delta_time_; }
  [[nodiscard]] auto FindCamera() noexcept -> CameraComponent &;
  
  [[nodiscard]] auto GetEntity(const std::string &name) noexcept -> Entity & { return *entities_.at(name); }
  [[nodiscard]] auto GetEntity(const std::string &name) const noexcept -> const Entity & { return *entities_.at(name); }
  auto CreateEntity(const std::string &name) noexcept -> Entity & { GAME_ASSERT(entities_.find(name) == entities_.end()); return *(*entities_.emplace(name, new Entity{*this, name}).first).second; }
  auto GetOrCreateEntity(const std::string &name) noexcept -> Entity &;

  void QuitEvent() noexcept;

private:
  bool running_ = false;
  Flags flags_;
  
  Window window_;
  RenderSystem render_system_;
  UI ui_;
  ResourceManager resource_manager_;
  
  entt::registry registry_;
  InputSystem input_system_;
  PhysicsSystem physics_system_;
  Debug debug_;

  std::unordered_map<std::string, Entity*> entities_; // name searching of entities. For debugging, might be deleted

  static constexpr inline uint64_t kFixedFPS = 60;
  static constexpr inline std::chrono::duration<double> kFixedDeltaTime = std::chrono::duration<double>{1.0f / static_cast<double>(kFixedFPS)};
  std::chrono::duration<double> delta_time_;
  std::chrono::duration<double> fixed_delta_time_ = kFixedDeltaTime;
  std::chrono::duration<double> fixed_delta_time_counter_ = kFixedDeltaTime;
  std::chrono::time_point<std::chrono::high_resolution_clock> frame_start_;
};
} // game

#include "Core/Entity.tpp"

#endif // GAME_GAME_HPP