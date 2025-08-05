#ifndef GAME_ENTITY_TPP
#define GAME_ENTITY_TPP

#include "Entity.hpp"

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/Type.hpp"
#include "Core/Game.hpp"

namespace game
{
inline Entity::Entity(Game &game, const std::string &name) noexcept : id_{game.GetRegistry().create()}, game_{game}, name_{name} {}

template<typename T>
[[nodiscard]] inline auto Entity::GetComponent() noexcept -> T & { GAME_ASSERT(HasComponent<T>()); return game_.GetRegistry().get<T>(id_); }
template<typename T>
[[nodiscard]] inline auto Entity::GetComponent() const noexcept -> const T & { GAME_ASSERT(HasComponent<T>()); return game_.GetRegistry().get<T>(id_); }
template<typename T>
[[nodiscard]] inline auto Entity::TryGetComponent() noexcept -> T * { return game_.GetRegistry().try_get<T>(id_); }
template<typename T>
[[nodiscard]] inline auto Entity::TryGetComponent() const noexcept -> const T * { return game_.GetRegistry().try_get<T>(id_); }
template<typename T>
inline auto Entity::AddComponent() noexcept -> T & { return game_.GetRegistry().emplace<T>(id_, this); }
template<typename T, typename... Args>
inline auto Entity::AddComponent(Args&&... args) noexcept -> T & { return game_.GetRegistry().emplace<T>(id_, this, args...); }
template<typename T>
[[nodiscard]] inline auto Entity::HasComponent() const noexcept -> bool { return game_.GetRegistry().all_of<T>(id_); }
template<typename... Args>
[[nodiscard]] inline auto Entity::HasAllComponents() const noexcept -> bool { return game_.GetRegistry().all_of<Args...>(id_); }
template<typename... Args>
[[nodiscard]] inline auto Entity::HasAnyComponent() const noexcept -> bool { return game_.GetRegistry().any_of<Args...>(id_); }
template<typename Pack>
[[nodiscard]] inline auto Entity::HasAnyComponentPack() const noexcept -> bool { return UnfoldHasAnyComponentPack<Pack>::Check(*this); }
} // game

#endif // GAME_ENTITY_TPP