#ifndef GAME_WINDOW_HPP
#define GAME_WINDOW_HPP

#include "Setup.hpp"

#include "Utils/MathConstants.hpp"
#include "Utils/Logger.hpp"
#include "Core/EventSystem.hpp"


class SDL_Window;

namespace game
{
class Game;

class Window
{
public:
  constexpr inline static int kUnitsPerScreenX = 100;
  constexpr inline static int kUnitsPerScreenY = 100;

  Window(Game &game) noexcept;
  ~Window() noexcept;
  Window(const Window &) noexcept = delete;
  Window(Window &&) noexcept = delete;
  Window &operator=(const Window &) noexcept = delete;
  Window &operator=(Window &&) noexcept = delete;
  void InitEvents() noexcept;

  [[nodiscard]] constexpr auto GetWidth() noexcept -> int { return width_; }
  [[nodiscard]] constexpr auto GetHeight() noexcept -> int { return height_; }
  void SetResolution(const int width, const int height) noexcept;
  [[nodiscard]] constexpr auto GetRenderWidth() noexcept -> int { return render_width_; }
  [[nodiscard]] constexpr auto GetRenderHeight() noexcept -> int { return render_height_; }
  [[nodiscard]] constexpr auto GetRenderPivotX() noexcept -> int { return render_pivot_x; }
  [[nodiscard]] constexpr auto GetRenderPivotY() noexcept -> int { return render_pivot_y; }
  void SetRenderResolution(const int render_width, const int render_height) noexcept;
  [[nodiscard]] constexpr auto GetPixelsPerUnitX() noexcept -> int { return pixels_per_unit_x_; }
  [[nodiscard]] constexpr auto GetPixelsPerUnitY() noexcept -> int { return pixels_per_unit_y_; }
  
  [[nodiscard]] constexpr auto GetSDLWindow() noexcept -> SDL_Window* { return sdl_window_; }

  void DispatchSDLEvents() noexcept;

  [[nodiscard]] auto GetTitle() noexcept -> const std::string & { return title_; }
  void SetTitle(const std::string &title) noexcept;

  
  private:
  auto WindowResizedEvent(const Event &event) noexcept -> bool;

  
  Game &game_;
  
  static constexpr inline int kStartupResDivFactor = 3;
  int width_;
  int height_;
  int render_width_ = 0;
  int render_height_ = 0;
  int render_pivot_x = 0;
  int render_pivot_y = 0;
  std::string title_ = "Game";

  DefaultFloatType pixels_per_unit_x_;
  DefaultFloatType pixels_per_unit_y_;

  SDL_Window *sdl_window_;
  EventCleaner event_cleaner_;
};
}

#endif