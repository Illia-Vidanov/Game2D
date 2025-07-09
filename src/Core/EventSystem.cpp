#include "EventSystem.hpp"

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Utils/Enum.hpp"


namespace game
{
Event &Event::operator=(const Event &other) noexcept
{
  common_.type = other.common_.type;

  switch(other.common_.type)
  {
  case EventType::kNone:
    break;

  case EventType::kKeyDown:
  case EventType::kKeyUp:
    key_data_.keycode = other.key_data_.keycode;
    key_data_.scancode = other.key_data_.scancode;
    key_data_.mod_keys = other.key_data_.mod_keys;

    break;
  
  case EventType::kMouseMotion:
    mouse_motion_.new_mouse_position_x = other.mouse_motion_.new_mouse_position_x;
    mouse_motion_.new_mouse_position_y = other.mouse_motion_.new_mouse_position_y;
    mouse_motion_.mouse_delta_x = other.mouse_motion_.mouse_delta_x;
    mouse_motion_.mouse_delta_y = other.mouse_motion_.mouse_delta_y;

    break;
  
  case EventType::kWindowResize:
    window_resize_.new_width = other.window_resize_.new_width;
    window_resize_.new_height = other.window_resize_.new_height;
    window_resize_.old_width = other.window_resize_.old_width;
    window_resize_.old_height = other.window_resize_.old_height;

    break;

  case EventType::kRenderAreaResize:
    render_area_resize_.new_width = other.window_resize_.new_width;
    render_area_resize_.new_height = other.window_resize_.new_height;
    render_area_resize_.old_width = other.window_resize_.old_width;
    render_area_resize_.old_height = other.window_resize_.old_height;

    break;

  // kCustom
  default:
    custom_.data = other.custom_.data;

    break;
  }

  return *this;
}

Event &Event::operator=(Event &&other) noexcept
{
  common_.type = other.common_.type;

  switch(other.common_.type)
  {
  case EventType::kNone:
    break;

  case EventType::kKeyDown:
  case EventType::kKeyUp:
    key_data_.keycode = other.key_data_.keycode;
    key_data_.scancode = other.key_data_.scancode;
    key_data_.mod_keys = other.key_data_.mod_keys;

    break;
  
  case EventType::kMouseMotion:
    mouse_motion_.new_mouse_position_x = other.mouse_motion_.new_mouse_position_x;
    mouse_motion_.new_mouse_position_y = other.mouse_motion_.new_mouse_position_y;
    mouse_motion_.mouse_delta_x = other.mouse_motion_.mouse_delta_x;
    mouse_motion_.mouse_delta_y = other.mouse_motion_.mouse_delta_y;

    break;
  
  case EventType::kWindowResize:
    window_resize_.new_width = other.window_resize_.new_width;
    window_resize_.new_height = other.window_resize_.new_height;
    window_resize_.old_width = other.window_resize_.old_width;
    window_resize_.old_height = other.window_resize_.old_height;

    break;

  case EventType::kRenderAreaResize:
    render_area_resize_.new_width = other.window_resize_.new_width;
    render_area_resize_.new_height = other.window_resize_.new_height;
    render_area_resize_.old_width = other.window_resize_.old_width;
    render_area_resize_.old_height = other.window_resize_.old_height;

    break;

  // kCustom
  default:
    custom_.data = std::move(other.custom_.data);

    break;
  }

  return *this;
}

Event::~Event() noexcept
{
  if((common_.type & kCustomTypeMask) != 0u)
    custom_.data.~any();
}

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