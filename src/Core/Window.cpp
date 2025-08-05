#include "Window.hpp"

#include "Setup.hpp"

#include "Core/Game.hpp"
#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Core/Events.hpp"
#include "Rendering/Utils.hpp"
#include "Core/AllComponents.hpp"


namespace game
{
Window::Window(Game &game) noexcept
  : game_(game)
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
}

Window::~Window() noexcept
{
  ZoneScopedC(0x00FFF2);

  SDL_DestroyWindow(sdl_window_);
  SDL_Quit();
}

void Window::InitEvents() noexcept
{
  WindowResizeEvent();
  game_.GetRenderSystem().WindowResizeEvent();
  game_.GetUI().WindowResizeEvent();
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
    ImGui_ImplSDL2_ProcessEvent(&event);
    switch(event.type)
    {
    case SDL_QUIT:
      game_.QuitEvent();
      
      break;

    case SDL_KEYDOWN:
      if(ImGui::GetIO().WantCaptureKeyboard)
        break;
      game_.GetInputSystem().KeyDownEvent(events::Key{event.key.keysym.sym, event.key.keysym.scancode, event.key.keysym.mod});
      
      break;

    case SDL_KEYUP:
      if(ImGui::GetIO().WantCaptureKeyboard)
        break;
      game_.GetInputSystem().KeyUpEvent(events::Key{event.key.keysym.sym, event.key.keysym.scancode, event.key.keysym.mod});
      
      break;

    case SDL_MOUSEMOTION:
      if(ImGui::GetIO().WantCaptureMouse)
        break;
      game_.GetInputSystem().MouseMotionEvent(events::MouseMotion{event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel});
      
      break;

    case SDL_MOUSEBUTTONDOWN:
      if(ImGui::GetIO().WantCaptureMouse)
        break;
      game_.GetInputSystem().MouseButtonDownEvent(events::MouseButton{event.button.button});

      break;
    
    case SDL_MOUSEBUTTONUP:
      if(ImGui::GetIO().WantCaptureMouse)
        break;
      game_.GetInputSystem().MouseButtonUpEvent(events::MouseButton{event.button.button});

      break;
    
    case SDL_WINDOWEVENT:
      switch(event.window.event)
      {
      case SDL_WINDOWEVENT_SIZE_CHANGED:
        if(width_ == event.window.data1 && height_ == event.window.data2)
          continue;

        width_ = event.window.data1;
        height_ = event.window.data2;
        WindowResizeEvent();
        game_.GetRenderSystem().WindowResizeEvent();
        game_.GetUI().WindowResizeEvent();

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

void Window::SetRenderResolution(const int new_render_width, const int new_render_height) noexcept
{
  render_pivot_x = (width_ - new_render_width) / 2;
  render_pivot_y = (height_ - new_render_height) / 2;

  if(render_width_ == new_render_width && render_height_ == new_render_height)
    return;
    
  pixels_per_unit_x_ = static_cast<DefaultFloatType>(new_render_width) / static_cast<DefaultFloatType>(kUnitsPerScreenX);
  pixels_per_unit_y_ = static_cast<DefaultFloatType>(new_render_height) / static_cast<DefaultFloatType>(kUnitsPerScreenY);
    
  //game_.GetEventSystem().DispatchEvent(Event{EventType::kRenderAreaResize}.SetOldRenderAreaWidth(render_width_).SetOldRenderAreaHeight(render_height_).SetNewRenderAreaWidth(new_render_width).SetNewRenderAreaHeight(new_render_height));
    
  render_width_ = new_render_width;
  render_height_ = new_render_height;
}

void Window::SetTitle(const std::string &title) noexcept
{
  ZoneScopedC(0x00FFF2);

  title_ = title;

  SDL_SetWindowTitle(sdl_window_, title_.c_str());
}

auto Window::ScreenToWorldPosition(const Vector2i &screen_point) noexcept -> Vector2
{
  const Vector2 &camera_position = game_.FindCamera().GetEntity().GetComponent<TransformComponent>().GetPosition();
  return camera_position + Vector2{kUnitsPerScreenX * (static_cast<DefaultFloatType>(screen_point.x() - render_pivot_x) / static_cast<DefaultFloatType>(render_width_) - 0.5f), kUnitsPerScreenY * -(static_cast<DefaultFloatType>(screen_point.y() - render_pivot_y) / static_cast<DefaultFloatType>(render_height_) - 0.5f)};
}

void Window::WindowResizeEvent() noexcept
{
  int smaller_axis = std::min(width_, height_);
  SetRenderResolution(smaller_axis, smaller_axis);
}
}