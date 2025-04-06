#ifndef GAME_FILE_READER_HPP
#define GAME_FILE_READER_HPP

#include "Setup.hpp"

#include <string>

namespace game
{
struct ImageData
{
  uint8_t *data = nullptr;
  int width = 0;
  int height = 0;
  int channel_number = 0;

  void Free() noexcept;
};

class FileReader
{
public:
  FileReader() = delete;

  // Read txt file
  // Path can be either relative to executable or whole
  static auto Txt(const std::string &path) noexcept -> std::string;
  static auto Image(const std::string &path) noexcept -> ImageData;

private:
};
} // game

#endif // GAME_FILE_READER_HPP