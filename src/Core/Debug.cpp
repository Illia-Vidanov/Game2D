#include "Debug.hpp"

#include "Setup.hpp"

#include "Core/Entity.hpp"
#include "Core/Game.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "Player/InputSystem.hpp"
#include "Core/Window.hpp"


namespace game
{
Debug::Debug(Game &game) noexcept
  : game_{game}
{

}

void Debug::Update() noexcept
{
  UpdateSelected();
}

void Debug::UpdateSelected() noexcept
{
  if(game_.GetInputSystem().GetKeyDown(SDLK_RIGHTBRACKET))
    active_ = !active_;

  if(!select_mode_)
    return;

  if(game_.GetInputSystem().GetMouseButtonUp(MouseButton::kLeft))
  {
    if((last_mouse_position_ - game_.GetInputSystem().GetMousePosition()).squaredNorm() < kSquaredSameMousePositionElipson)
      ++select_entity_layer_;
    else
      select_entity_layer_ = 0;

    GAME_DLOG(LogType::kInfo) << select_entity_layer_ << ' ' << (last_mouse_position_ - game_.GetInputSystem().GetMousePosition()).squaredNorm();

    last_mouse_position_ = game_.GetInputSystem().GetMousePosition();
    
    if(!game_.GetInputSystem().GetKey(SDLK_LCTRL))
      selected_entities_.clear();

    std::vector<Entity*> found_entities = game_.GetPhysicsSystem().TestPoint(game_.GetWindow().ScreenToWorldPosition(game_.GetInputSystem().GetMousePosition()));
    if(!found_entities.empty())
    {
      Entity *selected_entity = found_entities[std::min(found_entities.size() - 1, static_cast<std::size_t>(select_entity_layer_))];
      if(std::find(selected_entities_.begin(), selected_entities_.end(), selected_entity) == selected_entities_.end())
        selected_entities_.emplace_back(selected_entity);
    }
  }
}
} // game