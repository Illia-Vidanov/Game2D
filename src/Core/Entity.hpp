#ifndef GAME_ENTITY_HPP
#define GAME_ENTITY_HPP

#include "Setup.hpp"

#include "Core/Game.hpp"
#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"


namespace game
{
class Entity
{
public:
  Entity(entt::entity id, Game &game) noexcept : id_{id}, game_{game} {};
  Entity(Game &game) noexcept : id_{game.GetRegistry().create()}, game_{game} {};

  [[nodiscard]] constexpr auto GetID() const noexcept -> entt::entity { return id_; }
  [[nodiscard]] constexpr auto GetGame() noexcept -> Game& { return game_; }
  [[nodiscard]] constexpr auto GetGame() const noexcept -> const Game& { return game_; }

  template<typename T>
  [[nodiscard]] auto GetComponent() noexcept -> T& { return game_.GetRegistry().get<T>(id_); }
  template<typename T>
  [[nodiscard]] auto TryGetComponent() noexcept -> T* { return game_.GetRegistry().try_get<T>(id_); }
  template<typename T>
  auto AddComponent() noexcept -> T& { return game_.GetRegistry().emplace<T>(id_, *this); }
  template<typename T>
  [[nodiscard]] auto HasComponent() const noexcept -> bool { return game_.GetRegistry().all_of<T>(id_); }
  template<typename... Args>
  [[nodiscard]] auto HasAllComponents() const noexcept -> bool { return game_.GetRegistry().all_of<Args...>(id_); }

private:
  entt::entity id_;
  Game &game_;
};
} // game

#endif