#ifndef GAME_WINDOW_HPP
#define GAME_WINDOW_HPP

#include <string>

#include "Setup.hpp"

#include "Core/EventHandler.hpp"
#include "Utils/MathConstants.hpp"


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
  void Exit() noexcept;
  void InitEvents() noexcept;

  [[nodiscard]] constexpr inline auto GetWidth() noexcept -> int { return width_; }
  [[nodiscard]] constexpr inline auto GetHeight() noexcept -> int { return height_; }
  void SetResolution(const int width, const int height) noexcept;
  [[nodiscard]] constexpr inline auto GetRenderWidth() noexcept -> int { return render_width_; }
  [[nodiscard]] constexpr inline auto GetRenderHeight() noexcept -> int { return render_height_; }
  void SetRenderResolution(const int render_width, const int render_height) noexcept;
  [[nodiscard]] constexpr inline auto GetPixelsPerUnitX() noexcept -> int { return pixels_per_unit_x_; }
  [[nodiscard]] constexpr inline auto GetPixelsPerUnitY() noexcept -> int { return pixels_per_unit_y_; }
  
  [[nodiscard]] constexpr inline auto GetSDLWindow() noexcept -> SDL_Window* { return sdl_window_; }

  void DispatchSDLEvents() noexcept;

  [[nodiscard]] inline auto GetTitle() noexcept -> const std::string & { return title_; }
  void SetTitle(const std::string &title) noexcept;

  
  private:
  auto WindowResizedEvent(const Event &event) noexcept -> bool;

  
  Game &game_;
  
  static constexpr inline int kStartupResDivFactor = 3;
  /// Use int instead of uint16_t as elsewhere because SDL requires pointers to int to fetch screen resolution
  int width_;
  int height_;
  int render_width_ = 0;
  int render_height_ = 0;
  std::string title_ = "Game";

  DefFloatType pixels_per_unit_x_;
  DefFloatType pixels_per_unit_y_;

  SDL_Window *sdl_window_;
  EventCleaner event_cleaner_;
};
}

#endif