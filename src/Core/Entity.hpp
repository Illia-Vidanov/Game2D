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
  Entity(Game &game) noexcept;

  [[nodiscard]] constexpr auto GetID() const noexcept -> entt::entity { return id_; }
  [[nodiscard]] constexpr auto GetGame() noexcept -> Game& { return game_; }
  [[nodiscard]] constexpr auto GetGame() const noexcept -> const Game& { return game_; }

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
};
} // game

#ifndef GAME_GAME_HPP
#include "Core/Entity.tpp"
#endif // GAME_GAME_HPP

#endif