#ifndef GAME_EVENTS_HPP
#define GAME_EVENTS_HPP

#include "Setup.hpp"


namespace game
{
namespace events
{
struct WindowResize
{
  int old_window_size_x;
  int old_window_size_y;
};

struct RenderAreaResize
{
  int old_render_area_size_x;
  int old_render_area_size_y;
};

struct Key
{
  SDL_Keycode keycode;
  SDL_Scancode scancode;
  uint16_t mod_keys;
};

struct MouseButton
{
  int button;
};

struct MouseMotion
{
  int position_x;
  int position_y;
  int delta_x;
  int delta_y;
};
} // events
} // game

#endif // GAME_EVENTS_HPP