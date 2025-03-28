#ifndef GAME_FILE_READER_HPP
#define GAME_FILE_READER_HPP

#include "Setup.hpp"

#include <string>

namespace game
{
class FileReader
{
public:
  FileReader() = delete;

  /// Read txt file
  /// Path can be either relative to executable or whole
  static std::string Txt(const std::string &path) noexcept;

private:
};
} // game

#endif // GAME_FILE_READER_HPP