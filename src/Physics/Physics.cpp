#include "Physics.hpp"

#include "Setup.hpp"

#include <vector>
#include <utility>

#include "Physics/Collider.hpp"
#include "Utils/Logger.hpp"


namespace game
{
void Physics::Update() noexcept
{
  for(const SquareCollider *collider : square_colliders_)
  {
    
  }
}
} // game