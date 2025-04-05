#ifndef GAME_WINDOW_HPP
#define GAME_WINDOW_HPP

#include <string>

#include "Setup.hpp"

#include <Eigen/Dense>

#include "Core/EventHandler.hpp"


class SDL_Window;

namespace game
{
class Game;

class Window
{
public:
  Window(Game &game) noexcept;
  void Exit() noexcept;
  void InitEvents() noexcept;

  [[nodiscard]] constexpr inline auto GetWidth() noexcept -> int { return width_; }
  [[nodiscard]] constexpr inline auto GetHeight() noexcept -> int { return height_; }
  [[nodiscard]] constexpr inline auto GetRenderWidth() noexcept -> uint16_t { return render_width_; }
  [[nodiscard]] constexpr inline auto GetRenderHeight() noexcept -> uint16_t { return render_height_; }
  [[nodiscard]] inline auto GetResolution() noexcept -> Eigen::Vector2i { return Eigen::Vector2i{width_, height_}; }
  void SetResolution(const Eigen::Vector2i &resolution) noexcept;
  
  [[nodiscard]] constexpr inline auto GetSDLWindow() noexcept -> SDL_Window* { return sdl_window_; }

  void DispatchSDLEvents() noexcept;

  [[nodiscard]] inline auto GetTitle() noexcept -> const std::string & { return title_; }
  void SetTitle(const std::string &title) noexcept;

  bool WindowResizedEvent(const Event &event) noexcept;

private:
  Game &game_;

  static constexpr inline int kStartupResDivFactor = 3;
  /// Use int instead of uint16_t as elsewhere because SDL requires pointers to int to fetch screen resolution
  int width_;
  int height_;
  uint16_t render_width_;
  uint16_t render_height_;
  std::string title_ = "Game";

  SDL_Window *sdl_window_;
  EventCleaner event_cleaner_;
};
}

#endif