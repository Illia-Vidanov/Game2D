#include "SpriteComponent.hpp"

#include "Setup.hpp"


namespace game
{
SpriteComponent::SpriteComponent(entt::entity self, Game &game) noexcept
  : self_{self}
  , game_{game}
{
}
} // game