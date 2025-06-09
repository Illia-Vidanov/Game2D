#ifndef GAME_PHYSICS_SYSTEM_HPP
#define GAME_PHYSICS_SYSTEM_HPP

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"


namespace game
{
class Game;

class PhysicsSystem
{
  using MapType = std::unordered_map<entt::entity, b2BodyId>;
public:
  PhysicsSystem(Game &game);

  void Update() noexcept;

  [[nodiscard]] constexpr auto GetWorldId() const noexcept -> b2WorldId { return world_id_; }
  [[nodiscard]] auto Getb2BodyId(entt::entity entity) const noexcept -> b2BodyId { MapType::const_iterator it = body_ids_.find(entity); return it == body_ids_.end() ? b2_nullBodyId : it->second; }
  void Addb2BodyId(entt::entity entity, b2BodyId body_id) noexcept { body_ids_[entity] = body_id; }

  [[nodiscard]] static auto Isb2BodyIdNull(b2BodyId body_id) noexcept -> bool { return body_id.index1 == b2_nullBodyId.index1; } // The only way I found to check for validity. It might be also advisable to check for generation, but I am not sure...

private:
  Game &game_;

  b2WorldId world_id_;
  MapType body_ids_;
};
} // game

#endif // GAME_PHYSICS_SYSTEM_HPP