#include "EventHandler.hpp"

#include "Setup.hpp"

#include <utility>

#include <SDL2/SDL.h>
#include <tracy/Tracy.hpp>

#include "Utils/Enum.hpp"


namespace game
{
auto Event::GetName() const noexcept -> std::string
{
  switch(GetType())
  {
  case EventType::kNone:
    return "None";
  case EventType::kQuit:
    return "Quit";
  case EventType::kKeyDown:
    return "Key Down";
  case EventType::kKeyUp:
    return "Key Up";
  case EventType::kWindowResize:
    return "Window Resize";
    case EventType::kRenderAreaResize:
    return "Render Area Resize";
  default:
    return "Other: " + std::to_string(ToUnderlying(GetType()));
  }
}
} // game