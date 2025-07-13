#ifndef GAME_ENTITY_HPP
#define GAME_ENTITY_HPP

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"


namespace game
{
class Game;

class Entity
{
public:
  Entity(Game &game, const std::string &name) noexcept;
  Entity(const Entity &other) = delete;
  Entity(Entity &&other) = delete;
  Entity &operator=(const Entity &other) = delete;
  Entity &operator=(Entity &&other) = delete;

  [[nodiscard]] constexpr auto GetID() const noexcept -> entt::entity { return id_; }
  [[nodiscard]] constexpr auto GetGame() noexcept -> Game& { return game_; }
  [[nodiscard]] constexpr auto GetGame() const noexcept -> const Game& { return game_; }
  [[nodiscard]] constexpr auto GetName() const noexcept -> const std::string & { return name_; }
  constexpr void SetActive(bool active) noexcept { active_ = active; }
  [[nodiscard]] constexpr auto GetActive() const noexcept -> bool { return active_; }

  template<typename T>
  [[nodiscard]] auto GetComponent() noexcept -> T&;
  template<typename T>
  [[nodiscard]] auto GetComponent() const noexcept -> const T&;
  template<typename T>
  [[nodiscard]] auto TryGetComponent() noexcept -> T*;
  template<typename T>
  [[nodiscard]] auto TryGetComponent() const noexcept -> const T*;
  template<typename T>
  auto AddComponent() noexcept -> T&;
  template<typename T, typename... Args>
  auto AddComponent(Args&&... args) noexcept -> T&;
  template<typename T>
  [[nodiscard]] auto HasComponent() const noexcept -> bool;
  template<typename... Args>
  [[nodiscard]] auto HasAllComponents() const noexcept -> bool;

private:
  entt::entity id_;
  Game &game_;
  bool active_ = true;
  std::string name_; // Mostly for debugging purpuses. We might delete it in the future
};
} // game

#ifndef GAME_GAME_HPP
#include "Core/Entity.tpp"
#endif // GAME_GAME_HPP

#endif // GAME_ENTITY_HPP