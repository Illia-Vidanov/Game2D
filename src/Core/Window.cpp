#include "Window.hpp"

#include "Setup.hpp"

#include "Core/Game.hpp"
#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Core/EventSystem.hpp"
#include "Rendering/Utils.hpp"


namespace game
{
Window::Window(Game &game) noexcept
  : game_(game)
  , event_cleaner_(game_.GetEventSystem())
{
  ZoneScopedC(0x00FFF2);

  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
  SDL_GL_SetSwapInterval(1); // Enable vsync

  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);
  width_ = display_mode.w / kStartupResDivFactor;
  height_ = display_mode.h / kStartupResDivFactor;

  sdl_window_ = SDL_CreateWindow(title_.c_str(),
                                 SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED,
                                 width_,
                                 height_,
                                 SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
  
  GAME_ASSERT(sdl_window_ != nullptr) << "Couldn't create sdl window: " << SDL_GetError();

  game_.GetEventSystem().AddListener(event_cleaner_, EventType::kWindowResize, this, [](const Event &event, void *window) -> bool { return reinterpret_cast<Window*>(window)->WindowResizedEvent(event); });
}

Window::~Window() noexcept
{
  ZoneScopedC(0x00FFF2);

  SDL_DestroyWindow(sdl_window_);
  SDL_Quit();
}

void Window::InitEvents() noexcept
{
  game_.GetEventSystem().DispatchEvent(Event{Event::WindowResize{0, 0, static_cast<int>(width_), static_cast<int>(height_)}});
}

void Window::DispatchSDLEvents() noexcept
{
  ZoneScopedC(0xe8bb25);

  SDL_Event event;
  #ifndef TRACY_ENABLE
  while(SDL_PollEvent(&event))
  {
  #else
  while(true)
  {
    {
      ZoneScopedNC("Poll SDL event", 0xe8bb25);
      int state = SDL_PollEvent(&event);
      if(!state)
        break;
      ZoneValue(event.type);
    }
  #endif
    switch(event.type)
    {
    case SDL_QUIT:
      game_.GetEventSystem().DispatchEvent(Event{Event::Quit{}});
      break;
    case SDL_KEYDOWN:
      game_.GetEventSystem().DispatchEvent(Event{Event::KeyDown{
        event.key.keysym.sym,
        event.key.keysym.scancode,
        event.key.keysym.mod
      }});
      break;
    case SDL_KEYUP:
      game_.GetEventSystem().DispatchEvent(Event{Event::KeyUp{
        event.key.keysym.sym,
        event.key.keysym.scancode,
        event.key.keysym.mod
      }});
      break;
    case SDL_WINDOWEVENT:
      switch(event.window.event)
      {
      case SDL_WINDOWEVENT_SIZE_CHANGED:
        if(width_ != event.window.data1 || height_ != event.window.data2)
          game_.GetEventSystem().DispatchEvent(Event{Event::WindowResize{width_, height_, event.window.data1, event.window.data2}});
        break;
      default:
        break;
      }
      break;
    default:
      break;
    }
  }
}

void Window::SetResolution(const int width, const int height) noexcept
{
  ZoneScopedC(0x00FFF2);

  width_ = width;
  height_ = height;

  GAME_ASSERT(width_ > 0 && height_ > 0) << "Widht: " << width_ << " and height: " << height_ << " of a window must be positive intagers";

  SDL_SetWindowSize(sdl_window_, width_, height_);
}

void Window::SetRenderResolution(const int render_width, const int render_height) noexcept
{
  if(render_width_ == render_width && render_height_ == render_height)
    return;
    
  render_width_ = render_width;
  render_height_ = render_height;
  
  pixels_per_unit_x_ = static_cast<DefaultFloatType>(render_width_) / static_cast<DefaultFloatType>(kUnitsPerScreenX);
  pixels_per_unit_y_ = static_cast<DefaultFloatType>(render_height_) / static_cast<DefaultFloatType>(kUnitsPerScreenY);
  
  game_.GetEventSystem().DispatchEvent(Event::RenderAreaResize{render_width_, render_height_, render_width, render_height});
}

void Window::SetTitle(const std::string &title) noexcept
{
  ZoneScopedC(0x00FFF2);

  title_ = title;

  SDL_SetWindowTitle(sdl_window_, title_.c_str());
}

auto Window::WindowResizedEvent(const Event &event) noexcept -> bool
{
  width_ = event.GetNewResolutionX();
  height_ = event.GetNewResolutionY();
  
  int smaller_axis = std::min(width_, height_);
  SetRenderResolution(smaller_axis, smaller_axis);

  return false;
}
}