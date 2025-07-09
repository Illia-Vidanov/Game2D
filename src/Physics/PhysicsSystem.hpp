#ifndef GAME_PHYSICS_SYSTEM_HPP
#define GAME_PHYSICS_SYSTEM_HPP

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"


template<>
struct std::hash<b2BodyId>
{
  auto operator()(const b2BodyId &body_id) const noexcept -> std::size_t { return static_cast<std::size_t>(body_id.index1) | (static_cast<std::size_t>(body_id.world0) << 32) | (static_cast<std::size_t>(body_id.generation) << 48); }
};

template<>
struct std::equal_to<b2BodyId>
{
  constexpr auto operator()(const b2BodyId &lhs, const b2BodyId &rhs) const noexcept -> bool { return lhs.index1 == rhs.index1 && lhs.world0 == rhs.world0 && lhs.generation == rhs.generation; }
};


namespace game
{
class Game;
class Entity;

class PhysicsSystem
{
public:
  PhysicsSystem(Game &game) noexcept;
  ~PhysicsSystem() noexcept;
  PhysicsSystem(const PhysicsSystem &) noexcept = delete;
  PhysicsSystem(PhysicsSystem &&) noexcept = delete;
  PhysicsSystem &operator=(const PhysicsSystem &) noexcept = delete;
  PhysicsSystem &operator=(PhysicsSystem &&) noexcept = delete;

  void FixedUpdate() noexcept;

  void Updateb2Transform(Entity *entity) const noexcept;

  [[nodiscard]] constexpr auto GetWorldId() const noexcept -> b2WorldId { return world_id_; }
  [[nodiscard]] auto TryGetb2BodyId(Entity* entity) const noexcept -> b2BodyId { std::unordered_map<Entity*, b2BodyId>::const_iterator it = entity_to_body_map_.find(entity); return it == entity_to_body_map_.end() ? b2_nullBodyId : it->second; }
  [[nodiscard]] auto Getb2BodyId(Entity* entity) const noexcept -> b2BodyId { GAME_ASSERT(Hasb2BodyId(entity)); return entity_to_body_map_.find(entity)->second; }
  [[nodiscard]] auto Hasb2BodyId(Entity* entity) const noexcept -> bool { return entity_to_body_map_.find(entity) != entity_to_body_map_.end(); }
  void Addb2BodyId(Entity* entity, b2BodyId body_id) noexcept { GAME_ASSERT(!Hasb2BodyId(entity)); entity_to_body_map_[entity] = body_id; body_to_entity_map_[body_id] = entity; }
  void RemoveEntity(Entity* entity) noexcept { GAME_ASSERT(Hasb2BodyId(entity)); std::unordered_map<Entity*, b2BodyId>::const_iterator it = entity_to_body_map_.find(entity); body_to_entity_map_.erase(it->second); entity_to_body_map_.erase(entity);}
  
  [[nodiscard]] auto GetEntity(b2BodyId body_id) const noexcept -> Entity* { GAME_ASSERT(HasEntity(body_id)); return body_to_entity_map_.find(body_id)->second; }
  [[nodiscard]] auto HasEntity(b2BodyId body_id) const noexcept -> bool { return body_to_entity_map_.find(body_id) != body_to_entity_map_.end(); }

  [[nodiscard]] static auto Isb2BodyIdNull(b2BodyId body_id) noexcept -> bool { return body_id.index1 == b2_nullBodyId.index1; } // The only way I found to check for validity. It might be also advisable to check for generation, but I am not sure...

  [[nodiscard]] auto TestPoint(const Vector2 &position) noexcept -> std::vector<Entity*>;

private:
  static constexpr inline DefaultFloatType kPointTestSize = 0.1f;

  Game &game_;

  b2WorldId world_id_;
  std::unordered_map<Entity*, b2BodyId> entity_to_body_map_;
  std::unordered_map<b2BodyId, Entity*> body_to_entity_map_;
};
} // game

#endif // GAME_PHYSICS_SYSTEM_HPP