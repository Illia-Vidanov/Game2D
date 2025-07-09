#include "Debug.hpp"

#include "Setup.hpp"

#include "Core/Entity.hpp"
#include "Core/Game.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "Player/InputSystem.hpp"
#include "Core/Window.hpp"


namespace game
{
void Debug::Update() noexcept
{
  UpdateSelected();
}

void Debug::UpdateSelected() noexcept
{
  if(!select_mode_)
    return;

  if(game_.GetInputSystem().GetMouseButton(MouseButton::kLeft))
  {
    selected_entities_.clear();
    std::vector<Entity*> found_entities = game_.GetPhysicsSystem().TestPoint(game_.GetWindow().ScreenToWorldPosition(game_.GetInputSystem().GetMousePosition()));
    if(!found_entities.empty())
      selected_entities_.emplace_back(found_entities[0]);
  }
}
} // game