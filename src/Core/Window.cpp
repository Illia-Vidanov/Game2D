#include "Window.hpp"

#include "Setup.hpp"

#include "Core/Game.hpp"
#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Core/EventSystem.hpp"
#include "Rendering/Utils.hpp"
#include "Physics/TransformComponent.hpp"


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
  game_.GetEventSystem().DispatchEvent(Event{EventType::kWindowResize}.SetOldWindowWidth(0).SetOldWindowHeight(0).SetNewWindowWidth(width_).SetNewWindowHeight(height_));
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
      game_.GetEventSystem().DispatchEvent(Event{EventType::kQuit});
      
      break;

    case SDL_KEYDOWN:
      if(ImGui::GetIO().WantCaptureKeyboard)
        break;
      game_.GetEventSystem().DispatchEvent(Event{EventType::kKeyDown}.SetKeycode(event.key.keysym.sym).SetScancode(event.key.keysym.scancode).SetModKeys(event.key.keysym.mod));
      
      break;

    case SDL_KEYUP:
      if(ImGui::GetIO().WantCaptureKeyboard)
        break;
      game_.GetEventSystem().DispatchEvent(Event{EventType::kKeyUp}.SetKeycode(event.key.keysym.sym).SetScancode(event.key.keysym.scancode).SetModKeys(event.key.keysym.mod));
      
      break;

    case SDL_MOUSEMOTION:
      game_.GetEventSystem().DispatchEvent(Event{EventType::kMouseMotion}.SetNewMousePositionX(event.motion.x).SetNewMousePositionY(event.motion.y).SetMouseDeltaX(event.motion.xrel).SetMouseDeltaY(event.motion.yrel));
    
      break;

    case SDL_MOUSEBUTTONDOWN:
      if(ImGui::GetIO().WantCaptureMouse)
        break;
      game_.GetEventSystem().DispatchEvent(Event{EventType::kMouseButtonDown}.SetMouseButton(event.button.button));

      break;
    
    case SDL_MOUSEBUTTONUP:
      if(ImGui::GetIO().WantCaptureMouse)
        break;
      game_.GetEventSystem().DispatchEvent(Event{EventType::kMouseButtonUp}.SetMouseButton(event.button.button));

      break;
    
    case SDL_WINDOWEVENT:
      switch(event.window.event)
      {
      case SDL_WINDOWEVENT_SIZE_CHANGED:
        if(width_ != event.window.data1 || height_ != event.window.data2)
          game_.GetEventSystem().DispatchEvent(Event{EventType::kWindowResize}.SetOldWindowWidth(width_).SetOldWindowHeight(height_).SetNewWindowWidth(event.window.data1).SetNewWindowHeight(event.window.data2));
        
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
  render_pivot_x = (width_ - render_width) / 2;
  render_pivot_y = (height_ - render_height) / 2;

  if(render_width_ == render_width && render_height_ == render_height)
    return;
    
    
  pixels_per_unit_x_ = static_cast<DefaultFloatType>(render_width) / static_cast<DefaultFloatType>(kUnitsPerScreenX);
  pixels_per_unit_y_ = static_cast<DefaultFloatType>(render_height) / static_cast<DefaultFloatType>(kUnitsPerScreenY);
    
  game_.GetEventSystem().DispatchEvent(Event{EventType::kRenderAreaResize}.SetOldRenderAreaWidth(render_width_).SetOldRenderAreaHeight(render_height_).SetNewRenderAreaWidth(render_width).SetNewRenderAreaHeight(render_height));
    
  render_width_ = render_width;
  render_height_ = render_height;
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

auto Window::WindowResizedEvent(const Event &event) noexcept -> bool
{
  width_ = event.GetNewWindowWidth();
  height_ = event.GetNewWindowHeight();
  
  int smaller_axis = std::min(width_, height_);
  SetRenderResolution(smaller_axis, smaller_axis);

  return false;
}
}