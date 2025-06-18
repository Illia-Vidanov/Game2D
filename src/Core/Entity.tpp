#ifndef GAME_ENTITY_TPP
#define GAME_ENTITY_TPP

#include "Entity.hpp"

#include "Setup.hpp"

#include "Core/Game.hpp"

namespace game
{
inline Entity::Entity(Game &game) noexcept : id_{game.GetRegistry().create()}, game_{game} {}

template<typename T>
[[nodiscard]] inline auto Entity::GetComponent() noexcept -> T& { return game_.GetRegistry().get<T>(id_); }
template<typename T>
[[nodiscard]] inline auto Entity::TryGetComponent() noexcept -> T* { return game_.GetRegistry().try_get<T>(id_); }
template<typename T>
inline auto Entity::AddComponent() noexcept -> T& { return game_.GetRegistry().emplace<T>(id_, this); }
template<typename T, typename... Args>
inline auto Entity::AddComponent(Args&&... args) noexcept -> T& { return game_.GetRegistry().emplace<T>(id_, this, args...); }
template<typename T>
[[nodiscard]] inline auto Entity::HasComponent() const noexcept -> bool { return game_.GetRegistry().all_of<T>(id_); }
template<typename... Args>
[[nodiscard]] inline auto Entity::HasAllComponents() const noexcept -> bool { return game_.GetRegistry().all_of<Args...>(id_); }
} // game

#endif // GAME_ENTITY_TPP